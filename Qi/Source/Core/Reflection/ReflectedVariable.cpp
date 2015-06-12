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
	PointerTable table;
	table.Populate(*this, true);
	std::stringstream tmpStream;
	//m_reflectionData->Serialize(this, tmpStream, table);

	// At this point, we'll have a valid pointer table that needs to be serialized.
	// Write that to the output stream first followed by the serialization of this
	// reflected variable.
	table.Serialize(stream);

	stream << tmpStream.str();
}

void ReflectedVariable::Deserialize(std::istream &stream)
{
	// First, deserialize the pointer table which is located
	// at the beginning of the stream.
	PointerTable table;
	table.Deserialize(stream);

	m_reflectionData->Deserialize(this, stream, table);
}

} // namespace Qi



