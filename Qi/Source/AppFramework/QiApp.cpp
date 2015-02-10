//
//  QiApp.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "QiApp.h"
#include "../Engine/Engine.h"
#include "../Core/Utility/Timer.h"
#include <assert.h>

QiApp::QiApp(QiAppImpl *app) :
    m_engine(nullptr),
    m_app(app)
{
}

QiApp::~QiApp()
{
    delete m_app;
    m_app = nullptr;
    
    delete m_engine;
    m_engine = nullptr;
}

bool QiApp::init()
{
    Qi::EngineConfig config;
    m_app->configure(config);
    
    m_engine = new Qi::Engine;
    return m_engine->init(config);
}

void QiApp::deinit()
{
    m_engine->shutdown();
}

void QiApp::execute()
{
    assert(m_app != nullptr);
    
    // Create the timer to use for between-frame dt.
    Qi::Timer timer;
    
    bool ok = m_app->init();
    if (ok)
    {
        // Add any custom systems to the engine.
        std::vector<Qi::SystemBase *> custom_systems;
        m_app->addCustomSystems(custom_systems);
        for (size_t ii = 0; ii < custom_systems.size(); ++ii)
        {
            m_engine->addSystem(custom_systems[ii]);
        }
        
        bool run = true;
        timer.start();
        while (run)
        {
            float dt = timer.dt();
            
            // Step the engine forward first.
            m_engine->step(dt);
            
            // Now step the app forward.
            run = m_app->run(dt);
        }
        
        m_app->deinit();
    }
}
