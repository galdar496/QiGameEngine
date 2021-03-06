//
//  DirectXWindow.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "DirectXWindow.h"
#include "../../../WindowMessage.h"
#include "../../../../Core/Utility/Logger/Logger.h"
#include "../../../Engine.h"

#ifdef QI_WINDOWS

namespace Qi
{

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DirectXWindow::DirectXWindow()
{

}

DirectXWindow::~DirectXWindow()
{
	// Just in case.
	Deinit();
}

Result DirectXWindow::Init(WindowCInfo config)
{
	Qi_LogInfo("Creating DirectX rendering window");

	Result result(ReturnCode::kSuccess);

	// Get the instance of this application.
	m_windowInstance = GetModuleHandle(NULL);
	m_windowTitle    = (LPCSTR)config.windowName.c_str();

	// Setup the windows class.
	WNDCLASSEX windowsClass;
	windowsClass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowsClass.lpfnWndProc    = WndProc;
	windowsClass.cbClsExtra     = 0;
	windowsClass.cbWndExtra     = 0;
	windowsClass.hInstance      = m_windowInstance;
	windowsClass.hIcon          = LoadIcon(NULL, IDI_WINLOGO);
	windowsClass.hIconSm        = windowsClass.hIcon;
	windowsClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	windowsClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowsClass.lpszMenuName   = NULL;
	windowsClass.lpszClassName  = m_windowTitle;
	windowsClass.cbSize         = sizeof(WNDCLASSEX);

	RegisterClassEx(&windowsClass);

	int posX = 0;
	int posY = 0;
	if (config.fullscreen)
	{
		// Setup fullscreen options.
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));

		screenSettings.dmSize       = sizeof(screenSettings);
		screenSettings.dmPelsWidth  = config.screenWidth;
		screenSettings.dmPelsHeight = config.screenHeight;
		screenSettings.dmBitsPerPel = 32;
		screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - config.screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - config.screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_window = CreateWindowEx(WS_EX_APPWINDOW, 
		                      m_windowTitle, 
		                      m_windowTitle,
		                      WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU,
		                      posX, 
		                      posY, 
		                      config.screenWidth, 
		                      config.screenHeight, 
		                      NULL, 
		                      NULL, 
		                      m_windowInstance, 
		                      NULL);
	if (m_window == nullptr)
	{
		Qi_LogError("Unable to create DirectX rendering window. CreateWindowEx error code: %d", GetLastError());
		result.code = ReturnCode::kWindowCreationFailed;
	}
	else
	{
        // Give the engine pointer to the windowing system so that the engine can handle 
        // all necessary messages.
        SetWindowLongPtr(m_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(config.engine));

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(m_window, SW_SHOW);
		SetForegroundWindow(m_window);
		SetFocus(m_window);

		// Hide the mouse cursor.
		//ShowCursor(false);
	}

	return result;
}

void DirectXWindow::Deinit()
{
	if (m_window)
	{
		// Show the mouse cursor.
		ShowCursor(true);

		// Fix the display settings if leaving full screen mode.
		//if (FULL_SCREEN)
		{
			//ChangeDisplaySettings(NULL, 0);
		}

		// Remove the window.
		DestroyWindow(m_window);
		m_window = nullptr;

		// Remove the application instance.
		UnregisterClass(m_windowTitle, m_windowInstance);
		m_windowInstance = NULL;
	}
}

void DirectXWindow::Update(const float dt)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Handle the windows messages.
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

    // Extract the engine pointer.
    Engine *engine = reinterpret_cast<Engine *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    bool messageHandled = false;
    if (engine != nullptr)
    {
        Win32WindowMessage message(umessage, wparam, lparam);
        messageHandled = engine->HandleMessage(&message);
    }
    
    // Either the engine object is not yet valid OR the engine did not handle
    // the message. Either way, let Windows take care of the message.
    if (!messageHandled)
    {
        result =  DefWindowProc(hwnd, umessage, wparam, lparam);
    }

	return result;
}

} // namespace Qi

#endif
