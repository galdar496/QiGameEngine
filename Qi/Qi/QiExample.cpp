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
    
        virtual void configure(Qi::EngineConfig &config) const
        {
            config.flushLogFile = true;
        }
        
        virtual bool init() { return true; }
    
        virtual void addCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const {}
        
        virtual void deinit() {}
    
        virtual bool step(const float dt) { return false; }
};

QI_IMPLEMENT_GAME(QiExample);
