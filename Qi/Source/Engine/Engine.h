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

#include "EngineConfig.h"
#include "../Defines.h"

#ifdef QI_DEBUG
    #include "../Core/Utility/Logger.h"
#endif

#include "../Core/Containers/Array.h"

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
        bool Init(const EngineConfig &config);
    
        ///
        /// Step the game scene forward one frame. This updates all systems/game objects
        /// as well as starts rendering.
        /// @param dt Delta time. Elapsed time from the previous call to "step()".
        ///
        void Step(const float dt);
    
        ///
        /// Shutdown the engine. All systems will be shutdown
        /// and any current rendering will be terminated. Once this function is finished,
        /// all memory will have been deallocated and the engine will return to a
        /// pre-initialized state.
        ///
        void Shutdown();
    
        ///
        /// Add a system to the manager. Any system added this way will be managed by the
        /// Engine, including the System's memory.
        /// @param system Already initialized system to add to the engine.
        ///
        void AddSystem(SystemBase *system);
    
    private:
    
        // Purposely not implemented.
        Engine(const Engine &other) = delete;
        Engine &operator=(const Engine &other) = delete;
    
    #ifdef QI_DEBUG
        ///
        /// Default message handler for message logging. This function prints the incoming
        /// message to the console and is only used in debug.
        /// @param message Message to print
        /// @param channel The channel that generated this message.
        ///
        void HandleLogMessages(const char *message, Logger::Channel channel);
    #endif
    
        bool m_initiailzed; ///< If true, the engine has been properly initialized.
    
        Array<SystemBase *> m_systems; ///< All systems managed by the engine.
};

} // namespace Qi
