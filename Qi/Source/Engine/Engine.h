//
//  Engine.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Main object which controls the engine. All engine systems reside inside
/// of this class and are controlled through it. This class provides
/// and entry-point for scene updating and rendering.
///

#include "../Core/Utility/Logger.h"

namespace Qi
{

class Engine
{
    public:
    
        Engine();
        ~Engine();
    
        ///
        /// Initialize the engine for use. All subsystems will be initialized
        /// during this call. After returning, the engine is ready for use.
        /// @return Initialization success.
        ///
        bool init();
    
        ///
        /// Step the game scene forward one frame. This updates all systems/game objects
        /// as well as starts rendering.
        ///
        void step();
    
        ///
        /// Shutdown the engine. All systems will be shutdown
        /// and any current rendering will be terminated. Once this function is finished,
        /// all memory will have been deallocated and the engine will return to a
        /// pre-initialized state.
        ///
        void shutdown();
    
    private:
    
        // Purposely not implemented.
        Engine(const Engine &other);
        Engine &operator=(const Engine &other);
    
        ///
        /// Default message handler for message logging. This function prints the incoming
        /// message to the console and is only used in debug.
        /// @param message Message to print
        /// @param channel The channel that generated this message.
        ///
        void handleLogMessages(const char *message, Logger::Channel channel);
    
        bool m_initiailzed; ///< If true, the engine has been properly initialized.
};

} // namespace Qi
