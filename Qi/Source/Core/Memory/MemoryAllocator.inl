//
//  MemoryAllocator.inl
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <cstdlib>
#include <assert.h>
#include "../../Defines.h"

namespace Qi
{

template<class T>
T *MemoryAllocator::Allocate(unsigned long long numBytes, const char *filename, int lineNumber)
{
    assert(m_initialized);
    
    // For now, just use malloc until a more advanced system is ready.
    void *memory = (std::malloc(numBytes));
    T *result = new (memory) T; // placement new to run the constructor.
    
#ifdef QI_DEBUG
    MemoryRecord record;
    record.numBytes = numBytes;
    record.filename = std::string(filename);
    record.lineNumber = lineNumber;
    m_records[(void *)memory] = record;
#endif
    
    return result;
}

template<class T>
void MemoryAllocator::Free(T *address)
{
    assert(m_initialized);
    
    if (address != nullptr)
    {
        // For now, just use free() until a more advanced system is ready.
        std::free((void *)address);
        
#ifdef QI_DEBUG
        auto iter = m_records.find((void *)address);
        assert(iter != m_records.end());
        m_records.erase(iter);
#endif

        address = nullptr;
    }
}
    
} // namespace Qi
