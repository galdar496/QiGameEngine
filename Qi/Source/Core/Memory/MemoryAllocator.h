//
//  MemoryAllocator.h
//  Qi Game Engine
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Memory allocator for the Qi Game Engine. This allocator
/// will handle all memory allocation/deallocation requests
/// and keep track of how much memory has been allocated
/// (to detect memory leaks). This class is implemented
/// as a singleton.
///

#include <string>
#include <unordered_map>

#define Qi_AllocateMemory(type) Qi::MemoryAllocator::GetInstance().Allocate<type>(sizeof(type), __FILE__, __LINE__)
#define Qi_AllocateMemoryArray(type, count) Qi::MemoryAllocator::GetInstance().Allocate<type>(sizeof(type) * count, __FILE__, __LINE__)
#define Qi_FreeMemory(address) Qi::MemoryAllocator::GetInstance().Free(address)

namespace Qi
{

class MemoryAllocator
{
    public:
    
        ///
        /// Instance accessor to get to the singleton object.
        /// @return Static instance of MemoryAllocator.
        ///
        static MemoryAllocator &GetInstance();
    
        ///
        /// Initialize the memory allocator for use.
        /// @return Initialization success.
        ///
        bool Init();
    
        ///
        /// Deinitialize the memory allocator. Any still-allocated
        /// memory will be reported.
        ///
        void Deinit();
    
        ///
        /// Allocate a user-defined amount of memory.
        /// @param numBytes Number of bytes to allocate.
        /// @param filename Filename that this allocation came from.
        /// @param lineNumber Line number where this allocation took place.
        /// @return Pointer to an allocated buffer.
        ///
        template<class T>
        T *Allocate(unsigned long long numBytes, const char *filename = nullptr, int lineNumber = 0);
    
        ///
        /// Frees an allocated buffer.
        /// @param address Address of the buffer to free. If
        ///        null, this function will not do anything.
        ///
        template<class T>
        void Free(T *address);
    
    private:
    
        // Do not implement.
        MemoryAllocator(const MemoryAllocator &other) = delete;
        MemoryAllocator & operator=(const MemoryAllocator &other) = delete;
    
        MemoryAllocator();
        ~MemoryAllocator();
    
        bool m_initialized; ///< If true, the memory allocated is initialized.
    
        ///
        /// Record stored for each allocation (debug only).
        ///
        struct MemoryRecord
        {
            unsigned long long numBytes; ///< Number of bytes allocated.
            
            int lineNumber;      ///< Line number this allocation came from.
            std::string filename; ///< Filename that allocated this memory.
        };
    
        typedef std::unordered_map<void *, MemoryRecord> Record;
        Record m_records; ///< All current allocations in the system (debug only). Stored by address.
    
    
};

} // namespace Qi

#include "MemoryAllocator.inl"