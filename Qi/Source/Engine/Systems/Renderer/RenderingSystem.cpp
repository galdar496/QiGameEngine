//
//  RenderingSystem.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "RenderingSystem.h"
#include "Window/WindowBase.h"
#include "../../../Core/Defines.h"
#include "../../../Core/Utility/Logger/Logger.h"
#include "../../../Core/Memory/MemorySystem.h"
#include "../SystemConfig/ConfigFileReader.h"
#include "../../Engine.h"

// NOTE: Currently Qi only supports windows and DirectX.
#ifdef QI_WINDOWS
	#include "Window/DirectXWindow.h"
#endif

namespace Qi
{

RenderingSystem::RenderingSystem() :
	SystemBase("RenderingSystem"),
	m_window(nullptr)
{
}

RenderingSystem::~RenderingSystem()
{
	
}

Result RenderingSystem::Init(const CInfo &cinfo)
{
    QI_ASSERT(!m_initialized);

    Result result(ReturnCode::kSuccess);

    WindowBase::WindowCInfo windowCinfo;
    cinfo.configVariables->GetVariableValue<uint32>(ConfigVariables::kWindowWidth, windowCinfo.screenWidth);
    cinfo.configVariables->GetVariableValue<uint32>(ConfigVariables::kWindowHeight, windowCinfo.screenHeight);
    cinfo.configVariables->GetVariableValue<bool>(ConfigVariables::kFullscreen, windowCinfo.fullscreen);
    cinfo.configVariables->GetVariableValue<std::string>(ConfigVariables::kGameName, windowCinfo.windowName);
    windowCinfo.engine = cinfo.engine;

    // Create the windowing system and initialize it.
    #ifdef QI_WINDOWS
        m_window = Qi_AllocateMemory(DirectXWindow);
    #endif

    result = m_window->Init(windowCinfo);
  
    m_initialized = result.IsValid();
    return result;
}

void RenderingSystem::Deinit()
{
	QI_ASSERT(m_initialized);
	Qi_LogInfo("Deinitializing windowing system...");

	m_window->Deinit();
	Qi_FreeMemory(m_window);
	m_window = nullptr;

	m_initialized = false;
}

void RenderingSystem::Update(const float dt)
{
	QI_ASSERT(m_initialized);

	m_window->Update(dt);
}

} // namespace Qi