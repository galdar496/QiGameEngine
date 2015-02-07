//
//  QiExample.cpp
//  Qi
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/AppFramework/QiApp.h"

class QiExample : public QiAppImpl
{
    public:
    
        virtual void configure(Qi::EngineConfig &config)
        {
            config.flushLogFile = true;
        }
        
        virtual bool init() { return true; }
    
        virtual void addCustomSystems(std::vector<Qi::SystemBase *> systems) {}
        
        virtual void deinit() {}
    
        virtual bool run() { return true; }
};

QI_IMPLEMENT_APP(QiExample);
