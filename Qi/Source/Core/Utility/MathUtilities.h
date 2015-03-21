//
//  MathUtilities.h
//  Qi Game Engine
//
//  Created by Cody White on 3/18/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Simple math utility functions which can be used across the engine.
///

namespace  Qi
{

///
/// Determine if the passed-in value is a power of two.
///
/// @param x Value to check to be power of two.
/// @return If true, this value is a power of two.
///
inline bool IsPowerOf2(int x)
{
    return (x > 1) && ((x & (x - 1)) == 0);
}

} // namespace Qi.
