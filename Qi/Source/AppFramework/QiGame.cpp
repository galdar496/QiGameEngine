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
#include <assert.h>

QiGame::QiGame()
{
}

QiGame::~QiGame()
{
}

void QiGame::runGame(QiGameImpl *game)
{
    assert(game != nullptr);
    
    Qi::Engine engine;
    
    // Configure and initialize the engine and game.
    {
        Qi::EngineConfig config;
        game->configure(config);
        
        if (!(engine.init(config) && game->init()))
        {
            return;
        }
    }
    
    // Add any custom systems to the engine.
    {
        Qi::Array<Qi::SystemBase *> custom_systems;
        game->addCustomSystems(custom_systems);
        for (uint32 ii = 0; ii < custom_systems.getSize(); ++ii)
        {
            engine.addSystem(custom_systems(ii));
        }
    }
    
    // Run the game!
    Qi::Timer timer;
    
    bool run = true;
    timer.start();
    while (run)
    {
        float dt = timer.dt();
        
        // Step the engine forward first.
        engine.step(dt);
        
        // Now step the game forward.
        run = game->step(dt);
    }
    
    // Game is over, deinitialize the game and shut the engine down.
    game->deinit();
    engine.shutdown();
}
