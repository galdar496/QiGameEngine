//
//  Win32WindowMessageHandler.cpp
//  Qi Game Engine
//
//  Created by Cody White on 11/11/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "WindowMessage.h"
#include "Engine.h"
#include "../Core/Defines.h"

#ifdef QI_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif // QI_WINDOWS

namespace Qi
{

#ifdef QI_WINDOWS

bool Engine::HandleMessage(WindowMessage *message)
{
    QI_ASSERT(message);

    bool messageHandled = true;
    Win32WindowMessage *winMessage = static_cast<Win32WindowMessage *>(message);

    // Process the message.
    switch (winMessage->umessage)
    {
        // Check if the window is being destroyed.
        case WM_DESTROY:
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            m_shouldShutdown = true;
            break;
        }

        // The engine does not handle this message.
        default:
        {
            messageHandled = false;
        }
    }

    return messageHandled;
}

#endif // QI_WINDOWS

} // namespace Qi