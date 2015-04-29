//
//  ReflectionData.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ReflectionData.h"
#include "ReflectedVariable.h"
#include "PointerTable.h"
#include <iostream>

namespace Qi
{

// ReflectedMember implementation begin ------------------------------------------------------

	ReflectedMember::ReflectedMember(const std::string &name, size_t offset, size_t size, bool isPointer, const ReflectionData *reflectionData) :
    m_name(name),
    m_offset(offset),
	m_size(size),
	m_isPointer(isPointer),
    m_data(reflectionData)
{
}

ReflectedMember::~ReflectedMember()
{
}

const std::string &ReflectedMember::GetName() const
{
    return m_name;
}

size_t ReflectedMember::GetOffset() const
{
    return m_offset;
}

const ReflectionData *ReflectedMember::GetData() const
{
    return m_data;
}

size_t ReflectedMember::GetSize() const
{
	return m_size;
}

bool ReflectedMember::IsArray() const
{
	// If this is an array, m_size will contain the size of of the entire array
	// whereas the reflection data will contain the size of just one element of
	// the array.
	return (m_size > m_data->GetSize());
}

bool ReflectedMember::IsPointer() const
{
	return m_isPointer;
}

// ReflectedMember implementation end --------------------------------------------------------

// ReflectionData implementation begin -------------------------------------------------------

ReflectionData::ReflectionData(const std::string name, size_t size) :
    m_name(name),
    m_size(size),
    m_serializeFunction(nullptr),
	m_parent(nullptr)
{
}
    
ReflectionData::~ReflectionData()
{
	for (auto iter = m_members.begin(); iter != m_members.end(); ++iter)
	{
		delete *iter;
	}
}
    
void ReflectionData::Init(const std::string &name, size_t size)
{
	QI_ASSERT(size > 0 && "Invalid size specified as a reflected type");
    m_name = name;
    m_size = size;
}
    
const std::string &ReflectionData::GetName() const
{
    return m_name;
}
    
size_t ReflectionData::GetSize() const
{
    return m_size;
}

void ReflectionData::DeclareParent(const ReflectionData *parent)
{
	m_parent = parent;
}
    
void ReflectionData::AddMember(const ReflectedMember *member)
{
	m_members.push_back(const_cast<ReflectedMember *>(member));
}
    
bool ReflectionData::HasDataMembers() const
{
    return (!m_members.empty());
}

const ReflectedMember *ReflectionData::GetMember(const std::string &name) const
{
	for (auto iter = m_members.begin(); iter != m_members.end(); ++iter)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}

	return nullptr;
}

const ReflectionData::Members &ReflectionData::GetMembers() const
{
    return m_members;
}
    
void ReflectionData::PrintMembers() const
{
	// First, check to see if we have a parent type. If so, print its members first.
	if (m_parent)
	{
		m_parent->PrintMembers();
	}

    std::cout << "Members for type " << m_name << std::endl;
	for (auto iter = m_members.begin(); iter != m_members.end(); ++iter)
	{
		const ReflectedMember *member = *iter;
		std::cout << "\t" << member->GetData()->GetName() << " " << member->GetName() << ":" << member->GetOffset() << std::endl;
	}
}

void Pad(std::ostream &stream, uint32 pad)
{
    for (uint32 ii = 0; ii < pad; ++ii)
    {
        stream << "\t";
    }
}

#define PTR_ADD(PTR, OFFSET) \
((void *)(((char *)(PTR)) + (OFFSET)))

void ReflectionData::Serialize(const ReflectedVariable *variable, std::ostream &stream, PointerTable &pointerTable, uint32 padding) const
{
	// If this object has a parent, serialize its data first.
	if (m_parent)
	{
		m_parent->Serialize(variable, stream, pointerTable, padding);
	}

    // If this type has a valid serialization function then it knows how to serialize itself, let it.
    if (m_serializeFunction)
    {
        m_serializeFunction(variable, stream, pointerTable);
        return;
    }
    
    // For each member of this type, ask it to serialize itself. ///

	// Write out the name of this type along with this specific instance's index in the pointer table.
	{
		ReflectedVariable classInstance(variable->GetReflectionData(), variable->GetValue<void *>());
		stream << m_name << " * " << pointerTable.AddPointer(classInstance, false) << std::endl;
	}

	Pad(stream, padding);
    stream << "[" << std::endl;
    ++padding;
    //while (member)
	for (auto iter = m_members.begin(); iter != m_members.end(); ++iter)
    {
		const ReflectedMember *member = *iter;
		Pad(stream, padding);

		// If this type is an array, we have to serialize each element of the array before moving on 
		// to the next member variable.
		if (member->IsArray())
		{
			stream << member->GetName() << std::endl;
			++padding;
			const ReflectionData *data = member->GetData();
			size_t baseTypeSize = data->GetSize();
			QI_ASSERT(baseTypeSize > 0 && "Attempted to serialize unknown type");
			size_t arrayLength = member->GetSize() / baseTypeSize;
			for (size_t ii = 0; ii < member->GetSize(); ii += baseTypeSize)
			{
				Pad(stream, padding);

				// Get the next element to serialize.
				void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset() + ii);
				ReflectedVariable arrayElement(data, offsetData);
				data->Serialize(&arrayElement, stream, pointerTable, padding);
			}
			--padding;
		}
		else if (member->IsPointer())
		{
			void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset());
			ReflectedVariable memberVariable(member->GetData(), offsetData);

			stream << member->GetName() << " * " << pointerTable.AddPointer(memberVariable, true) << std::endl;
		}
		else // non-array/pointer type.
		{
			stream << member->GetName() << " ";
			void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset());
			ReflectedVariable memberVariable(member->GetData(), offsetData);
			member->GetData()->Serialize(&memberVariable, stream, pointerTable, padding);
		}
    }

    --padding;
    Pad(stream, padding);
    stream << "]" << std::endl;
}

void ReflectionData::Deserialize(ReflectedVariable *variable, std::istream &stream, PointerTable &pointerTable) const
{
	// If this object has a parent, deserialize its data first.
	if (m_parent)
	{
		m_parent->Deserialize(variable, stream, pointerTable);
	}

	// If this type has a valid deserialization function then it knows how to deserialize itself, let it.
	if (m_deserializeFunction)
	{
		m_deserializeFunction(variable, stream, pointerTable);
		return;
	}

	// For each member read from this object, ask it to deserialize itself if we have a definition for it.

	// Read the incoming '[' first.
	std::string streamInput;
	stream >> streamInput; // Read the class type first.
	QI_ASSERT(streamInput == m_name);
	
	// Read the index in the pointer table for this type. The format should be
	// "Type * index"
	PointerTable::TableIndex pointerIndex = 0;
	{
		stream >> streamInput;
		QI_ASSERT(streamInput == "*");
		
		stream >> pointerIndex;
	}

	// Read the starting bracket denoting the start of member variables for this type.
	{
		stream >> streamInput;
		QI_ASSERT(streamInput == "[");
	}

	// We may have already deserialized this object, check the pointer table for any references.
	bool shouldDeserialize = true;
	{
		ReflectedVariable tablePointer = pointerTable.GetPointer(pointerIndex);
		if (tablePointer.GetInstanceData() != nullptr)
		{
			// This instance has already been deserialized, just use that.
			*variable = tablePointer;
			shouldDeserialize = false;
		}
		else
		{
			// Add this instance to the pointer table so that future instances can reference it.
			pointerTable.AddPointer(*variable, pointerIndex);
		}
	}

	while (streamInput != "]")
	{
		// Read in the type.
		stream >> streamInput;
        QI_ASSERT(stream);

		// Only deserialize if we have to, otherwise just skip over this type.
		if (shouldDeserialize)
		{
			const ReflectedMember *member = GetMember(streamInput);
			if (member)
			{
				// If this member is an array type, read in each element of the array individually.
				if (member->IsArray())
				{
					const ReflectionData *data = member->GetData();
					size_t baseTypeSize = data->GetSize();
					size_t arrayLength = member->GetSize() / baseTypeSize;
					for (size_t ii = 0; ii < member->GetSize(); ii += baseTypeSize)
					{
						// Get the next element to serialize.
						void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset() + ii);
						ReflectedVariable arrayElement(data, offsetData);
						data->Deserialize(&arrayElement, stream, pointerTable);
					}
				}
				else // non-array type.
				{
					void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset());
					ReflectedVariable memberVariable(member->GetData(), offsetData);
					member->GetData()->Deserialize(&memberVariable, stream, pointerTable);
				}
			}
		}
	}
}

void ReflectionData::SetSerializeFunction(SerializeFunction function)
{
    m_serializeFunction = function;
}

void ReflectionData::SetDeserializeFunction(DeserializeFunction function)
{
	m_deserializeFunction = function;
}
    
// ReflectionData implementation end ---------------------------------------------------------

} // namespace Qi

