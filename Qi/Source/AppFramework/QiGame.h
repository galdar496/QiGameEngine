//
//  QiGame.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Application driver for all Qi games. Invoke the macro QI_IMPLEMENT_GAME
/// to define a main function which will properly initialize
/// the engine and invoke your custom QiGameImpl.
///

#include "QiGameImpl.h"

class QiGame
{
    public:
    
        QiGame();
        ~QiGame();
    
        ///
        /// Run the passed-in QiGameImpl. This includes initialization of both
        /// the game and engine, running the game, and all shutdown.
        /// @param game Allocated derivation of QiGameImpl to control a game.
        ///
        void RunGame(QiGameImpl *game);
};

#define QI_IMPLEMENT_GAME(GameImpl)    \
    int main(int argc, const char* argv[])  \
    {                                       \
        QiGame game;                        \
        GameImpl *impl = new GameImpl;      \
        game.RunGame(impl);                 \
        delete impl;                        \
        return 0;                           \
    }

