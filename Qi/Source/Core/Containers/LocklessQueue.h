//
//  LocklessQueue.h
//  Qi Game Engine
//
//  Created by Cody White on 3/18/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Implement a lockless queue. This operates very similar to a normal queue except that
/// it is threadsafe without relying on locks, allowing for much more efficient
/// use. NOTE: The queue uses no dynamic memory. This simplifies the implementation, speeds
/// up it's use, and avoids the ABA problem. However, the queue is a fixed size and cannot
/// be resized after being instantiated.
/// This code is an adaptaion of http://www.codeproject.com/Articles/153898/Yet-another-implementation-of-a-lock-free-circular
/// NOTE: Queue MUST be a power-of-two size.
///

#include "../../Defines.h"
#include <atomic>

namespace Qi
{

template<class T, uint32 QUEUE_SIZE /* Must be power of two */>
class LocklessQueue
{
    public:
    
        ///
        /// Default constructor/copy constructor/destructor.
        ///
        LocklessQueue();
        ~LocklessQueue();
    
        ///
        /// Get the current element count in the queue.
        /// @return Number of elements in the queue.
        ///
        inline uint32 GetSize() const;
    
        ///
        /// Push an element onto the end of the queue.
        /// @param element Element to push onto the queue. Note that the element
        ///                is not a pointer.
        /// @return Success. If false, the queue is full.
        ///
        bool Push(const T& element);
    
        ///
        /// Pop an element off of the front of the queue.
        /// @param element A copy of the top element on the queue.
        /// @return Success. If false, the queue is empty.
        ///
        bool Pop(T& element);
    
        ///
        /// Clear the queue. Since the queue uses a static array
        /// this operation just resets the internal counters for
        /// where the queue is currently being read/written from.
        /// NOTE: This is not a threadsafe operation!!!
        ///
        inline void Clear();
    
    private:
    
        // For threadsafe reasons, make sure this class can't be
        // copied or assigned.
        LocklessQueue(const LocklessQueue &other) = delete;
        LocklessQueue & operator=(const LocklessQueue &other) = delete;
    
        ///
        /// Calculate the index in the circular array that corresponds to a particular
        /// 'index' value.
        ///
        inline uint32 CountToIndex(uint32 index);
    
        ///
        /// Internal queue object. Note that this queue is circular and will wrap
        /// around as objects are added/popped off.
        ///
        T m_queue[QUEUE_SIZE];
        std::atomic<uint32> m_writeIndex;   ///< The index in 'm_queue' where the next element will be inserted.
        std::atomic<uint32> m_readIndex;    ///< The index in 'm_queue' where the next element will be read.
    
        ///
        /// Maximum current read index for the queue. If it's not the same as 'm_writeIndex'
        /// then that means there are writes pending in the queue. The places in the queue
        /// have been reserved but the data is still not in the queue.
        ///
        std::atomic<uint32> m_maxReadIndex;
};

} // namespace Qi

#include "LocklessQueue.inl"
