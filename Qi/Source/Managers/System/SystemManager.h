//
//  SystemManager.h
//  Qi Game Engine
//
//  Main system manager
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "../../Core/Utility/FastDelegate.h"
#include "../../Core/Utility/Logger.h"
#include "../../Core/Utility/Timer.h"
#include <vector>

namespace Qi
{

class SystemManager
{
    public:
    
        // Event to register for for system updates. Passes the elapsed time since the previous update
        // event.
        typedef FastDelegate <void (const float)> UpdateEvent;
    
        /**
          * Default constructor.
          */
        SystemManager();
    
        /**
          * Destructor.
          */
        ~SystemManager();
    
        /**
          * Initialize the Qi game engine for use. Must be called before calling 'run'.
          */
        bool initialize(const std::string &configFile);
    
        /**
          * Register for an update event.
          */
        void registerUpdateHandler(const UpdateEvent &handler);
    
        /**
          * Run the game engine. This function will only return once the engine is shutdown.
          * Ensure that all initialization code is performed before calling this function.
          */
        void run();
    
        /**
          * Shutdown the engine and cleanup everything.
          */
        void shutdown();
    
    private:
    
        // Purposely not implemented.
        SystemManager(const SystemManager &other);
        SystemManager &operator=(const SystemManager &other);
    
        /**
          * Default message handler for log messages. This funtion prints the 
          * incoming message to the console and is only used in debug.
          */
        void handleLogMessages(const char *message, Logger::Channel channel);
    
        /**
          * Update all game systems.
          */
        void update();
    
        /**
          * Populate the internal configuration object. 
          * @return False if the config file could not be read.
          */
        bool readConfigFile(const std::string &config_file);
    
        /**
          * Engine configuration.
          */
        struct Config
        {
            /// Screen dimensions (in pixels).
            int screen_width;
            int screen_height;
        };
    
        Config m_engineConfig;
        std::vector<UpdateEvent> m_update_handlers; // Update event handlers. Called during system updating.
    
        bool m_running; // If true, the engine is currently running.
    
        Timer m_timer;
};

} // namespace Qi
