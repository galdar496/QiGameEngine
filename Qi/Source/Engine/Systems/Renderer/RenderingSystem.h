//
//  RenderingSystem.h
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

#include "../SystemBase.h"
#include <string>

namespace Qi
{

// Forward declarations.
class WindowBase;

class RenderingSystem : public SystemBase
{
	public:

		RenderingSystem();
		virtual ~RenderingSystem() override;

		////// Overrides for SystemBase API //////
		virtual Result Init(const ConfigVariables &configVariables) override;
		virtual void Deinit() override;
		virtual void Update(const float dt) override;
		//////////////////////////////////////////

	private:

		WindowBase *m_window; ///< Window object to use for the engine rendering window.
};

} // namespace Qi