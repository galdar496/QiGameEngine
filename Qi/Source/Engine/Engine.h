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
#include "EngineConfig.h"

#include <vector>

namespace Qi
{

// Forward declarations.
class SystemBase;

class Engine
{
    public:
    
        Engine();
        ~Engine();
    
        ///
        /// Initialize the engine for use. All subsystems will be initialized
        /// during this call. After returning, the engine is ready for use.
        /// @param config Configuration object which is used to setup the engine.
        /// @return Initialization success.
        ///
        bool init(const EngineConfig &config);
    
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
    
        ///
        /// Add a system to the manager. Any system added this way will be managed by the
        /// Engine, including the System's memory.
        /// @param system Already initialized system to add to the engine.
        ///
        void addSystem(SystemBase *system);
    
    private:
    
        // Purposely not implemented.
        Engine(const Engine &other) = delete;
        Engine &operator=(const Engine &other) = delete;
    
        ///
        /// Default message handler for message logging. This function prints the incoming
        /// message to the console and is only used in debug.
        /// @param message Message to print
        /// @param channel The channel that generated this message.
        ///
        void handleLogMessages(const char *message, Logger::Channel channel);
    
        bool m_initiailzed; ///< If true, the engine has been properly initialized.
    
        // TODO: change this from a vector to a custom container once it's ready.
        std::vector<SystemBase *> m_systems; ///< All systems managed by the engine.
};

} // namespace Qi
