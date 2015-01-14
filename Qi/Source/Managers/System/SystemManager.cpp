//
//  SystemManager.cpp
//  Qi Game Engine
//
//  Main system manager
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "SystemManager.h"
#include "../../Core/Utility/Logger.h"
#include <assert.h>

namespace Qi
{

/**
 * Default constructor.
 */
SystemManager::SystemManager()
{
}

/**
 * Destructor.
 */
SystemManager::~SystemManager()
{
}

/**
 * Initialize the Qi game engine for use. Must be called before calling 'run'.
 */
bool SystemManager::initialize()
{
    // Initialize the logging system first as all systems will use it.
    if (!Logger::getInstance().initialize())
    {
        throw std::runtime_error("FATAL: Cannot initialize logging system");
    }
    
    LOG_INFO("Logger successfully initialized");
    
    return true;
}

/**
 * Register for an update event.
 */
void SystemManager::registerUpdateHandler(const UpdateEvent &handler)
{
    m_update_handlers.push_back(handler);
}

/**
 * Run the game engine. This function will only return once the engine is shutdown.
 * Ensure that all initialization code is performed before calling this function.
 */
void SystemManager::run()
{
}

} // namespace Qi
