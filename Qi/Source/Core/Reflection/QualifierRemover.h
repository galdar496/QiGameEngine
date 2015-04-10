//
//  QualifierRemover.h
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Remove any qualifiers from a type. For example, take
/// an int * and return an int. Similar with const float ->
/// float.
///

namespace Qi
{
    // Define a series of structs that can strip out all C++ qualifiers
    // and get down to just the type.

    template<class T>
    struct QualifierRemover
    {
        typedef T type;
    };
    
    template<class T>
    struct QualifierRemover<const T>
    {
        typedef typename QualifierRemover<T>::type type;
    };
    
    template<class T>
    struct QualifierRemover<const T &>
    {
        typedef typename QualifierRemover<T>::type type;
    };
    
    template<class T>
    struct QualifierRemover<T &&>
    {
        typedef typename QualifierRemover<T>::type type;
    };
    
    // Preserve pointer types.
    template<class T>
    struct QualifierRemover<const T *>
    {
        typedef typename QualifierRemover<T *>::type type;
    };
    
} // namespace Qi



