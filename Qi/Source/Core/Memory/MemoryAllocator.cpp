//
//  MemoryAllocator.cpp
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "MemoryAllocator.h"
#include "../Utility/Logger/Logger.h"
#include "../Defines.h"

namespace Qi
{



MemoryAllocator::MemoryAllocator() :
    m_initialized(false)
{
}

MemoryAllocator::~MemoryAllocator()
{
}

MemoryAllocator &MemoryAllocator::GetInstance()
{
    static MemoryAllocator allocator;
    return allocator;
}
    
Result MemoryAllocator::Init()
{
    QI_ASSERT(!m_initialized);
    m_initialized = true;
    return Result(ReturnCode::kSuccess);
}
    
void MemoryAllocator::Deinit()
{
    QI_ASSERT(m_initialized);
    
#ifdef QI_DEBUG
    if (!m_records.empty())
    {
        Qi_LogWarning("Memory leaks detected:");
        for (auto iter = m_records.begin(); iter != m_records.end(); ++iter)
        {
            Qi_LogWarning("\tLeak: %s(%d) - %u bytes", iter->second.filename.c_str(),
                                                       iter->second.lineNumber,
                                                       iter->second.numBytes);
        }
    }

    m_records.clear();
#endif

    m_initialized = false;
}
    
} // namespace Qi

