//
//  ReflectionData.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ReflectionData.h"
#include <iostream>

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

// ReflectedMember implementation end --------------------------------------------------------

// ReflectionData implementation begin -------------------------------------------------------

ReflectionData::ReflectionData(const std::string name, size_t size) :
    m_name(name),
    m_size(size),
    m_members(nullptr)
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
        std::cout << "\t" << member->GetData()->GetName() << " " << member->GetName() << std::endl;
        member = member->GetNextMember();
    }
}
    
// ReflectionData implementation end ---------------------------------------------------------

} // namespace Qi

