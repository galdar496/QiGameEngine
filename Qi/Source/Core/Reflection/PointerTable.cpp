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
#include <istream>

namespace Qi
{

PointerTable::PointerTable()
{

}

PointerTable::~PointerTable()
{

}

#define PTR_ADD(PTR, OFFSET) \
((void *)(((char *)(PTR)) + (OFFSET)))

void PointerTable::Populate(const ReflectedVariable &reflectedVariable, bool needsSerialization)
{
	if (!HasPointer(reflectedVariable))
	{
		// Add this object's instance to the table.
		AddPointer(reflectedVariable, needsSerialization);

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
	return iter->second;
}

void PointerTable::Serialize(std::ostream &stream)
{
	// First write out the size of the table.
	stream << m_dataTable.size();// << std::endl;

	for (size_t ii = 0; ii < m_dataTable.size(); ++ii)
	{
		// Only serialize this object if it won't be serialized by some other object (is a child
		// of an object already being serialized).
		if (m_dataTable[ii].second)
		{
			stream << std::endl; 
			const ReflectionData *reflectionData = m_dataTable[ii].first.GetReflectionData();
			if (reflectionData->HasParent())
			{
				// Write out the name of this type (so that the deserializer knows what type that any base classes
				// belong to).
				stream << "(" << reflectionData->GetName() << ") ";
			}

			stream << std::endl << ii << " ";

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
	std::vector<std::pair<PointerTable::TableIndex, ReflectedVariable> > pointersToPatch;

	do 
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
		char *instanceData = new char[reflectionData->GetSize()];
		ReflectedVariable variable(reflectionData, static_cast<void *>(instanceData));

		// Reset the stream position before moving on so that the reflection deserialization code can
		// read that info as well.
		stream.seekg(streamPosition);

		// Allow this variable to deserialize itself.
		reflectionData->Deserialize(&variable, stream, *this, pointersToPatch);

		// Eat the newline character.
		stream.ignore(256, '\n');

	} while (stream.good());


	// Fixup any pointers now that the entire table has been read.
	for (size_t ii = 0; ii < pointersToPatch.size(); ++ii)
	{ 
		PointerTable::TableIndex index = pointersToPatch[ii].first;
		ReflectedVariable tablePointer = m_dataTable[index].first;
		ReflectedVariable v = pointersToPatch[ii].second;

		// Set the pointer to the proper pointer in the table.
		v.GetValue<void *>() = (void *)tablePointer.GetInstanceData();
	}
}

PointerTable::TableIndex PointerTable::AddPointer(const ReflectedVariable &pointer, bool needsSerialization)
{
	PointerAddress address = reinterpret_cast<PointerAddress>(pointer.GetInstanceData());
	LookupTable::iterator iter = m_lookupTable.find(address);
	if (iter != m_lookupTable.end())
	{
		// This pointer already exists in the table, return its index.
		TableIndex index = iter->second;

		// If this pointer already exists and also wants to be serialized, update its
		// serialization status. There may be the case where the pointer does not need
		// to be serialized anymore as what it points to has already been serialized.
		if (m_dataTable[index].second == true)
		{
			m_dataTable[index].second = needsSerialization;
		}

		return index;
	}

	TableIndex index = m_dataTable.size();
	TableRecord record(pointer, needsSerialization);
	m_dataTable.push_back(record);

	m_lookupTable[address] = index;
	return index;
}

bool PointerTable::HasPointer(const ReflectedVariable &variable) const
{
	PointerAddress address = reinterpret_cast<PointerAddress>(variable.GetInstanceData());
	LookupTable::const_iterator iter = m_lookupTable.find(address);

	return (iter != m_lookupTable.end());
}

} // namespace Qi