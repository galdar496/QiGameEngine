//
//  PointerTable.cpp
//  Qi Game Engine
//
//  Created by Cody White on 4/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "PointerTable.h"
#include "ReflectionDataManager.h"
#include "../Defines.h"
#include "ReflectionUtilities.h"
#include <istream>

namespace Qi
{

PointerTable::PointerTable()
{

}

PointerTable::~PointerTable()
{

}

void PointerTable::Populate(const ReflectedVariable &reflectedVariable, bool needsSerialization)
{
	if (!HasPointer(reflectedVariable))
	{
		// Add this object's instance to the table.
		AddPointer(reflectedVariable, needsSerialization);

		if (reflectedVariable.GetInstanceData() == nullptr)
		{
			// No need to keep processing this type, it is null.
			return;
		}

		// Loop over each of this member's variables and add them to the table using a depth-first
		// recursive call.
		const ReflectionData *reflectionData = reflectedVariable.GetReflectionData();
		for (auto iter = reflectionData->GetMembers().begin(); iter != reflectionData->GetMembers().end(); ++iter)
		{
			const ReflectedMember *member = *iter;
			
			// Only add objects who also have data members.
			if (member->GetReflectionData()->HasDataMembers() || member->IsPointer())
			{
				void *offsetData = PTR_ADD(reflectedVariable.GetInstanceData(), member->GetOffset());
				ReflectedVariable memberVariable(member->GetReflectionData(), offsetData);
				if (member->IsPointer())
				{
					void *pointerData = &(*(memberVariable.GetValue<char *>()));
					ReflectedVariable resolvedPointer(member->GetReflectionData(), pointerData);
					
					// Tell the serialization code that this variable needs to be manually serialized
					// as we don't have direct access to it under the current object.
					Populate(resolvedPointer, true);
				}
				else
				{
					Populate(memberVariable, false);
				}
			}
		}
	}
}

ReflectedVariable &PointerTable::GetPointer(TableIndex index)
{
	QI_ASSERT(index >= 0 && index < m_dataTable.size());
	return m_dataTable[index].first;
}

PointerTable::TableIndex PointerTable::GetIndex(const ReflectedVariable &variable) const
{
	PointerAddress address = reinterpret_cast<PointerAddress>(variable.GetInstanceData());
	LookupTable::const_iterator iter = m_lookupTable.find(address);
	
	QI_ASSERT(iter != m_lookupTable.end());

	// Look though the possible entires at this address to match up the typename to the passed
	// in variable type.
	const std::string &typeName = variable.GetReflectionData()->GetName();
	Objects::const_iterator objectIter = iter->second.begin();
	for (; objectIter != iter->second.end(); ++objectIter)
	{
		if (objectIter->first->GetName() == typeName)
		{
			return objectIter->second;
		}
	}

	// Shouldn't ever get here.
	QI_ASSERT(0);
	return static_cast<TableIndex>(-1);
}

void PointerTable::Serialize(std::ostream &stream)
{
	// First write out the size of the table.
	stream << m_dataTable.size() << std::endl;

	for (size_t ii = 0; ii < m_dataTable.size(); ++ii)
	{
		// Only serialize this object if it won't be serialized by some other object (is a child
		// of an object already being serialized).
		if (m_dataTable[ii].second)
		{
			//stream << std::endl; 
			const ReflectionData *reflectionData = m_dataTable[ii].first.GetReflectionData();
			if (reflectionData->HasParent())
			{
				// Write out the name of this type (so that the deserializer knows what type that any base classes
				// belong to).
				stream << "(" << reflectionData->GetName() << ") ";
			}

			const ReflectedVariable *tableVariable = &(m_dataTable[ii].first);

			// This function will auto-serialize all objects contained within the current object unless
			// they are only associated via pointers. In that case, only the index into the table will
			// be written.
			tableVariable->GetReflectionData()->Serialize(tableVariable, stream, *this);
		}
	}

	stream.flush();
}

void PointerTable::Deserialize(std::istream &stream)
{
	// The first thing in the stream should be the size of the pointer table.
	size_t tableSize = 0;
	stream >> tableSize;
	QI_ASSERT(tableSize > 0);

	m_dataTable.resize(tableSize);

	std::string streamInput;

	ReflectionDataManager &manager = ReflectionDataManager::GetInstance();

	// Eat the newline character.
	stream.ignore(256, '\n');

	while (stream.peek() > 0) // Valid characters have an ascii value > 0.
	{
		// See if there is a derived type that we're about to read in.
		bool inheritedObject = false;
		if (stream.peek() == '(')
		{
			// Read in the name of the derived type.
			stream >> streamInput;

			// The name will have surrounding () symbols, get rid of them.
			streamInput = streamInput.substr(1, streamInput.length() - 2);

			inheritedObject = true;
		}

		// Save off this position in the stream so that the deserialization code and read it again.
		std::streamoff streamPosition = stream.tellg();

		// Read in the table index for this variable.
		TableIndex index;
		stream >> index;
		QI_ASSERT(index >= 0 && index < tableSize);

		// If this isn't a derived type, just read in its name and start working with it.
		if (!inheritedObject)
		{
			// Read in the type.
			stream >> streamInput;
		}

		const ReflectionData *reflectionData = manager.GetReflectionData(streamInput);
		QI_ASSERT(reflectionData);

		// Allocate the space for this type.
		void *instanceData = reflectionData->AllocateInstance();
		ReflectedVariable variable(reflectionData, static_cast<void *>(instanceData));

		// Reset the stream position before moving on so that the reflection deserialization code can
		// read that info as well.
		stream.seekg(streamPosition);

		// Allow this variable to deserialize itself.
		reflectionData->Deserialize(&variable, stream, *this, false);

		// Eat the newline character.
		stream.ignore(256, '\n');
	} 

	// Fixup any pointers now that the entire table has been read.
	for (size_t ii = 0; ii < m_pointersToPatch.size(); ++ii)
	{ 
		PointerTable::TableIndex index = m_pointersToPatch[ii].first;
		ReflectedVariable tablePointer = m_dataTable[index].first;
		ReflectedVariable v = m_pointersToPatch[ii].second;

		// Set the pointer to the proper pointer in the table.
		v.GetValue<void *>() = (void *)tablePointer.GetInstanceData();
	}
}

void PointerTable::AddPatchPointer(PointerTable::TableIndex index, ReflectedVariable &pointer)
{
	std::pair<PointerTable::TableIndex, ReflectedVariable> pointerToPatch;
	pointerToPatch.first = index;
	pointerToPatch.second = pointer;
	m_pointersToPatch.push_back(pointerToPatch);
}

PointerTable::TableIndex PointerTable::AddPointer(const ReflectedVariable &pointer, bool needsSerialization)
{
	PointerAddress address = reinterpret_cast<PointerAddress>(pointer.GetInstanceData());
	LookupTable::iterator iter = m_lookupTable.find(address);
	if (iter != m_lookupTable.end())
	{
		// An entry for this address already exists. Make sure we have a matching entry for this type.
		const std::string &typeName = pointer.GetReflectionData()->GetName();
		Objects::iterator objectIter = iter->second.begin();
		for (; objectIter != iter->second.end(); ++objectIter)
		{
			if (objectIter->first->GetName() == typeName)
			{
				// This pointer already exists in the table, return its index.
				TableIndex index = objectIter->second;

				// If this pointer already exists and also wants to be serialized, update its
				// serialization status. There may be the case where the pointer does not need
				// to be serialized anymore as what it points to has already been serialized.
				if (m_dataTable[index].second == true)
				{
					m_dataTable[index].second = needsSerialization;
				}

				return index;
			}
		}

		// If we got to this point, then that means that this specific type doesn't exist at this address (for example, the first
		// member variable in a struct). Add it now.
		TableIndex index = m_dataTable.size();
		Instance instance(pointer.GetReflectionData(), index);
		TableRecord record(pointer, needsSerialization);
		m_dataTable.push_back(record);

		iter->second.push_back(instance);

		return index;
	}

	// We don't yet have an entry for this address. Create one and add it to the table.

	// First create the Objects list to push into the lookup table.
	TableIndex index = m_dataTable.size();
	Instance instance(pointer.GetReflectionData(), index);
	Objects objects;
	objects.push_back(instance);

	// Push the data onto the table record.
	TableRecord record(pointer, needsSerialization);
	m_dataTable.push_back(record);

	m_lookupTable[address] = objects;
	return index;
}

bool PointerTable::HasPointer(const ReflectedVariable &variable) const
{
	PointerAddress address = reinterpret_cast<PointerAddress>(variable.GetInstanceData());
	LookupTable::const_iterator iter = m_lookupTable.find(address);

	if (iter == m_lookupTable.end())
	{
		return false;
	}

	// We have an entry for this address, make sure that we have a type match as well.
	const std::string &typeName = variable.GetReflectionData()->GetName();
	for (Objects::const_iterator objectIter = iter->second.begin(); objectIter != iter->second.end(); ++objectIter)
	{
		if (objectIter->first->GetName() == typeName)
		{
			return true;
		}
	}

	return false;
}

} // namespace Qi