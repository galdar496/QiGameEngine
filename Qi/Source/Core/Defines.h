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

#include <assert.h>

#if defined(DEBUG) || defined(_DEBUG)
    #define QI_DEBUG
    #define QI_ASSERT(x) assert(x)
#else
    #define QI_RELEASE
    #define QI_ASSERT(x)
#endif

#if defined(_MSC_VER)
    #define QI_WINDOWS
#endif

#if defined(QI_WINDOWS)
    #define QI_ALIGN(x) __declspec(align(x))
#else
    #define QI_ALIGN(x) __attribute__ ((aligned(x)))
#endif

