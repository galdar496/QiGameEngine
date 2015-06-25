//
//  QiExample.cpp
//  Qi
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/AppFramework/QiGame.h"
#include "../Source/Core/Reflection/Reflection.h"
#include "../Source/Core/Reflection/ReflectedVariable.h"
#include <iostream>
#include <sstream>

class QiExample : public QiGameImpl
{
    public:
    
        virtual void Configure(Qi::EngineConfig &config) const override
        {
            config.flushLogFile = true;
        }
        
        virtual bool Init() override { return true; }

        virtual void AddCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const override {}
        
        virtual void Deinit() override {}
    
		virtual bool Step(const float dt) override
		{
            #ifdef QI_WINDOWS
                int x;
                std::cin >> x;
            #endif
            return false;
        }
};

QI_IMPLEMENT_GAME(QiExample);
