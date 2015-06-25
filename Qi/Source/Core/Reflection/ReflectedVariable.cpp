//
//  ReflectedVariable.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ReflectedVariable.h"
#include "ReflectionData.h"
#include "PointerTable.h"
#include <sstream>

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


ReflectedVariable &ReflectedVariable::operator=(const ReflectedVariable &other)
{
	if (this != &other)
	{
		m_reflectionData = other.m_reflectionData;
		m_instanceData = other.m_instanceData;
	}

	return *this;
}

ReflectedVariable::~ReflectedVariable()
{
    m_reflectionData = nullptr;
    m_instanceData   = nullptr;
}

ReflectedVariable::ReflectedVariable(const ReflectionData *reflectionData, void *instanceData) :
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

void ReflectedVariable::SetInstanceData(const void *data)
{
	m_instanceData = const_cast<void *>(data);
}

void ReflectedVariable::Serialize(std::ostream &stream) const
{
	// Add all objects that are referenceable from this variable
	// to the pointer table. This table will then be used to patch
	// pointers later while serializing.
	PointerTable table;
	table.Populate(*this, true);

	// At this point, we'll have a valid pointer table that needs to be serialized.
	table.Serialize(stream);
}

void ReflectedVariable::Deserialize(std::istream &stream)
{
	// Create the pointer table to use for pointer patching while deserializing.
	PointerTable table;

	// Deserialize the stream into the table.
	table.Deserialize(stream);

	// Extract the first element of the table since element 0 represents 
	// the main (parent) variable being extracted.
	this->GetValue<void *>() = table.GetPointer(0).m_instanceData;
}

} // namespace Qi



