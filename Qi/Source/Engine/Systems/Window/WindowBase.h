//
//  WindowBase.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Provide an interface for a platform-specific window to work with. This window will be controlled through the WindowingSystem.
///

#include "../../../Core/BaseTypes.h"
#include <string>

namespace Qi
{

class WindowBase
{
	public:

		WindowBase();
		virtual ~WindowBase();

		struct WindowCInfo
		{
			uint32 screenWidth;  ///< Width of the window (in pixels).
			uint32 screenHeight; ///< Height of the window (in pixels).
			bool   fullscreen;   ///< If true, create the window in fullscreen mode.

			std::string windowName; ///< Name to use for the window title.
		};

		///
		/// Initialize the window for use. Once this function returns, the window
		/// should be created and ready for use.
		///
		/// @param config Configuration object for enabling window options.
		/// @return If the system was properly initialized or not.
		///
		virtual Result Init(WindowCInfo config) = 0;

		///
		/// Deinitialize the window. This function should destroy the current window.
		///
		virtual void Deinit() = 0;

		virtual void Update(const float dt) = 0;
};

} // namespace Qi
