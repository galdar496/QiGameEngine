//
//  WindowingSystem.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "WindowingSystem.h"
#include "Window/WindowBase.h"
#include "../../Core/Defines.h"
#include "../../Core/Utility/Logger/Logger.h"
#include "../../Core/Memory/MemorySystem.h"

// NOTE: Currently Qi only supports windows and DirectX.
#ifdef QI_WINDOWS
	#include "Window\DirectXWindow.h"
#endif

namespace Qi
{

WindowingSystem::WindowingSystem() :
	m_initialized(false),
	m_window(nullptr)
{
}

WindowingSystem::~WindowingSystem()
{
	
}

Result WindowingSystem::Init(const Cinfo *info)
{
	QI_ASSERT(!m_initialized);
	QI_ASSERT(info);
	Qi_LogInfo("Initializing windowing system...");

	const WindowingSystemCInfo *cinfo = static_cast<const WindowingSystemCInfo *>(info);

	// Create the windowing system and initialize it.
	#ifdef QI_WINDOWS
		m_window = Qi_AllocateMemory(DirectXWindow);
	#endif

	WindowBase::WindowCInfo windowInfo;
	windowInfo.screenWidth  = cinfo->screenWidth;
	windowInfo.screenHeight = cinfo->screenHeight;
	windowInfo.fullscreen   = cinfo->fullscreen;
	windowInfo.windowName   = cinfo->windowName;

	Result result = m_window->Init(windowInfo);
	if (result.IsValid())
	{
		m_initialized = true;
	}

	return result;
}

void WindowingSystem::Deinit()
{
	QI_ASSERT(m_initialized);
	Qi_LogInfo("Deinitializing windowing system...");

	m_window->Deinit();
	Qi_FreeMemory(m_window);
	m_window = nullptr;

	m_initialized = false;
}

void WindowingSystem::Update(const float dt)
{
	QI_ASSERT(m_initialized);

	m_window->Update(dt);
}

std::string WindowingSystem::GetName() const
{
	return "WindowingSystem";
}

} // namespace Qi