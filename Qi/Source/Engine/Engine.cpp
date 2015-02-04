//
//  Engine.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Engine.h"
#include "../Defines.h"
#include <iostream>
#include <assert.h>

namespace Qi
{

Engine::Engine() :
    m_initiailzed(false)
{
}

Engine::~Engine()
{
}

bool Engine::init(const EngineConfig &config)
{
    // Initialize the logging system first as all systems will use it.
    if (!Logger::getInstance().initialize())
    {
        throw std::runtime_error("FATAL: Cannot initialize logging system");
    }
    
    #if DEBUG
        // Register this class as the default message handler.
        Logger::MessageEvent handler(this, &Engine::handleLogMessages);
        Logger::getInstance().registerForMessages(handler, Logger::kInfo);
        Logger::getInstance().registerForMessages(handler, Logger::kDebug);
        Logger::getInstance().registerForMessages(handler, Logger::kWarning);
        Logger::getInstance().registerForMessages(handler, Logger::kError);
    #endif
    
    Qi_LogInfo("-Initializing engine-");
    
    Qi_LogInfo("EngingConfig -- screen (%u x %u)", config.screen_width, config.screen_height);
    
    Qi_LogInfo("-Engine successfully initialized-");
    m_initiailzed = true;
    return m_initiailzed;
}

void Engine::step()
{
    assert(m_initiailzed);
}

void Engine::shutdown()
{
    assert(m_initiailzed);
    m_initiailzed = false;
    
    Qi_LogInfo("-Shutting down the engine-");
}

void Engine::handleLogMessages(const char *message, Logger::Channel channel)
{
    std::cout << message << std::endl;
}

} // namespace Qi

