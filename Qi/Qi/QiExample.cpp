//
//  QiExample.cpp
//  Qi
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/AppFramework/QiGame.h"

class QiExample : public QiGameImpl
{
    public:
    
        virtual void Configure(Qi::EngineConfig &config) const
        {
            config.flushLogFile = true;
        }
        
        virtual bool Init() { return true; }

        virtual void AddCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const {}
        
        virtual void Deinit() {}
    
        virtual bool Step(const float dt) { return false; }
};

QI_IMPLEMENT_GAME(QiExample);
