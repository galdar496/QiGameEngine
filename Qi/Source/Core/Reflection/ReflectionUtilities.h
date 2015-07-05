//
//  ReflectionUtilities.h
//  Qi Game Engine
//
//  Created by Cody White on 6/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Utility functions to be used by the reflection system.
///

namespace Qi
{

///
/// Simple pointer manipulation.
///
inline void *PointerOffset(const void *ptr, size_t offset)
{
    return (void *)((char *)ptr + offset);
}

} // namespace Qi
