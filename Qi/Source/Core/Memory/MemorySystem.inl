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
#include <stdalign.h>

namespace Qi
{

template<class T>
T *MemorySystem::Allocate(uint32 numBytes, const char *filename, int lineNumber)
{
    QI_ASSERT(m_initialized);
    
	uint32 totalSize = numBytes + sizeof(MemoryHeader);
	void *memory = m_allocator->Allocate(totalSize, alignof(T));

	// Place a small header at the beginning of the memory block that denotes the size 
	// of this memory.
	MemoryHeader header;
	header.memorySize = numBytes;
	std::memcpy(memory, &header, sizeof(MemoryHeader));

	// Run placement new over each allocated element.
	size_t numElements = numBytes / sizeof(T);
	T *result = (T *)(static_cast<unsigned char *>(memory) + sizeof(MemoryHeader));
	for (size_t ii = 0; ii < numElements; ++ii)
	{
		 new (result + ii) T();
	}

#ifdef QI_DEBUG
    MemoryRecord record;
    record.filename = std::string(filename);
    record.lineNumber = lineNumber;
    m_records[memory] = record;
#endif
    
    return result;
}

template<class T>
void MemorySystem::Free(T *address)
{
    QI_ASSERT(m_initialized);
    
    if (address != nullptr)
    {
		// Adjust the address to account for the header pre-pended during allocation and extract
		// that header information.
		void *adjustedAddress = (void *)((unsigned char *)address - sizeof(MemoryHeader));
		MemoryHeader header;
		std::memcpy(&header, adjustedAddress, sizeof(MemoryHeader));

	#ifdef QI_DEBUG
		auto iter = m_records.find(adjustedAddress);
        QI_ASSERT(iter != m_records.end());
		m_records.erase(iter);
	#endif

		// Be sure to invoke the destructor on each object that this 
		// allocation references. The loop uses descending order to preserve
		// the destruction order of C++, the object that was constructed last
		// should be destroyed first.
		size_t index = header.memorySize / sizeof(T);
		while (index)
		{
			--index;
			address[index].~T();
		}

		m_allocator->Deallocate(adjustedAddress);
        address = nullptr;
    }
}
    
} // namespace Qi
