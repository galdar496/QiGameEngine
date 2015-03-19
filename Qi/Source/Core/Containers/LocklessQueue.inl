//
//  LocklessQueue.inl
//  Qi Game Engine
//
//  Created by Cody White on 3/18/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <thread>
#include <assert.h>
#include "../Utility/MathUtilities.h"

namespace Qi
{

template<class T, uint32 QUEUE_SIZE>
LocklessQueue<T, QUEUE_SIZE>::LocklessQueue() :
    m_writeIndex(0),
    m_readIndex(0),
    m_maxReadIndex(0)
{
    // Make sure that this queue is actually power-of-two.
    assert(IsPowerOf2(QUEUE_SIZE));
}

template<class T, uint32 QUEUE_SIZE>
LocklessQueue<T, QUEUE_SIZE>::~LocklessQueue()
{
}

template<class T, uint32 QUEUE_SIZE>
inline uint32 LocklessQueue<T, QUEUE_SIZE>::GetSize() const
{
    uint32 currentWriteIndex = m_writeIndex;
    uint32 currentReadIndex  = m_readIndex;
    
    // Case where the queue start/end doesn't yet wrap around to the beginning.
    if (currentWriteIndex >= currentReadIndex)
    {
        return (currentWriteIndex - currentReadIndex);
    }
    // Queue has wrapped around to the beginning.
    else
    {
        return (QUEUE_SIZE + currentWriteIndex - currentReadIndex);
    }
}

template<class T, uint32 QUEUE_SIZE>
bool LocklessQueue<T, QUEUE_SIZE>::Push(const T& element)
{
    uint32 currentWriteIndex;
    
    do
    {
        if (GetSize() == QUEUE_SIZE)
        {
            // The queue is full.
            return false;
        }
        
        currentWriteIndex = m_writeIndex;
        
    } while (!std::atomic_compare_exchange_weak(&m_writeIndex, &currentWriteIndex, currentWriteIndex + 1));
    
    // We know now that this index is reserved for us. Use it to save the data.
    m_queue[CountToIndex(currentWriteIndex)] = element;
    
    // Update the maximum read index after saving the data.
    while (!std::atomic_compare_exchange_weak(&m_maxReadIndex, &currentWriteIndex, currentWriteIndex + 1))
    {
        // Yield here in case there are more software threads than hardware processors.
        std::this_thread::yield();
    }
    
    return true;
}

template<class T, uint32 QUEUE_SIZE>
bool LocklessQueue<T, QUEUE_SIZE>::Pop(T& element)
{
    uint32 currentReadIndex;
    
    do
    {
        currentReadIndex    = m_readIndex;
        
        if (GetSize() == 0)
        {
            // The queue is empty.
            return false;
        }
        
        // Retrieve the data from the queue
        element = m_queue[CountToIndex(currentReadIndex)];
        
        // Try to perfrom now the compare-and-exchange operation on the read index. If we succeed
        // element already contains what m_readIndex pointed to before we
        // increased it.
        if (std::atomic_compare_exchange_weak(&m_readIndex, &currentReadIndex, currentReadIndex + 1))
        {
            // Got here. The value was retrieved from the queue.
            return true;
        }
        
        // The read has failed. A different thread must have read the element stored at
        // CountToIndex(currentReadIndex) before we could perform the compare-and-exchange
        // operation.
        
    } while(1); // Keep looping and try to read again.
    
    // Something went wrong. It shouldn't be possible to reach here.
    assert(0 && "LockelssQueue::Pop() shouldn't ever get here");
    return false;
}

template<class T, uint32 QUEUE_SIZE>
inline void LocklessQueue<T, QUEUE_SIZE>::Clear()
{
    m_readIndex    = 0;
    m_writeIndex   = 0;
    m_maxReadIndex = 0;
}

template<class T, uint32 QUEUE_SIZE>
inline uint32 LocklessQueue<T, QUEUE_SIZE>::CountToIndex(uint32 index)
{
    // Because QUEUE_SIZE must be power of two, we can use & instead
    // of % for a modulus.
    return (index & (QUEUE_SIZE - 1));
}

} // namespace Qi

