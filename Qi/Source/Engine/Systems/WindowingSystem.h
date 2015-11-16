//
//  WindowingSystem.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Define the windowing system to use to control engine windows. This windowing system will utilize a platform-specific
/// windowing environment and contains all interactions with it.
///

#include "SystemBase.h"
#include <string>

namespace Qi
{

// Forward declarations.
class WindowBase;

class WindowingSystem : public SystemBase
{
	public:

		WindowingSystem();
		virtual ~WindowingSystem() override;

		////// Overrides for SystemBase API //////

		struct WindowingSystemCInfo : public Cinfo
		{
			uint32 screenWidth;  ///< Width to create the screen (in pixels).
			uint32 screenHeight; ///< Height to create the screen (in pixels).
			bool   fullscreen;   ///< If true, initialize the window to be fullscreen.

			std::string windowName; ///< Name to use for the window title.
		};

		virtual Result Init(const Cinfo *info) override;
		virtual void Deinit() override;
		virtual void Update(const float dt) override;
		virtual std::string GetName() const override;
		//////////////////////////////////////////

	private:

		bool m_initialized; ///< If true, this system has been properly initialized.

		WindowBase *m_window; ///< Window object to use for the engine rendering window.
};

} // namespace Qi