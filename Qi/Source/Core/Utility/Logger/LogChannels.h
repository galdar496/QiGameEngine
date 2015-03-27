//
//  LogChannels.h
//  Qi Game Engine
//
//  Created by Cody White on 3/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Different logging channels that messages can be logged to.
///

namespace Qi
{
    ///
    /// Channels for logging messages to. For custom channels, start
    /// at 'kCustom' and add any necessary channels there.
    ///
    enum LogChannel
    {
        kInfo,        ///< Informational messages
        kDebug,       ///< Debug messages
        kWarning,     ///< Warnings that will not directly cause problems
        kError,       ///< Serious errors that will prevent the system from running correctly
        kCustom,      ///< Other messages
        kNumChannels, ///< Always keep this last for count reasons.
    };
};