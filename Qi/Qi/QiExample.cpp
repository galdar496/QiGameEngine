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

class Lame
{
    public:
    
        QI_DECLARE_REFLECTION_CLASS(Lame);
 
		Lame() : x(15) {}
    private:
    
        int x;
};


class Test
{
    public:
    
    QI_DECLARE_REFLECTION_CLASS(Test);
    
    Test() :
        variable1(1),
        variable2(2),
        variable3('a')
        {}
    
   // private:
        int variable1;
        int variable2;
        char variable3;
        Lame l;
};

QI_REFLECT_DATA_MEMBERS(Test)
{
    QI_REFLECT_MEMBER(variable1);
    QI_REFLECT_MEMBER(variable2);
    QI_REFLECT_MEMBER(variable3);
    QI_REFLECT_MEMBER(l);
}

QI_REFLECT_DATA_MEMBERS(Lame)
{
    QI_REFLECT_MEMBER(x);
}

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
            const Qi::ReflectionData *data = Qi::ReflectionDataManager::GetInstance().GetReflectionData("Test");
            data->PrintMembers();
            
            Test t;
			t.variable1 = 100;
			t.variable3 = 'c';
            Qi::ReflectedVariable v(t);
			std::stringstream stream;
            v.Serialize(stream);
            
			Test t2;
			Qi::ReflectedVariable v2(t2);
			v2.Deserialize(stream);

			int x;
			std::cin >> x;
            return false;
        }
};

QI_IMPLEMENT_GAME(QiExample);
