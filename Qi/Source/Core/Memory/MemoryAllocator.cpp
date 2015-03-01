//
//  MemoryAllocator.cpp
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "MemoryAllocator.h"
#include "../Utility/Logger.h"
#include "../../Defines.h"
#include <assert.h>

namespace Qi
{



MemoryAllocator::MemoryAllocator() :
    m_initialized(false)
{
}

MemoryAllocator::~MemoryAllocator()
{
}

MemoryAllocator &MemoryAllocator::getInstance()
{
    static MemoryAllocator allocator;
    return allocator;
}
    
bool MemoryAllocator::init()
{
    assert(!m_initialized);
    m_initialized = true;
    return m_initialized;
}
    
void MemoryAllocator::deinit()
{
    assert(m_initialized);
    
#ifdef QI_DEBUG
    if (!m_records.empty())
    {
        Qi_LogWarning("Memory leaks detected:");
        for (auto iter = m_records.begin(); iter != m_records.end(); ++iter)
        {
            Qi_LogWarning("\tLeak: %s(%d) - %u bytes", iter->second.filename.c_str(),
                                                       iter->second.line_number,
                                                       iter->second.num_bytes);
        }
    }

    m_records.clear();
#endif

    m_initialized = false;
}
    
} // namespace Qi

