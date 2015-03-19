//
//  Array.inl
//  Qi Game Engine
//
//  Created by Cody White on 3/1/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Memory/MemoryAllocator.h"

namespace Qi
{

template<class T>
Array<T>::Array() :
    m_elements(nullptr),
    m_allocatedSize(0),
    m_back(0),
    m_count(0)
{
}

template<class T>
Array<T>::Array(const Array &other)
{
    m_count = other.m_count;
    m_back = other.m_back;
    m_allocatedSize = other.m_allocatedSize;
    
    if (m_allocatedSize)
    {
        m_elements = Qi_AllocateMemoryArray(T, m_allocatedSize);
        std::memcpy(m_elements, other.m_elements, m_count * sizeof(T));
    }
    else
    {
        m_elements = nullptr;
    }
    
}

template<class T>
Array<T>::~Array()
{
    Clear();
}

template<class T>
Array<T> & Array<T>::operator=(const Array<T> &other)
{
    if (this != &other)
    {
        m_count = other.m_count;
        m_back = other.m_back;
        m_allocatedSize = other.m_allocatedSize;
        
        m_elements = Qi_AllocateMemoryArray(T, m_allocatedSize);
        std::memcpy(m_elements, other.m_elements, m_count * sizeof(T));
    }
    
    return *this;
}

template<class T>
bool Array<T>::PushBack(const T &value)
{
    if (m_count >= m_allocatedSize)
    {
        Reallocate();
    }
    
    void *buffer = (void *)&m_elements[m_back];
    new (buffer) T(value);
    ++m_count;
    ++m_back;
    
    return true;
}

template<class T>
void Array<T>::Resize(uint32 num_elements)
{
    if (m_count > 0)
    {
        Clear();
    }
    
    m_elements = Qi_AllocateMemoryArray(T, num_elements);
    m_count = num_elements;
    m_allocatedSize = num_elements;
}

template<class T>
uint32 Array<T>::GetSize() const
{
    return m_count;
}

template<class T>
uint32 Array<T>::GetAllocateSize() const
{
    return m_allocatedSize;
}

template<class T>
void Array<T>::Clear()
{
    if (m_allocatedSize)
    {
        m_count = 0;
        m_back = 0;
        m_allocatedSize = 0;
        Qi_FreeMemory(m_elements);
        m_elements = nullptr;
    }
}

template<class T>
T &Array<T>::operator()(int index) const
{
    assert(index < m_count);
    return m_elements[index];
}

template<class T>
void Array<T>::Reallocate()
{
    // Create a new array which is double the size of the previous one.
    uint32 new_size = (m_allocatedSize != 0) ? m_allocatedSize * 2 : m_DEFAULT_ARRAY_SIZE;
    T *tmp_array = Qi_AllocateMemoryArray(T, new_size);
    
    // Copy the old data from the previous array into the new one and free
    // the old one.
    memcpy((void *)tmp_array, (void *)m_elements, m_allocatedSize * sizeof(T));
    Qi_FreeMemory(m_elements);
    m_elements = tmp_array;
    m_allocatedSize = new_size;
}

} // namespace Qi
