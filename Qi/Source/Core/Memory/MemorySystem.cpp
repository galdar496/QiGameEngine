//
//  MemorySystem.cpp
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "MemorySystem.h"
#include "../Utility/Logger/Logger.h"
#include "../Defines.h"

namespace Qi
{

MemorySystem::MemorySystem() :
    m_initialized(false),
	m_allocator(nullptr)
{
}

MemorySystem::~MemorySystem()
{
}

MemorySystem &MemorySystem::GetInstance()
{
	static MemorySystem allocator;
    return allocator;
}
    
Result MemorySystem::Init(Allocator *allocator)
{
    QI_ASSERT(!m_initialized);
	QI_ASSERT(allocator != nullptr);
	QI_ASSERT(allocator->IsInitialized());

	m_allocator = allocator;

    m_initialized = true;
    return Result(ReturnCode::kSuccess);
}
    
void MemorySystem::Deinit()
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

	// Make sure the allocator is cleaned up as well.
	m_allocator->Deinit();
	delete m_allocator;
	m_allocator = nullptr;

    m_initialized = false;
}

Allocator *MemorySystem::GetAllocator() const
{
	QI_ASSERT(m_initialized);
	return m_allocator;
}
    
} // namespace Qi

