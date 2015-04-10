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
    Pad(stream, padding);
    if (m_serializeFunction)
    {
        stream << ": ";
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
        member->GetData()->Serialize(&memberVariable, stream, padding);
               
        member = member->GetNextMember();
    }
    --padding;
    Pad(stream, padding);
    stream << "]" << std::endl;
}

void ReflectionData::SetSerializeFunction(SerializeFunction function)
{
    m_serializeFunction = function;
}
    
// ReflectionData implementation end ---------------------------------------------------------

} // namespace Qi

