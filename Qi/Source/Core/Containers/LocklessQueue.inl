//
//  LocklessQueue.inl
//  Qi Game Engine
//
//  Created by Cody White on 3/18/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <thread>
#include <assert.h>

namespace Qi
{

template<class T>
LocklessQueue<T>::LocklessQueue() :
    m_writeIndex(0),
    m_readIndex(0),
    m_maxReadIndex(0),
    m_allocatedSize(0)
{
}

template<class T>
LocklessQueue<T>::~LocklessQueue()
{
    Clear();
    m_allocatedSize = 0;
}

template<class T>
inline void LocklessQueue<T>::Init(uint32 size)
{
    assert(m_allocatedSize == 0);
    
    m_queue.Resize(size);
    m_allocatedSize = size;
}

template<class T>
inline uint32 LocklessQueue<T>::GetAllocatedSize() const
{
    return m_allocatedSize;
}

template<class T>
inline uint32 LocklessQueue<T>::GetSize() const
{
    assert(m_allocatedSize > 0);
    
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
        return (m_allocatedSize + currentWriteIndex - currentReadIndex);
    }
}

template<class T>
bool LocklessQueue<T>::Push(const T& element)
{
    assert(m_allocatedSize > 0);
    
    uint32 currentWriteIndex;
    
    do
    {
        if (GetSize() == m_allocatedSize)
        {
            // The queue is full.
            return false;
        }
        
        currentWriteIndex = m_writeIndex;
        
    } while (!std::atomic_compare_exchange_weak(&m_writeIndex, &currentWriteIndex, currentWriteIndex + 1));
    
    // We know now that this index is reserved for us. Use it to save the data.
    m_queue(CountToIndex(currentWriteIndex)) = element;
    
    // Update the maximum read index after saving the data.
    while (!std::atomic_compare_exchange_weak(&m_maxReadIndex, &currentWriteIndex, currentWriteIndex + 1))
    {
        // Yield here in case there are more software threads than hardware processors.
        std::this_thread::yield();
    }
    
    return true;
}

template<class T>
bool LocklessQueue<T>::Pop(T& element)
{
    assert(m_allocatedSize > 0);
    
    uint32 currentReadIndex;
    
    do
    {
        currentReadIndex = m_readIndex;
        
        if (GetSize() == 0)
        {
            // The queue is empty.
            return false;
        }
        
        // Retrieve the data from the queue
        element = m_queue(CountToIndex(currentReadIndex));
        
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

template<class T>
inline void LocklessQueue<T>::Clear()
{
    assert(m_allocatedSize > 0);
    
    m_readIndex    = 0;
    m_writeIndex   = 0;
    m_maxReadIndex = 0;
}

template<class T>
inline uint32 LocklessQueue<T>::CountToIndex(uint32 index)
{
    return (index % m_allocatedSize);
}

} // namespace Qi

