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
#include "WindowMessage.h"
#include "../Core/Defines.h"
#include "../Core/BaseTypes.h"

#ifdef QI_DEBUG
    #include "../Core/Utility/Logger/Logger.h"
#endif

#include "../Core/Containers/Array.h"

namespace Qi
{

// Forward declarations.
class SystemBase;
class EntitySystem;
class RenderingSystem;

class Engine
{
    public:
    
        Engine();
        ~Engine();
    
        ///
        /// Initialize the engine for use. All subsystems will be initialized
        /// during this call. After returning, the engine is ready for use.
        ///
        /// @param config Configuration object which is used to setup the engine.
        /// @returns Initialization success.
        ///
        Result Init(const EngineConfig &config);
    
        ///
        /// Step the game scene forward one frame. This updates all systems/game objects
        /// as well as starts rendering.
        ///
        /// @param dt Delta time. Elapsed time from the previous call to "step()".
        /// @return If false, the engine has been requested to shutdown.
        ///
        bool Step(const float dt);
    
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
        ///
        /// @param system Already initialized system to add to the engine.
        ///
        void AddSystem(SystemBase *system);

        ///
        /// Handle an incoming message from the windowing system. This message is usually
        /// piped through a platform-specific window. NOTE: the definition of this function 
        /// will be defined in "PLATFFORM_NAME"MessageHandler.cpp where "PLATFORMNAME" is something
        /// like Win32 or OSX.
        ///
        /// @param message WindowMessage object which contains the message contents.
        /// @return If true, the message was handled.
        ///
        bool HandleMessage(WindowMessage *message);
    
    private:
    
        // Purposely not implemented.
        Engine(const Engine &other) = delete;
        Engine &operator=(const Engine &other) = delete;
    
        ///
        /// Create the internal systems to handle various engine tasks (rendering, entities, physics, etc.).
        ///
        Result CreateInternalSystems(const EngineConfig &config);
    
        ///
        /// Shutdown any engine systems and make sure all memory is cleaned up.
        ///
        void ShutdownEngineSystems();
    
    #ifdef QI_DEBUG
        ///
        /// Default message handler for message logging. This function prints the incoming
        /// message to the console and is only used in debug.
        ///
        /// @param message Message to print
        /// @param channel The channel that generated this message.
        ///
        void HandleLogMessages(const char *message, LogChannel channel);
    #endif
    
        bool m_initiailzed;     ///< If true, the engine has been properly initialized.
        bool m_shouldShutdown;  ///< If true, the engine should shutdown on the next update tick.
    
        Array<SystemBase *> m_engineSystems; ///< All engine systems that the engine knows about (both internal and custom systems).
    
        // Internal system references created and owned by the engine. The systems all live inside of "m_engineSystems" but these
        // pointers exist for quick access to a specific system.
        EntitySystem    *m_entitySystem;
		RenderingSystem *m_renderingSystem;
};

} // namespace Qi
