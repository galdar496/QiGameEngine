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
#include <string>

namespace Qi
{

struct EngineConfig
{
    public:

        ///
        /// Initialize the default configuration.
        ///
        EngineConfig() :
            flushLogFile(false)
        {}

        std::string configFile; ///< Configuration file to use for configuring the engine. If this is not set, the engine will use internal defaults.
        bool flushLogFile;      ///< If true, the logfile is flushed after each write.
};

} // namespace Qi
