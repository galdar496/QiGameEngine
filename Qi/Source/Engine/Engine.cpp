//
//  Engine.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Engine.h"
#include "../Core/Utility/Logger.h"
#include "../Core/Memory/MemoryAllocator.h"
#include "Systems/SystemBase.h"
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
    assert(!m_initiailzed);
    
    // Initialize the logging system first as all systems will use it.
    if (!Logger::getInstance().init(config.flushLogFile))
    {
        throw std::runtime_error("FATAL: Cannot initialize logging system");
    }
    
    #ifdef QI_DEBUG
        // Register this class as the default message handler.
        Logger::MessageEvent handler(this, &Engine::handleLogMessages);
        Logger::getInstance().registerForMessages(handler, Logger::kInfo);
        Logger::getInstance().registerForMessages(handler, Logger::kDebug);
        Logger::getInstance().registerForMessages(handler, Logger::kWarning);
        Logger::getInstance().registerForMessages(handler, Logger::kError);
    #endif
    
    // Initialize the memory allocation system after the logger but before everything else.
    if (!MemoryAllocator::getInstance().init())
    {
        return false;
    }
    
    Qi_LogInfo("-Initializing engine-");
    
    Qi_LogInfo("EngingConfig -- screen (%u x %u)", config.screen_width, config.screen_height);
    
    Qi_LogInfo("-Engine successfully initialized-");
    m_initiailzed = true;
    return m_initiailzed;
}

void Engine::step(const float dt)
{
    assert(m_initiailzed);
    
    Qi_LogInfo("Engine stepping frame forward %f seconds", dt);
    
    // Add system updates to the job queue here.
    // After the job queue is finished processing, kick of the renderer to process the
    // render job queue.
}

void Engine::shutdown()
{
    assert(m_initiailzed);
    m_initiailzed = false;
    
    Qi_LogInfo("-Shutting down the engine-");
    
    // De-initialize all systems and delete the memory for them.
    for (uint32 ii = 0; ii < m_systems.getSize(); ++ii)
    {
        Qi_LogInfo("Shutting down system %s", m_systems(ii)->getName().c_str());
        m_systems(ii)->deinit();
        delete m_systems(ii);
        m_systems(ii) = nullptr;
    }
    
    // Shutdown singleton objects. Be sure to always shutdown the logger last.
    MemoryAllocator::getInstance().deinit();
    Logger::getInstance().deinit();
}

void Engine::addSystem(SystemBase *system)
{
    assert(m_initiailzed);
    
    Qi_LogInfo("Adding system %s to the engine", system->getName().c_str());
    
    m_systems.pushBack(system);
}

#ifdef QI_DEBUG
void Engine::handleLogMessages(const char *message, Logger::Channel channel)
{
    std::cout << message << std::endl;
}
#endif

} // namespace Qi

