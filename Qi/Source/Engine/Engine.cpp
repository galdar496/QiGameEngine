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

bool Engine::Init(const EngineConfig &config)
{
    assert(!m_initiailzed);
    
    // Initialize the logging system first as all systems will use it.
    if (!Logger::GetInstance().Init(config.flushLogFile))
    {
        throw std::runtime_error("FATAL: Cannot initialize logging system");
    }
    
    #ifdef QI_DEBUG
        // Register this class as the default message handler.
        Logger::MessageEvent handler(this, &Engine::HandleLogMessages);
        Logger::GetInstance().RegisterForMessages(handler, Logger::kInfo);
        Logger::GetInstance().RegisterForMessages(handler, Logger::kDebug);
        Logger::GetInstance().RegisterForMessages(handler, Logger::kWarning);
        Logger::GetInstance().RegisterForMessages(handler, Logger::kError);
    #endif
    
    // Initialize the memory allocation system after the logger but before everything else.
    if (!MemoryAllocator::GetInstance().Init())
    {
        return false;
    }
    
    Qi_LogInfo("-Initializing engine-");
    
    Qi_LogInfo("EngingConfig -- screen (%u x %u)", config.screenWidth, config.screenHeight);
    
    Qi_LogInfo("-Engine successfully initialized-");
    m_initiailzed = true;
    return m_initiailzed;
}

void Engine::Step(const float dt)
{
    assert(m_initiailzed);
    
    Qi_LogInfo("Engine stepping frame forward %f seconds", dt);
    
    // Add system updates to the job queue here.
    // After the job queue is finished processing, kick of the renderer to process the
    // render job queue.
}

void Engine::Shutdown()
{
    assert(m_initiailzed);
    m_initiailzed = false;
    
    Qi_LogInfo("-Shutting down the engine-");
    
    // De-initialize all systems and delete the memory for them.
    for (uint32 ii = 0; ii < m_systems.GetSize(); ++ii)
    {
        Qi_LogInfo("Shutting down system %s", m_systems(ii)->GetName().c_str());
        m_systems(ii)->Deinit();
        delete m_systems(ii);
        m_systems(ii) = nullptr;
    }
    
    // Shutdown singleton objects. Be sure to always shutdown the logger last.
    MemoryAllocator::GetInstance().Deinit();
    Logger::GetInstance().Deinit();
}

void Engine::AddSystem(SystemBase *system)
{
    assert(m_initiailzed);
    
    Qi_LogInfo("Adding system %s to the engine", system->GetName().c_str());
    
    m_systems.PushBack(system);
}

#ifdef QI_DEBUG
void Engine::HandleLogMessages(const char *message, Logger::Channel channel)
{
    std::cout << message << std::endl;
}
#endif

} // namespace Qi

