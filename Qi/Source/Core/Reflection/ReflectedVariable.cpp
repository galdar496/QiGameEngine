//
//  ReflectedVariable.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ReflectedVariable.h"
#include "ReflectionData.h"

namespace Qi
{

ReflectedVariable::ReflectedVariable() :
    m_reflectionData(nullptr),
    m_instanceData(nullptr)
{
}

ReflectedVariable::ReflectedVariable(const ReflectedVariable &other)
{
    m_reflectionData = other.m_reflectionData;
    m_instanceData   = other.m_instanceData;
}

ReflectedVariable::~ReflectedVariable()
{
    m_reflectionData = nullptr;
    m_instanceData   = nullptr;
}

ReflectedVariable::ReflectedVariable(const ReflectionData *reflectionData, const void *instanceData) :
    m_reflectionData(reflectionData),
    m_instanceData(instanceData)
{
}

const ReflectionData *ReflectedVariable::GetReflectionData() const
{
    return m_reflectionData;
}

const void *ReflectedVariable::GetInstanceData() const
{
    return m_instanceData;
}

void ReflectedVariable::Serialize(std::ostream &stream) const
{
    m_reflectionData->Serialize(this, stream);
}

} // namespace Qi



