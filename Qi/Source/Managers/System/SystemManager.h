//
//  SystemManager.h
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "../ManagerBase.h"
#include "../../ThirdParty/FastDelegate.h"
#include "../../Core/Utility/Logger.h"
#include "../../Core/Utility/Timer.h"
#include <vector>

///
/// Main system manager that controls the Qi Game Engine. This manager
/// has references to every other manager in the engine and is the main
/// access point to the outside world. Note that the game world
/// will not run until the function run() has been called.
///

namespace Qi
{

class SystemManager : public ManagerBase
{
    public:
    
        ///
        /// Event to register for system updates. The elapsed time (in seconds) since the previous update
        /// is passed with the event.
        ///
        typedef FastDelegate <void (const float)> UpdateEvent;
    
        SystemManager();
        ~SystemManager();
    
        ///////// ManagerBase overrides /////////////
    
        ///
        /// Initialize the engine for use.
        ///
        virtual bool initialize() final;
    
        ///
        /// Call this to shutdown the entire engine. All managers will be shutdown from here
        /// and all memory will be cleaned up. If the engine is running, it will be stopped
        /// before taking down the rest of the engine.
        ///
        virtual void shutdown() final;
    
        ///
        /// Update all game systems.
        ///
        virtual void update(const float dt) final;
    
        /////////////////////////////////////////////
    
        ///
        /// Register for an update event.
        /// @param handler A message handler with signature 'UpdateEvent'
        ///
        void registerUpdateHandler(const UpdateEvent &handler);
    
        ///
        /// Run the game engine. This function will only return once the engine is shutdown.
        /// Ensure that all initialization code is performed before calling this function.
        ///
        void run();
    
    private:
    
        /// Purposely not implemented.
        SystemManager(const SystemManager &other);
        SystemManager &operator=(const SystemManager &other);
    
        ///
        /// Default message handler for message logging. This function prints the incoming
        /// message to the console and is only used in debug.
        /// @param message Message to print
        /// @param channel The channel that generated this message.
        ///
        void handleLogMessages(const char *message, Logger::Channel channel);
    
        ///
        /// Populate the internal configuration object.
        /// @return True if the config file could be read.
        ///
        bool readConfigFile(const std::string &config_file);
    
        ///
        /// Engine configuration.
        ///
        struct Config
        {
            int screen_width;  ///< With of the screen (in pixels).
            int screen_height; ///< Height of the screen (in pixels).
        };
    
        Config m_engineConfig;
        std::vector<UpdateEvent> m_update_handlers; ///< Update event handlers. Called during system updating.
    
        bool m_running; ///< If true, the engine is currently running.
    
        Timer m_timer; ///< Timer object used to calculate the time since the last frame.
};

} // namespace Qi
