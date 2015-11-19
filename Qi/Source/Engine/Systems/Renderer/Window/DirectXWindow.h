//
//  DirectXWindow.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Implement a DirectX windowing system.
///

#include "WindowBase.h"
#include "../../../../Core/Defines.h"

#ifdef QI_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

namespace Qi
{

class DirectXWindow : public WindowBase
{
	public:

		DirectXWindow();
		~DirectXWindow();

		////// Overrides for WindowBase API //////

		virtual Result Init(WindowCInfo config) override;
		virtual void Deinit() override;
		virtual void Update(const float dt) override;
		//////////////////////////////////////////

	private:

		HINSTANCE m_windowInstance; ///< DirectX instance of this window.
		HWND m_window;              ///< DirectX window object.
		LPCSTR m_windowTitle;       ///< Title of the window.
};

} // namespace Qi

#endif
