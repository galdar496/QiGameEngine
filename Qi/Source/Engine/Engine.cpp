//
//  Engine.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Engine.h"
#include "../Core/Utility/Logger/Logger.h"
#include "../Core/Memory/MemorySystem.h"
#include "../Core/Memory/HeapAllocator.h"
#include "Systems/SystemBase.h"
#include "Systems/EntitySystem.h"
#include "Systems/Renderer/RenderingSystem.h"
#include <iostream>

namespace Qi
{

Engine::Engine() :
    m_initiailzed(false),
	m_entitySystem(nullptr),
	m_renderingSystem(nullptr)
{
}

Engine::~Engine()
{

}

Result Engine::Init(const EngineConfig &config)
{
    QI_ASSERT(!m_initiailzed);
    
    // Initialize the logging system first as all systems will use it.
    if (!Logger::GetInstance().Init(Logger::LogFileType::kHTML, config.flushLogFile).IsValid())
    {
        throw std::runtime_error("FATAL: Cannot initialize logging system");
    }
    
    #ifdef QI_DEBUG
        // Register this class as the default message handler.
        Logger::MessageEvent handler(this, &Engine::HandleLogMessages);
        Logger::GetInstance().RegisterForMessages(handler, LogChannel::kInfo);
        Logger::GetInstance().RegisterForMessages(handler, LogChannel::kDebug);
        Logger::GetInstance().RegisterForMessages(handler, LogChannel::kWarning);
        Logger::GetInstance().RegisterForMessages(handler, LogChannel::kError);
    #endif
    
	Result result(ReturnCode::kUnknownError);

	// Initialize the memory allocation system after the logger but before everything else.
	{
		// Install a heap allocator for now until more allocators are available, then make
		// the allocator type optional.
		HeapAllocator *allocator = new HeapAllocator;
		allocator->Init(nullptr);

		result = MemorySystem::GetInstance().Init(allocator);
		if (!result.IsValid())
		{
			return result;
		}
	}
    
    Qi_LogInfo("-Initializing engine-");
    Qi_LogInfo("EngingConfig -- ");
	Qi_LogInfo("\tscreen (%u x %u)", config.screenWidth, config.screenHeight);
	Qi_LogInfo("\tmax entity count: %u", config.maxWorldEntities);
    
    result = CreateInternalSystems(config);
    if (!result.IsValid())
    {
        return result;
    }
    
    Qi_LogInfo("-Engine successfully initialized-");
    m_initiailzed = true;
    result.code = ReturnCode::kSuccess;
    return result; 
}

void Engine::Step(const float dt)
{
    QI_ASSERT(m_initiailzed);
    
    //Qi_LogInfo("Engine stepping frame forward %f seconds", dt);
    
    // Add system updates to the job queue here.
    // After the job queue is finished processing, kick of the renderer to process the
    // render job queue.

	m_renderingSystem->Update(dt);
    
    
    
    // Update custom systems.
    for (uint32 ii = 0; ii < m_customSystems.GetSize(); ++ii)
    {
        m_customSystems[ii]->Update(dt);
    }
}

void Engine::Shutdown()
{
    QI_ASSERT(m_initiailzed);
    m_initiailzed = false;
    
    Qi_LogInfo("-Shutting down the engine-");
    
    // De-initialize all custom systems and delete the memory for them.
    for (uint32 ii = 0; ii < m_customSystems.GetSize(); ++ii)
    {
        Qi_LogInfo("Shutting down system %s", m_customSystems[ii]->GetName().c_str());
        m_customSystems[ii]->Deinit();
        delete m_customSystems[ii];
        m_customSystems[ii] = nullptr;
    }
    
    ShutdownInternalSystems();
    
    // Shutdown singleton objects. Be sure to always shutdown the logger last.
	MemorySystem::GetInstance().Deinit();
    Logger::GetInstance().Deinit();
}

Result Engine::CreateInternalSystems(const EngineConfig &config)
{
	// Entity system.
    {
        m_entitySystem = Qi_AllocateMemory(EntitySystem);
        Qi_LogInfo("Adding system %s", m_entitySystem->GetName().c_str());
        
        EntitySystem::EntitySystemCInfo info;
        info.maxEntities = config.maxWorldEntities;
        Result result = m_entitySystem->Init(&info);
        if (!result.IsValid())
        {
			Qi_LogError("Unable to initialize system %s", m_entitySystem->GetName().c_str());
            return result;
        }
    }

	// Windowing system.
	{
		m_renderingSystem = Qi_AllocateMemory(RenderingSystem);
		Qi_LogInfo("Adding system %s", m_renderingSystem->GetName().c_str());

		RenderingSystem::RenderingSystemCInfo info;
		info.screenWidth  = config.screenWidth;
		info.screenHeight = config.screenHeight;
		info.fullscreen   = false;
		info.windowName   = config.gameTitle;

		Result result = m_renderingSystem->Init(&info);
		if (!result.IsValid())
		{
			Qi_LogError("Unable to initialize system %s", m_renderingSystem->GetName().c_str());
			return result;
		}
	}
    
    return Result(ReturnCode::kSuccess);
}

void Engine::ShutdownInternalSystems()
{
    Qi_LogInfo("Shutting down %s", m_entitySystem->GetName().c_str());
    m_entitySystem->Deinit();
    Qi_FreeMemory(m_entitySystem);
    m_entitySystem = nullptr;

	Qi_LogInfo("Shutting down %s", m_renderingSystem->GetName().c_str());
	m_renderingSystem->Deinit();
	Qi_FreeMemory(m_renderingSystem);
	m_renderingSystem = nullptr;
}

void Engine::AddSystem(SystemBase *system)
{
    QI_ASSERT(m_initiailzed);
    
    Qi_LogInfo("Adding system %s to the engine", system->GetName().c_str());
    
    m_customSystems.PushBack(system);
}

#ifdef QI_DEBUG
void Engine::HandleLogMessages(const char *message, LogChannel channel)
{
    std::cout << message << std::endl;
}
#endif

} // namespace Qi

