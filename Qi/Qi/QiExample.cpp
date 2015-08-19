//
//  QiExample.cpp
//  Qi
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/AppFramework/QiGame.h"
#include <iostream>

#include "../Source/Core/Memory/MemorySystem.h"

int la = 0;

struct Test
{
	Test() { x = la++; std::cout << "CREATING " << x << std::endl; }
	~Test() { std::cout << "EXITING " << x << std::endl; }

	int x;
};

class QiExample : public QiGameImpl
{
    public:
    
        virtual void Configure(Qi::EngineConfig &config) const override
        {
            config.flushLogFile = true;
        }
        
        virtual bool Init() override
        {
            return true;
        }

        virtual void AddCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const override {}
        
        virtual void Deinit() override {}
    
		virtual bool Step(const float dt) override
		{
			Test *t = Qi_AllocateMemory(Test);
			std::cout << "1" << std::endl;
			Qi_FreeMemory(t);

// 			Test *t2 = Qi_AllocateMemoryArray(Test, 10);
// 			std::cout << "2" << std::endl;
// 			Qi_FreeMemory(t2);

            #ifdef QI_WINDOWS
                int x;
                std::cin >> x;
            #endif
            return false;
        }
};

QI_IMPLEMENT_GAME(QiExample);
