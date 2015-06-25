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

class Gra
{
public:

	QI_DECLARE_REFLECTED_CLASS(Gra);

	int x;
};

QI_REFLECT_CLASS(Gra)
{
	QI_REFLECT_MEMBER(x);
}

class Lame
{
    public:
    
		QI_DECLARE_REFLECTED_CLASS(Lame);
 
		//Lame() : x(15) {}
    //private:
    
		Gra g[5];
};
// 
// 
// class Test
// {
//     public:
//     
// 		QI_DECLARE_REFLECTED_CLASS(Test);
//     
// 	Test() :
// 		variable1(1),
// 		variable2(2),
// 		variable3('a') //{}
//          {
// 			 array[0] = 'a';
// 			 array[1] = 'b';
// 			 array[2] = 'c';
// 			 pointer1 = new int[4];
// 			 pointer2 = new int[4];
// 		 }
//     
//    // private:
//         int variable1;
//         int variable2;
//         char variable3;
//         Lame l;
// 
// 		char array[3];
// 		int *pointer1;
// 		int *pointer2;
// };
// 
// QI_REFLECT_CLASS(Test)
// {
//     QI_REFLECT_MEMBER(variable1);
//     QI_REFLECT_MEMBER(variable2);
//     QI_REFLECT_MEMBER(variable3);
//     QI_REFLECT_MEMBER(l);
// 	QI_REFLECT_MEMBER(array);
// 	QI_REFLECT_MEMBER(pointer1);
// 	QI_REFLECT_MEMBER(pointer2);
// }
// 
QI_REFLECT_CLASS(Lame)
{
    QI_REFLECT_MEMBER(g);
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
// 
// class PointerTest
// {
// public:
// 
// 	QI_DECLARE_REFLECTED_CLASS(PointerTest);
// 
// 	PointerTest() :
// 		x(10),
// 		y(1)
// 	{
// 
// 	}
// 
// 	int x;
// 	int y;
// };
// 
// class Gimli
// {
// public:
// 
// 	QI_DECLARE_REFLECTED_CLASS(Gimli);
// 
// 	Gimli() : x(15), y(1) { }
// 
//   	PointerTest *p;
//   	PointerTest *t;
// 
// 	int x;
// 	int y;
// 
// 	Derived *d;
// };

// QI_REFLECT_CLASS(PointerTest)
// {
// 	QI_REFLECT_MEMBER(x);
// 	QI_REFLECT_MEMBER(y);
// }
// 
// QI_REFLECT_CLASS(Gimli)
// {
//   	QI_REFLECT_MEMBER(p);
//   	QI_REFLECT_MEMBER(t);
// 
// 	QI_REFLECT_MEMBER(x);
// 	QI_REFLECT_MEMBER(y);
//  
//  	QI_REFLECT_MEMBER(d);
// }

class Test
{
public:

	QI_DECLARE_REFLECTED_CLASS(Test);

	Derived *d;
};

QI_REFLECT_CLASS(Test)
{
	QI_REFLECT_MEMBER(d);
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
		//             const Qi::ReflectionData *data = Qi::ReflectionDataManager::GetInstance().GetReflectionData("Test");
		//             data->PrintMembers();
		// 
// 		 			Derived d;
// 		 			d.base1 = false;
// 		 			d.derived1 = true;
// 		 			Qi::ReflectedVariable v(d);
// 		 			std::stringstream stream;
// 		 			v.Serialize(std::cout);

		// 			Derived d2;
		// 			Qi::ReflectedVariable v2(d2);
		// 			v2.Deserialize(stream);

// 		            Test t;
// 		 			t.variable1 = 100;
// 		  			t.variable3 = 'c';
// 		 			t.array[0] = 'x';
// 		 			t.array[1] = 'y';
// 		 			t.array[2] = 'z';
// 		            Qi::ReflectedVariable v(t);
// 		// 			std::stringstream stream;
// 		 			v.Serialize(std::cout);

		// 			Test t2;
		// 			Qi::ReflectedVariable v2(t2);
		// 			v2.Deserialize(stream);

		// 			std::cout << std::endl;
		// 

// 					Derived derived;
// 					derived.derived1 = false;
//  		 			Gimli test;
// 					test.x = 123;
// 					test.y = -125;
//  					test.d = &derived;
// 		 			test.p = new PointerTest;
//  		 			test.p->x = 11;
//   					test.t = new PointerTest;
// 					test.t->x = 124;
// 		 			Qi::ReflectedVariable v(test);
// 		 			std::stringstream stream;
// 					v.Serialize(stream);
// 
// 					std::cout << stream.str() << std::endl;
// 
// 					Gimli *test2 = nullptr;
// 					Qi::ReflectedVariable v2(test2);
// 				    v2.Deserialize(stream);

// 			Test t;
// 			t.l.g[0].x = 1;
// 			t.l.g[1].x = 2;
// 			t.l.g[2].x = 3;
// 			t.l.g[3].x = 4;
// 			t.l.g[4].x = 5;
// 			Qi::ReflectedVariable v(t);
// 			std::stringstream stream;
// 			v.Serialize(stream);
// 
// 			std::cout << stream.str() << std::endl;
// 
// 			Test *t2;
// 			Qi::ReflectedVariable v2(t2);
// 			v2.Deserialize(stream);

			Derived d;
			d.base1 = false;

// 			Qi::ReflectedVariable v(d);
// 			std::stringstream stream;
// 			v.Serialize(stream);
// 
// 			std::cout << stream.str() << std::endl;
// 
// 			Derived *d2 = nullptr;
// 			Qi::ReflectedVariable v2(d2);
// 			v2.Deserialize(stream);

			Test t;
			t.d = &d;
			Qi::ReflectedVariable v(t);
			std::stringstream stream;
			v.Serialize(stream);

			std::cout << stream.str() << std::endl;

			Test *t2;
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
