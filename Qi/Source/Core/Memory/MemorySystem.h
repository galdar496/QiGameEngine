//
//  MemorySystem.h
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Memory management system for the Qi Game Engine. This system
/// will handle all memory allocation/deallocation requests
/// and keep track of how much memory has been allocated
/// (to detect memory leaks). This class is implemented
/// as a singleton.
///

#include "Allocator.h"
#include "../BaseTypes.h"
#include <string>
#include <unordered_map>

#define Qi_AllocateMemory(type) Qi::MemorySystem::GetInstance().Allocate<type>(__FILE__, __LINE__)
#define Qi_AllocateMemoryArray(type, count) Qi::MemorySystem::GetInstance().AllocateArray<type>(count, __FILE__, __LINE__)
#define Qi_FreeMemory(address) Qi::MemorySystem::GetInstance().Free(address)
#define Qi_FreeMemoryArray(address) Qi::MemorySystem::GetInstance().FreeArray(address)

namespace Qi
{

class MemorySystem
{
    public:
    
        ///
        /// Instance accessor to get to the singleton object.
        ///
        /// @return Static instance of MemorySystem.
        ///
		static MemorySystem &GetInstance();
    
        ///
        /// Initialize the memory system for use.
        ///
		/// @param allocator Allocator to install and use in this memory system. After
		///                  this call, the memory system owns the Allocator instance.
        /// @return Initialization success.
        ///
        Result Init(Allocator *allocator);
    
        ///
        /// Deinitialize the memory system. Any still-allocated
        /// memory will be reported.
        ///
        void Deinit();
    
        ///
        /// Allocate an instance for a specific type.
        ///
        /// @param filename Filename that this allocation came from.
        /// @param lineNumber Line number where this allocation took place.
        /// @return Pointer to an allocated buffer.
        ///
        template<class T>
        T *Allocate(const char *filename = nullptr, int lineNumber = 0);

		///
		/// Allocate an array of type T.
		///
		/// @param arraySize Number of elements to allocate with the array.
		/// @param filename Filename that this allocation came from.
		/// @param lineNumber Line number where this allocation took place. 
		///
		template<class T>
		T *AllocateArray(uint32 arraySize, const char *filename = nullptr, int lineNumber = 0);
    
        ///
        /// Frees an allocated type.
        ///
        /// @param address Address of the buffer to free. If
        ///        null, this function will not do anything.
        ///
        template<class T>
        void Free(T *address);

		///
		/// Frees an allocated array buffer.
		///
		/// @param address Address of the buffer to free. If
		///        null, this function will not do anything.
		///
		template<class T>
		void FreeArray(T *address);

		///
		/// Get the allocator currently installed into the memory system.
		///
		/// @return Allocator instance.
		///
		Allocator *GetAllocator() const;
    
    private:
    
        // Do not implement.
		MemorySystem(const MemorySystem &other) = delete;
		MemorySystem & operator=(const MemorySystem &other) = delete;
    
		MemorySystem();
		~MemorySystem();

        bool m_initialized; ///< If true, the memory system is initialized.
    
        ///
        /// Record stored for each allocation (debug only).
        ///
        struct MemoryRecord
        {
            unsigned long long numBytes; ///< Number of bytes allocated.
            
			std::string filename; ///< Filename that allocated this memory.
            int lineNumber;       ///< Line number this allocation came from.
			bool isArray;         ///< If true, the allocation is an array type.
        };
    
        typedef std::unordered_map<void *, MemoryRecord> Record;
        Record m_records; ///< All current allocations in the system (debug only). Stored by address.

		Allocator *m_allocator; ///< Memory allocator installed into this memory system. All memory allocations/
		                        ///< deallocations will go through this allocator.
};

} // namespace Qi

#include "MemorySystem.inl"