//
//  QiApp.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "QiApp.h"
#include "../Engine/Engine.h"
#include <assert.h>

QiApp::QiApp(QiAppImpl *app) :
    m_engine(NULL),
    m_app(app)
{
}

QiApp::~QiApp()
{
    delete m_app;
    m_app = NULL;
    
    delete m_engine;
    m_engine = NULL;
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
    assert(m_app != NULL);
    
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
        while (run)
        {
            // Step the engine forward first.
            m_engine->step();
            
            // Now step the app forward.
            run = m_app->run();
        }
        
        m_app->deinit();
    }
}
