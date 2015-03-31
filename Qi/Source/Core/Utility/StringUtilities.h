//
//  StringUtilities.h
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Simplle string utility functions which can be used across the engine.
///

#include "../../Defines.h"
#include <string>

namespace Qi
{

inline uint32 StringHash(const std::string &string)
{
    // This code is a slight modification of the FNV hash found here:
    // http://www.isthe.com/chongo/tech/comp/fnv/index.html#public%5Fdomain
    
    uint32 hash = 0;
    for (size_t ii = 0; ii < string.length(); ++ii)
    {
        hash *= 37;
        hash += string[ii];
    }
    
    return hash;
}

} // namespace Qi

