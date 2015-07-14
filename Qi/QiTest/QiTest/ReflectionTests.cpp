//
//  ReflectionTests.cpp
//  QiTest
//
//  Created by Cody White on 6/24/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../Source/Core/Reflection/Reflection.h"
#include "../../Source/Core/Reflection/ReflectedVariable.h"
#include "../../Source/Core/Math/Vec4.h"
#include "../../Source/Core/Math/Quaternion.h"
#include "../../Source/Core/Math/Matrix4.h"
#include <sstream>

using namespace Qi;

template<class T>
void SerializeDeserialize(const T &in, T *&out)
{
	ReflectedVariable tmp1(in);
	std::stringstream stream;
	tmp1.Serialize(stream);

	ReflectedVariable tmp2(out);
	tmp2.Deserialize(stream);
}

class Simple
{
	public:

		QI_DECLARE_REFLECTED_CLASS(Simple);

		int x;
		int y;
};

QI_REFLECT_CLASS(Simple)
{
	QI_REFLECT_MEMBER(x);
	QI_REFLECT_MEMBER(y);
}

TEST(Reflection, SimpleTest)
{
	Simple s;
	s.x = 100;
	s.y = -123;

	Simple *s2 = nullptr;
	SerializeDeserialize(s, s2);

	EXPECT_EQ(s.x, s2->x);
	EXPECT_EQ(s.y, s2->y);

	// Cleanup.
	delete s2;
	s2 = nullptr;
}

class SimpleArray
{
public:

	QI_DECLARE_REFLECTED_CLASS(SimpleArray);

	int x[10];
};

QI_REFLECT_CLASS(SimpleArray)
{
	QI_REFLECT_MEMBER(x);
}

TEST(Reflection, SimpleArrayTest)
{
	SimpleArray s;
	for (int ii = 0; ii < 10; ++ii)
	{
		s.x[ii] = ii;
	}

	SimpleArray *s2 = nullptr;
	SerializeDeserialize(s, s2);

	for (int ii = 0; ii < 10; ++ii)
	{
		EXPECT_EQ(ii, s2->x[ii]);
	}

	// Cleanup.
	delete s2;
	s2 = nullptr;
}

class ArrayOfObjects
{
	public:

		QI_DECLARE_REFLECTED_CLASS(ArrayOfObjects);

		Simple s[10];
};

QI_REFLECT_CLASS(ArrayOfObjects)
{
	QI_REFLECT_MEMBER(s);
}

TEST(Reflection, ArrayOfObjectsTest)
{
	ArrayOfObjects array;
	for (int ii = 0; ii < 10; ++ii)
	{
		array.s[ii].x = ii;
		array.s[ii].y = ii;
	}

	ArrayOfObjects *array2 = nullptr;
	SerializeDeserialize(array, array2);

	for (int ii = 0; ii < 10; ++ii)
	{
		EXPECT_EQ(ii, array2->s[ii].x);
		EXPECT_EQ(ii, array2->s[ii].y);
	}

	// Cleanup.
	delete array2;
	array2 = nullptr;
}

class Pointer
{
	public:

		QI_DECLARE_REFLECTED_CLASS(Pointer);

		Simple *s;
};

QI_REFLECT_CLASS(Pointer)
{
	QI_REFLECT_MEMBER(s);
}

TEST(Reflection, PointerTest)
{
	Pointer p;
	p.s = new Simple();
	p.s->x = 123;
	p.s->y = 456;

	Pointer *p2 = nullptr;
	SerializeDeserialize(p, p2);

	EXPECT_EQ(123, p2->s->x);
	EXPECT_EQ(456, p2->s->y);

	// Cleanup.
	delete p.s;
	delete p2->s;
	delete p2;
}

class Base
{
public:

	QI_DECLARE_REFLECTED_CLASS(Base);

	Base() : base1(true) {}

	virtual int func()
	{
		return 1;
	}

	bool base1;
};

class Derived : public Base
{
public:

	QI_DECLARE_REFLECTED_CLASS(Derived);

	Derived() : derived1(true) {}

	virtual int func()
	{
		return 2;
	}

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

TEST(Reflection, InheritanceTest)
{
	Derived d;
	d.base1 = false;

	Derived *d2 = nullptr;
	SerializeDeserialize(d, d2);

	EXPECT_EQ(false, d2->base1);
	EXPECT_EQ(true,  d2->derived1);

	// Make sure the deserialized version has a proper vtable.
	EXPECT_EQ(2, d2->func());

	Base *b = static_cast<Base *>(d2);
	EXPECT_EQ(2, b->func());
	EXPECT_EQ(1, b->Base::func());

	// Cleanup.
	delete d2;
}

class Complex
{
	public:

		QI_DECLARE_REFLECTED_CLASS(Complex);

		Derived *derived;
		Pointer *p1;
		Pointer *p2;

		int i;
		float f;
		double d;

		bool array[5];

		ArrayOfObjects complexArray;
};

QI_REFLECT_CLASS(Complex)
{
 	QI_REFLECT_MEMBER(derived);
 	QI_REFLECT_MEMBER(p1);
 	QI_REFLECT_MEMBER(p2);
 	QI_REFLECT_MEMBER(i);
 	QI_REFLECT_MEMBER(f);
 	QI_REFLECT_MEMBER(d);
 	QI_REFLECT_MEMBER(array);
 	QI_REFLECT_MEMBER(complexArray);
}

TEST(Reflection, ComplexTest)
{
	Complex c;
	Derived d;
	d.derived1 = false;

	c.derived = &d;
	c.p1 = new Pointer;
	c.p1->s = new Simple;
	c.p1->s->x = 956;
	c.p1->s->y = -145;
	c.p2 = nullptr;

	c.i = 67;
	c.f = 124.036f;
	c.d = 10.45;

	c.array[0] = true;
	c.array[1] = true;
	c.array[2] = false;
	c.array[3] = false;
	c.array[4] = true;

	for (int ii = 0; ii < 10; ++ii)
	{
		c.complexArray.s[ii].x = ii;
		c.complexArray.s[ii].y = ii;
	}

	Complex *c2 = nullptr;
	SerializeDeserialize(c, c2);

	EXPECT_EQ(true, c2->derived->base1);
	EXPECT_EQ(false, c2->derived->derived1);
	EXPECT_EQ(956, c2->p1->s->x);
	EXPECT_EQ(-145, c2->p1->s->y);
	EXPECT_EQ(nullptr, c2->p2);

	EXPECT_EQ(67, c2->i);
	EXPECT_EQ(124.036f, c2->f);
	EXPECT_EQ(10.45, c2->d);

	EXPECT_EQ(true, c2->array[0]);
	EXPECT_EQ(true, c2->array[1]);
	EXPECT_EQ(false, c2->array[2]);
	EXPECT_EQ(false, c2->array[3]);
	EXPECT_EQ(true, c2->array[4]);

	for (int ii = 0; ii < 10; ++ii)
	{
		EXPECT_EQ(ii, c2->complexArray.s[ii].x);
		EXPECT_EQ(ii, c2->complexArray.s[ii].y);
	}

	delete c2->derived;
	delete c2->p1->s;
	delete c2->p1;
	delete c2;

	delete c.p1->s;
	delete c.p1;
}

TEST(Reflection, VectorTest)
{
	Qi::Vec4 v;
	v.v[0] = 1.0f;
	v.v[1] = 2.0f;
	v.v[2] = 3.0f;
	v.v[3] = 4.0f;

	Qi::Vec4 *v2;
	SerializeDeserialize(v, v2);

	EXPECT_EQ(1.0f, v2->v[0]);
	EXPECT_EQ(2.0f, v2->v[1]);
	EXPECT_EQ(3.0f, v2->v[2]);
	EXPECT_EQ(4.0f, v2->v[3]);

	delete v2;
}

TEST(Reflection, QuaternionTest)
{
	Qi::Quaternion q;
	q.q[0] = 1.0f;
	q.q[1] = 2.0f;
	q.q[2] = 3.0f;
	q.q[3] = 4.0f;

	Qi::Quaternion *q2;
	SerializeDeserialize(q, q2);

	EXPECT_EQ(1.0f, q2->q[0]);
	EXPECT_EQ(2.0f, q2->q[1]);
	EXPECT_EQ(3.0f, q2->q[2]);
	EXPECT_EQ(4.0f, q2->q[3]);

	delete q2;
}

TEST(Reflection, MatrixTest)
{
	Qi::Matrix4 m(1, 2, 3, 4,
		          5, 6, 7, 8,
		          9, 10, 11, 12,
		          13, 14, 15, 16);

	Qi::Matrix4 *m2;
	SerializeDeserialize(m, m2);

	EXPECT_EQ(1, (*m2)(0, 0));
	EXPECT_EQ(2, (*m2)(0, 1));
	EXPECT_EQ(3, (*m2)(0, 2));
	EXPECT_EQ(4, (*m2)(0, 3));

	EXPECT_EQ(5, (*m2)(1, 0));
	EXPECT_EQ(6, (*m2)(1, 1));
	EXPECT_EQ(7, (*m2)(1, 2));
	EXPECT_EQ(8, (*m2)(1, 3));

	EXPECT_EQ(9,  (*m2)(2, 0));
	EXPECT_EQ(10, (*m2)(2, 1));
	EXPECT_EQ(11, (*m2)(2, 2));
	EXPECT_EQ(12, (*m2)(2, 3));

	EXPECT_EQ(13, (*m2)(3, 0));
	EXPECT_EQ(14, (*m2)(3, 1));
	EXPECT_EQ(15, (*m2)(3, 2));
	EXPECT_EQ(16, (*m2)(3, 3));

	delete m2;
}

class StringObject
{
	public:

		QI_DECLARE_REFLECTED_CLASS(StringObject);

		std::string s1;
		std::string s2;
};

QI_REFLECT_CLASS(StringObject)
{
	QI_REFLECT_MEMBER(s1);
	QI_REFLECT_MEMBER(s2);
}

TEST(Reflection, StringTest)
{
	StringObject object;
	object.s1 = "This is a string";
	object.s2 = "This is also a string";

	StringObject *object2 = nullptr;
	SerializeDeserialize(object, object2);

	EXPECT_EQ("This is a string", object2->s1);
	EXPECT_EQ("This is also a string", object2->s2);

	delete object2;
}

TEST(Reflection, GetTypeTest)
{
    const std::string typeName = "StringObject";
    
    void *object = Qi::ReflectionDataManager::GetInstance().GetReflectionData(typeName)->AllocateInstance();
    
    EXPECT_EQ(true, object != nullptr);
}

TEST(Reflection, DataMemberCountTest)
{
    const Qi::ReflectionData *data = Qi::ReflectionDataManager::GetInstance().GetReflectionData("StringObject");
    EXPECT_EQ(2, data->GetMembers().size());
}

TEST(Reflection, UnknownType)
{
	const std::string typeName = "unknownType";

	const Qi::ReflectionData *data = Qi::ReflectionDataManager::GetInstance().GetReflectionData(typeName);
	EXPECT_EQ(nullptr, data);
}
