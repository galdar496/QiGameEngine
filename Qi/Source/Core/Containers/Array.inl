//
//  Array.cpp
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
    m_allocated_size(0),
    m_back(0),
    m_count(0)
{
}

template<class T>
Array<T>::Array(const Array &other)
{
    m_count = other.m_count;
    m_back = other.m_back;
    m_allocated_size = other.m_allocated_size;
    
    if (m_allocated_size)
    {
        m_elements = Qi_AllocateMemoryArray(T, m_allocated_size);
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
    clear();
}

template<class T>
Array<T> & Array<T>::operator=(const Array<T> &other)
{
    if (this != &other)
    {
        m_count = other.m_count;
        m_back = other.m_back;
        m_allocated_size = other.m_allocated_size;
        
        m_elements = Qi_AllocateMemoryArray(T, m_allocated_size);
        std::memcpy(m_elements, other.m_elements, m_count * sizeof(T));
    }
    
    return *this;
}

template<class T>
bool Array<T>::pushBack(const T &value)
{
    if (m_count >= m_allocated_size)
    {
        reallocate();
    }
    
    void *buffer = (void *)&m_elements[m_back];
    new (buffer) T(value);
    ++m_count;
    ++m_back;
    
    return true;
}

template<class T>
void Array<T>::resize(uint32 num_elements)
{
    if (m_count > 0)
    {
        clear();
    }
    
    m_elements = Qi_AllocateMemoryArray(T, num_elements);
    m_count = num_elements;
    m_allocated_size = num_elements;
}

template<class T>
uint32 Array<T>::getSize() const
{
    return m_count;
}

template<class T>
uint32 Array<T>::getAllocateSize() const
{
    return m_allocated_size;
}

template<class T>
void Array<T>::clear()
{
    if (m_allocated_size)
    {
        m_count = 0;
        m_back = 0;
        m_allocated_size = 0;
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
void Array<T>::reallocate()
{
    // Create a new array which is double the size of the previous one.
    uint32 new_size = (m_allocated_size != 0) ? m_allocated_size * 2 : m_DEFAULT_ARRAY_SIZE;
    T *tmp_array = Qi_AllocateMemoryArray(T, new_size);
    
    // Copy the old data from the previous array into the new one and free
    // the old one.
    memcpy((void *)tmp_array, (void *)m_elements, m_allocated_size * sizeof(T));
    Qi_FreeMemory(m_elements);
    m_elements = tmp_array;
    m_allocated_size = new_size;
}

} // namespace Qi
