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

#include "../Core/Defines.h"
#include "../Core/BaseTypes.h"

namespace Qi
{

struct EngineConfig
{
    ///
    /// Initialize the default configuration.
    ///
    EngineConfig() :
        screenWidth(800),
        screenHeight(800),
        flushLogFile(false)
    {}
    
    uint32 screenWidth;  ///< Width of the screen in pixels.
    uint32 screenHeight; ///< Height of the screen in pixels.
    
    bool flushLogFile; ///< If true, the logfile is flushed after each write.
};

} // namespace Qi
