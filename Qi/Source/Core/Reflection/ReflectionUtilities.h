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

///
/// Simple pointer manipulation.
///
#define PTR_ADD(PTR, OFFSET) \
    ((void *)(((char *)(PTR)) + (OFFSET)))