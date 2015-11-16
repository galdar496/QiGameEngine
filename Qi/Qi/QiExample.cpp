//
//  QiExample.cpp
//  Qi
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/AppFramework/QiGame.h"
#include <iostream>

#include "../Source/Core/Math/Vec4.h"
#include "../Source/Core/Memory/MemorySystem.h"

class QiExample : public QiGameImpl
{
    public:
    
        virtual void Configure(Qi::EngineConfig &config) const override
        {
            config.flushLogFile = true;
			config.gameTitle    = "QiExample";
        }
        
        virtual bool Init() override
        {
            return true;
        }

        virtual void AddCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const override {}
        
        virtual void Deinit() override {}
    
		virtual bool Step(const float dt) override
		{
			return true;
        }
};

QI_IMPLEMENT_GAME(QiExample);
