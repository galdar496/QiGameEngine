//
//  ReflectionData.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <iostream>
#include "ReflectionData.h"
#include "ReflectedVariable.h"

namespace Qi
{

// ReflectedMember implementation begin ------------------------------------------------------

ReflectedMember::ReflectedMember(const std::string &name, size_t offset, const ReflectionData *reflectionData) :
    m_name(name),
    m_offset(offset),
    m_data(reflectionData),
    m_nextMember(nullptr)
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

ReflectedMember *const& ReflectedMember::GetNextMember() const
{
    return m_nextMember;
}

ReflectedMember *& ReflectedMember::GetNextMember()
{
    return m_nextMember;
}

bool ReflectedMember::HasMembers() const
{
    return (m_nextMember != nullptr);
}

// ReflectedMember implementation end --------------------------------------------------------

// ReflectionData implementation begin -------------------------------------------------------

ReflectionData::ReflectionData(const std::string name, size_t size) :
    m_name(name),
    m_size(size),
    m_members(nullptr),
    m_lastMember(nullptr),
    m_serializeFunction(nullptr)
{
}
    
ReflectionData::~ReflectionData()
{
}
    
void ReflectionData::Init(const std::string &name, size_t size)
{
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
    
void ReflectionData::AddMember(const ReflectedMember *member)
{
    if (m_members)
    {
        m_lastMember->GetNextMember() = const_cast<ReflectedMember *>(member);
    }
    else
    {
        m_members = const_cast<ReflectedMember *>(member);
    }
    
    m_lastMember = const_cast<ReflectedMember *>(member);
}
    
bool ReflectionData::HasDataMembers() const
{
    return (m_members != nullptr);
}

const ReflectedMember *ReflectionData::GetMember(const std::string &name) const
{
	const ReflectedMember *member = m_members;
	while (member)
	{
		if (member->GetName() == name)
		{
			return member;
		}

		member = member->GetNextMember();
	}

	return nullptr;
}

const ReflectedMember *ReflectionData::GetMembers() const
{
    return m_members;
}
    
void ReflectionData::PrintMembers() const
{
    const ReflectedMember *member = m_members;
    
    std::cout << "Members for type " << m_name << std::endl;
    while (member)
    {
        std::cout << "\t" << member->GetData()->GetName() << " " << member->GetName() << ":" << member->GetOffset() << std::endl;
        member = member->GetNextMember();
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

void ReflectionData::Serialize(const ReflectedVariable *variable, std::ostream &stream, uint32 padding) const
{
    // If this type has a valid serialization function then it knows how to serialize itself, let it.
    if (m_serializeFunction)
    {
        m_serializeFunction(variable, stream);
        return;
    }
    
    // For each member of this type, ask it to serialize itself.
    const ReflectedMember *member = m_members;
    stream << m_name << std::endl;
	Pad(stream, padding);
    stream << "[" << std::endl;
    ++padding;
    while (member)
    {
        void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset());
        ReflectedVariable memberVariable(member->GetData(), offsetData);
		Pad(stream, padding);
		stream << member->GetName() << " ";
        member->GetData()->Serialize(&memberVariable, stream, padding);
               
        member = member->GetNextMember();
    }
    --padding;
    Pad(stream, padding);
    stream << "]" << std::endl;
}

void ReflectionData::Deserialize(ReflectedVariable *variable, std::istream &stream) const
{
	// If this type has a valid deserialization function then it knows how to deserialize itself, let it.
	if (m_deserializeFunction)
	{
		m_deserializeFunction(variable, stream);
		return;
	}

	// For each member read from this object, ask it to deserialize itself if we have a definition for it.

	// Read the incoming '[' first.
	std::string streamInput;
	stream >> streamInput; // Read the class type first.
	QI_ASSERT(streamInput == variable->GetReflectionData()->GetName());
	stream >> streamInput;
	QI_ASSERT(streamInput == "[");

	while (streamInput != "]")
	{
		// Read in the type.
		stream >> streamInput;
		const ReflectedMember *member = GetMember(streamInput);
		if (member)
		{
			void *offsetData = PTR_ADD(variable->GetInstanceData(), member->GetOffset());
			ReflectedVariable memberVariable(member->GetData(), offsetData);
			member->GetData()->Deserialize(&memberVariable, stream);
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

