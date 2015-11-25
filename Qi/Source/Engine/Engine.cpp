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
#include "Systems/SystemConfig/ConfigVariables.h"
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
    Shutdown();
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

    // Update engine systems systems.
    for (uint32 ii = 0; ii < m_engineSystems.GetSize(); ++ii)
    {
        m_engineSystems[ii]->Update(dt);
    }
}

void Engine::Shutdown()
{
    QI_ASSERT(m_initiailzed);
    m_initiailzed = false;
    
    Qi_LogInfo("-Shutting down the engine-");
    
    ShutdownEngineSystems();
    
    // Shutdown singleton objects. Be sure to always shutdown the logger last.
	MemorySystem::GetInstance().Deinit();
    Logger::GetInstance().Deinit();
}

Result Engine::CreateInternalSystems(const EngineConfig &config)
{
    Result result(ReturnCode::kSuccess);
    
    m_renderingSystem = Qi_AllocateMemory(RenderingSystem);
    m_engineSystems.PushBack(m_renderingSystem);

    m_entitySystem = Qi_AllocateMemory(EntitySystem);
    m_engineSystems.PushBack(m_entitySystem);

    // Read the config file.
    ConfigVariables configVariables;
    result = configVariables.ParseConfigFile(config.configFile);
    
    if (result.IsValid())
    {
        Qi_LogInfo("Creating base engine systems...");

        // Initialize the systems for use.
        for (uint32 ii = 0; ii < m_engineSystems.GetSize(); ++ii)
        {
            SystemBase *system = m_engineSystems[ii];

            result = system->Init(configVariables);
            if (!result.IsValid())
            {
                break;
            }

            Qi_LogInfo("\t%s successfully created", system->GetName().c_str());
        }
    }

    return result;
}

void Engine::ShutdownEngineSystems()
{
    for (uint32 ii = 0; ii < m_engineSystems.GetSize(); ++ii)
    {
        SystemBase *system = m_engineSystems[ii];
        Qi_LogInfo("Shutting down %s", system->GetName());
        
        system->Deinit();
        Qi_FreeMemory(system);
    }

    m_engineSystems.Clear();

    // Make sure the system pointers are all nulled out.
    m_entitySystem    = nullptr;
    m_renderingSystem = nullptr;
}

void Engine::AddSystem(SystemBase *system)
{
    QI_ASSERT(m_initiailzed);
    
    Qi_LogInfo("Adding system %s to the engine", system->GetName().c_str());
    
    m_engineSystems.PushBack(system);
}

#ifdef QI_DEBUG
void Engine::HandleLogMessages(const char *message, LogChannel channel)
{
    std::cout << message << std::endl;
}
#endif

} // namespace Qi

