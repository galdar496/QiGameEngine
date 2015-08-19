//
//  HeapAllocator.h
//  Qi Game Engine
//
//  Created by Cody White on 8/19/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Implement a heap allocator for the memory system. This allocator will simply
/// call off to the operating system's default memory allocator. It is not recommended
/// to use this allocator for anything outside of development/debugging.
///

#include "Allocator.h"
#include "../Defines.h"

namespace Qi
{

class HeapAllocator : public Allocator
{
    public:
    
		HeapAllocator() :
			m_initialized(false)
		{
		}

		virtual ~HeapAllocator()
		{
		}

		////////////////////// Functions from Qi::Allocator ////////////////////////////
		virtual Result Init(const Cinfo *info) override
		{
			QI_ASSERT(!m_initialized);

			// Nothing to initialize with a heap allocator.
			m_initialized = true;

			return Result(ReturnCode::kSuccess);
		}

		virtual void Deinit() override
		{
			QI_ASSERT(m_initialized);
			m_initialized = false;
		}

		virtual bool IsInitialized() const override
		{
			return m_initialized;
		}

		void *Allocate(size_t numBytes) const override
		{
			QI_ASSERT(m_initialized);

			void *memory = ::operator new(numBytes);
			return memory;
		}

		virtual void Deallocate(void *address) const override
		{
			QI_ASSERT(m_initialized);

			if (address != nullptr)
			{
				::operator delete(address);
				address = nullptr;
			}
		}
		////////////////////////////////////////////////////////////////////////////////

	private:

		bool m_initialized; ///< If true, this allocator is initialized and ready to use.
};

} // namespace Qi

#include "MemorySystem.inl"