//
//  QiGame.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "QiGame.h"
#include "../Engine/Engine.h"
#include "../Core/Utility/Timer.h"

QiGame::QiGame()
{
}

QiGame::~QiGame()
{
}

void QiGame::RunGame(QiGameImpl *game)
{
    QI_ASSERT(game != nullptr);
    
    Qi::Engine engine;
    
    // Configure and initialize the engine and game.
    {
        Qi::EngineConfig config;
        game->Configure(config);
        
        if (!(engine.Init(config) && game->Init()))
        {
            return;
        }
    }
    
    // Add any custom systems to the engine.
    {
        Qi::Array<Qi::SystemBase *> customSystems;
        game->AddCustomSystems(customSystems);
        for (uint32 ii = 0; ii < customSystems.GetSize(); ++ii)
        {
            engine.AddSystem(customSystems[ii]);
        }
    }
    
    // Run the game!
    Qi::Timer timer;
    
    bool run = true;
    timer.Start();
    while (run)
    {
        float dt = timer.Dt();
        
        // Step the engine forward first.
        engine.Step(dt);
        
        // Now step the game forward.
        run = game->Step(dt);
    }
    
    // Game is over, deinitialize the game and shut the engine down.
    game->Deinit();
    engine.Shutdown();
}
