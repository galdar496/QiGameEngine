//
//  QiEngineConfig.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

///
/// Engine configuration object. This can be populated and
/// passed to the engine for configuration.
///

#include "../Defines.h"

struct QiEngineConfig
{
    uint32 screen_width;  ///< Width of the screen in pixels.
    uint32 screen_height; ///< Height of the screen in pixels.
};
