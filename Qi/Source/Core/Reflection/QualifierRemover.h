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
/// a const int & and return an int. Similar with const float ->
/// float.
///

namespace Qi
{
    // Strip all qualifiers and get down to just the base type.

    template<class T>
    struct QualifierRemover
    {
		typedef typename std::decay<T>::type type;
    };

} // namespace Qi



