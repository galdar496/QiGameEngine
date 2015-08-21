//
//  MemorySystem.inl
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "../Defines.h"
#include <cstdlib>

namespace Qi
{

template<class T>
T *MemorySystem::Allocate(const char *filename, int lineNumber)
{
    QI_ASSERT(m_initialized);
 
	T *result = (T *)m_allocator->Allocate(sizeof(T));

	new (result) T;

#ifdef QI_DEBUG
    MemoryRecord record;
    record.filename = std::string(filename);
    record.lineNumber = lineNumber;
	record.numBytes = sizeof(T);
	record.isArray = false;
    m_records[static_cast<void *>(result)] = record;
#endif
    
    return result;
}

template<class T>
T *MemorySystem::AllocateArray(uint32 arraySize, const char *filename, int lineNumber)
{
	QI_ASSERT(m_initialized);

	T *result = (T *)m_allocator->Allocate(sizeof(T) * arraySize);

	new (result) T[arraySize];

#ifdef QI_DEBUG
	MemoryRecord record;
	record.filename = std::string(filename);
	record.lineNumber = lineNumber;
	record.numBytes = sizeof(T);
	record.isArray = true;
	m_records[static_cast<void *>(result)] = record;
#endif

	return result;

	return nullptr;
}

template<class T>
void MemorySystem::Free(T *address)
{
    QI_ASSERT(m_initialized);
    
    if (address != nullptr)
    {

	#ifdef QI_DEBUG
		auto iter = m_records.find(static_cast<void *>(address));
        QI_ASSERT(iter != m_records.end());
		QI_ASSERT(iter->second.isArray == false);
		m_records.erase(iter);
	#endif

		m_allocator->Deallocate(address);
    }
}

template<class T>
void MemorySystem::FreeArray(T *address)
{
	QI_ASSERT(m_initialized);

	if (address != nullptr)
	{

#ifdef QI_DEBUG
		auto iter = m_records.find(static_cast<void *>(address));
		QI_ASSERT(iter != m_records.end());
		QI_ASSERT(iter->second.isArray == true);
		m_records.erase(iter);
#endif

		m_allocator->Deallocate(address);
	}
}
    
} // namespace Qi
