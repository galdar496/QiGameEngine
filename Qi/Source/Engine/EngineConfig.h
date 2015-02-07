//
//  EngineConfig.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Engine configuration object. This can be populated and
/// passed to the engine for configuration.
///

#include "../Defines.h"

namespace Qi
{

struct EngineConfig
{
    ///
    /// Initialize the default configuration.
    ///
    EngineConfig() :
        screen_width(800),
        screen_height(800),
        flushLogFile(false)
    {}
    
    uint32 screen_width;  ///< Width of the screen in pixels.
    uint32 screen_height; ///< Height of the screen in pixels.
    
    bool flushLogFile; ///< If true, the logfile is flushed after each write.
};

} // namespace Qi
