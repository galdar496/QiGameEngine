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
T *MemoryAllocator::allocate(unsigned long long num_bytes, const char *filename, int line_number)
{
    assert(m_initialized);
    
    // For now, just use malloc until a more advanced system is ready.
    T *memory = static_cast<T *>(std::malloc(num_bytes));
    
#ifdef QI_DEBUG
    MemoryRecord record;
    record.num_bytes = num_bytes;
    record.filename = std::string(filename);
    record.line_number = line_number;
    m_records[(void *)memory] = record;
#endif
    
    return memory;
}

template<class T>
void MemoryAllocator::free(T *address)
{
    assert(m_initialized);
    
    if (address != nullptr)
    {
        // For now, just use free() until a more advanced system is ready.
        std::free(address);
        
#ifdef QI_DEBUG
        auto iter = m_records.find((void *)address);
        assert(iter != m_records.end());
        m_records.erase(iter);
#endif

        address = nullptr;
    }
}
    
} // namespace Qi
