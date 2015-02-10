//
//  Defines.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Defines to use for the project. All defition are platform independent.
///

#include <stdint.h>

#ifdef DEBUG
    #define QI_DEBUG
#else
    #define QI_RELEASE
#endif

typedef uint32_t uint32;
