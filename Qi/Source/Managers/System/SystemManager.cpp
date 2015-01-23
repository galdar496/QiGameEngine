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
#include "../../ThirdParty/tinyxml2.h"
#include <assert.h>
#include <iostream>

namespace Qi
{

/**
 * Default constructor.
 */
SystemManager::SystemManager() :
    m_running(false)
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
bool SystemManager::initialize(const std::string &configFile)
{
    // Initialize the logging system first as all systems will use it.
    if (!Logger::getInstance().initialize())
    {
        throw std::runtime_error("FATAL: Cannot initialize logging system");
    }
    Qi_LogInfo("Logger successfully initialized");
    
    #if DEBUG
        // Register this class as the default message handler.
        Logger::MessageEvent handler(this, &SystemManager::handleLogMessages);
        Logger::getInstance().registerForMessages(handler, Logger::kInfo);
        Logger::getInstance().registerForMessages(handler, Logger::kDebug);
        Logger::getInstance().registerForMessages(handler, Logger::kWarning);
        Logger::getInstance().registerForMessages(handler, Logger::kError);
    #endif
    
    // Populate the configuration object before setting up the rest of the managers.
    if (!readConfigFile(configFile))
    {
        return false;
    }
    
    m_running = false;
    
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
    m_running = true;
    
    m_timer.start();
    while(m_running)
    {
        update();
    }
}

/**
 * Shutdown the engine and cleanup everything.
 */
void SystemManager::shutdown()
{
    m_running = false;
}

/**
 * Default message handler for log messages. This funtion prints the
 * incoming message to the console and is only used in debug.
 */
void SystemManager::handleLogMessages(const char *message, Logger::Channel channel)
{
    std::cout << message << std::endl;
}

/**
 * Populate the internal configuration object.
 * @return False if the config file could not be read.
 */
bool SystemManager::readConfigFile(const std::string &config_file)
{
    tinyxml2::XMLDocument file;
    const tinyxml2::XMLElement *root_config_node = NULL;
    if (file.LoadFile(config_file.c_str()) != tinyxml2::XML_SUCCESS)
    {
        Qi_LogError("FATAL: Unable to load engine configuration file %s", config_file.c_str());
        return false;
    }
    Qi_LogInfo("Opened configuration file %s", config_file.c_str());
    
    return true;
}

/**
 * Update all game systems.
 */
void SystemManager::update()
{
    float dt = m_timer.dt();
    for (size_t ii = 0; ii < m_update_handlers.size(); ++ii)
    {
        m_update_handlers[ii](dt);
    }
}

} // namespace Qi
