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
        bool initialize();
    
        /**
          * Register for an update event.
          */
        void registerUpdateHandler(const UpdateEvent &handler);
    
        /**
          * Run the game engine. This function will only return once the engine is shutdown.
          * Ensure that all initialization code is performed before calling this function.
          */
        void run();
    
    private:
    
        // Purposely not implemented.
        SystemManager(const SystemManager &other);
        SystemManager &operator=(const SystemManager &other);
    
        /**
          * Update all game systems.
          */
        void update();
    
        std::vector<UpdateEvent> m_update_handlers; // Update event handlers. Called during system updating.
};

} // namespace Qi
