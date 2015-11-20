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

// NOTE: Currently Qi only supports windows and DirectX.
#ifdef QI_WINDOWS
	#include "Window/DirectXWindow.h"
#endif

namespace Qi
{

RenderingSystem::RenderingSystem() :
	SystemBase("RenderingSystem", "RenderSettings"),
	m_window(nullptr)
{
}

RenderingSystem::~RenderingSystem()
{
	
}

Result RenderingSystem::Init(const tinyxml2::XMLElement *rootEngineConfig)
{
    QI_ASSERT(!m_initialized);

    Result result(ReturnCode::kSuccess);

    // Read the RenderingSystems config node.
    const tinyxml2::XMLElement *settings = rootEngineConfig->FirstChildElement(m_configNodeName.c_str());
    if (!settings)
    {
        result = ReturnCode::kMissingConfigNode;
    }
    else
    {
        ConfigFileReader reader(settings);

        {
            // Create the windowing system and initialize it.
            #ifdef QI_WINDOWS
                m_window = Qi_AllocateMemory(DirectXWindow);
            #endif

            WindowBase::WindowCInfo cinfo;
            cinfo.screenWidth  = reader.GetVariableValue<int>(g_ConfigVariables[ConfigVariable::kWindowWidth]);
            cinfo.screenHeight = reader.GetVariableValue<int>(g_ConfigVariables[ConfigVariable::kWindowHeight]);
            cinfo.fullscreen   = reader.GetVariableValue<bool>(g_ConfigVariables[ConfigVariable::kFullscreen]);
            cinfo.windowName   = reader.GetVariableValue<std::string>(g_ConfigVariables[ConfigVariable::kGameName]);

            result = m_window->Init(cinfo);
        }
    }

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