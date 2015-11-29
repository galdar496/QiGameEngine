//
//  WindowMessage.h
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Provie a platform-agnostic object for messages from the windowing system.
///

#include "../Core/Defines.h"

#ifdef QI_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif // QI_WINDOWS

namespace Qi
{
    // Message object which has different parameters based on which OS
    // it was compiled on.
    struct WindowMessage
    {
        WindowMessage() {}
    };

    // Win32-specific message.
    #ifdef QI_WINDOWS
        struct Win32WindowMessage : public WindowMessage
        {
            Win32WindowMessage(UINT _umessage, WPARAM _wparam, LPARAM _lparam) :
                WindowMessage(),
                umessage(_umessage),
                wparam(_wparam),
                lparam(_lparam)
            {}

            UINT umessage;
            WPARAM wparam;
            LPARAM lparam;
        };
    #endif // QI_WINDOWS

} // namespace Qi

