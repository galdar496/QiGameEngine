//
//  Array.h
//  Qi Game Engine
//
//  Created by Cody White on 3/1/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Templated class which represents a congiuous allocation of elements.
/// Designed similar to the stl vector in that a set number of elements
/// are allocated up front. Once that number is passed the allocation is
/// doubled and all previous elements are copied.
///

#include "../../Defines.h"

namespace Qi
{

template<class T>
class Array
{
    public:
    
        ///
        /// Default constructor/copy constructor/destructor.
        ///
        Array();
        Array(const Array &other);
        ~Array();
        Array & operator=(const Array &other);
    
        ///
        /// Push a new value into the end of the array. If the current size of the array is too small
        /// then the size of the array will be doubled and all previous elements will be copied into
        /// the new array. NOTE: T's copy-constructor must be implemented.
        /// @return Insertion was successful.
        ///
        bool pushBack(const T &value);
    
        ///
        /// Resize the Array. If there are already elements in this array they will be lost.
        /// @param num_elements Target size for the array (in terms of element count).
        ///
        void resize(uint32 num_elements);
    
        ///
        /// Get the number of elements currently in the Array.
        /// @return Element count in the array.
        ///
        inline uint32 getSize() const;
    
        ///
        /// Get the allocated size of the array. This value is >=
        /// to the number of elements in the array.
        /// @return Allocated size of the array (in terms of elements).
        ///
        inline uint32 getAllocateSize() const;
    
        ///
        /// Clear all elements from the array. This also deallocates the underlying memory
        /// for the Array object.
        ///
        void clear();
    
        /// Operator overloads ///////////////////////
        inline T &operator()(int index) const;
    
    private:
    
        ///
        /// Reallocate the array to a new size and copy over all elements in the previous array.
        /// The previous allocation size is doubled to accomodate any new elements.
        ///
        void reallocate();
    
        T      *m_elements;      ///< Underlying array. Allocated on the first call to pushBack.
        uint32 m_back;           ///< Index into the array representing the next element to push into.
        uint32 m_count;          ///< Number of elements currently placed into the array.
        uint32 m_allocated_size; ///< Allocated size of the array.
    
        static const uint32 m_DEFAULT_ARRAY_SIZE = 20; ///< Default value to use to size the array.
};

} // namespace Qi

#include "Array.inl"