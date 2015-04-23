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
    
		QI_DECLARE_REFLECTED_CLASS(Lame);
 
		Lame() : x(15) {}
    private:
    
        int x;
};


class Test
{
    public:
    
		QI_DECLARE_REFLECTED_CLASS(Test);
    
	Test() :
		variable1(1),
		variable2(2),
		variable3('a') //{}
         {
			 array[0] = 'a';
			 array[1] = 'b';
			 array[2] = 'c';
		 }
    
   // private:
        int variable1;
        int variable2;
        char variable3;
        Lame l;

		char array[3];
};

QI_REFLECT_CLASS(Test)
{
    QI_REFLECT_MEMBER(variable1);
    QI_REFLECT_MEMBER(variable2);
    QI_REFLECT_MEMBER(variable3);
    QI_REFLECT_MEMBER(l);
	QI_REFLECT_MEMBER(array);
}

QI_REFLECT_CLASS(Lame)
{
    QI_REFLECT_MEMBER(x);
}

class Base
{
	public:

		QI_DECLARE_REFLECTED_CLASS(Base);

		Base() : base1(true) {}

		bool base1;
};

class Derived : public Base
{
	public:

		QI_DECLARE_REFLECTED_CLASS(Derived);

		Derived() : derived1(true) {}

		bool derived1;
};

QI_REFLECT_CLASS(Base)
{
	QI_REFLECT_MEMBER(base1);
}

QI_REFLECT_CLASS(Derived)
{
	QI_DECLARE_PARENT(Derived, Base);
	QI_REFLECT_MEMBER(derived1);
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
//             const Qi::ReflectionData *data = Qi::ReflectionDataManager::GetInstance().GetReflectionData("Derived");
//             data->PrintMembers();
// 
// 			Derived d;
// 			d.base1 = false;
// 			d.derived1 = false;
// 			Qi::ReflectedVariable v(d);
// 			std::stringstream stream;
// 			v.Serialize(std::cout);

// 			Derived d2;
// 			Qi::ReflectedVariable v2(d2);
// 			v2.Deserialize(stream);
            
            Test t;
			t.variable1 = 100;
 			t.variable3 = 'c';
			t.array[0] = 'x';
			t.array[1] = 'y';
			t.array[2] = 'z';
            Qi::ReflectedVariable v(t);
			std::stringstream stream;
			v.Serialize(stream);
            
			Test t2;
			Qi::ReflectedVariable v2(t2);
			v2.Deserialize(stream);

            #ifdef QI_WINDOWS
                int x;
                std::cin >> x;
            #endif
            return false;
        }
};

QI_IMPLEMENT_GAME(QiExample);
