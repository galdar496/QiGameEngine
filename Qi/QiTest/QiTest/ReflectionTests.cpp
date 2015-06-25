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
#include <sstream>

using namespace Qi;

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

	Qi::ReflectedVariable v(s);
	std::stringstream stream;
	v.Serialize(stream);

	Simple *s2 = nullptr;
	Qi::ReflectedVariable v2(s2);
	v2.Deserialize(stream);

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

	Qi::ReflectedVariable v(s);
	std::stringstream stream;
	v.Serialize(stream);

	SimpleArray *s2 = nullptr;
	Qi::ReflectedVariable v2(s2);
	v2.Deserialize(stream);

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

	Qi::ReflectedVariable v(array);
	std::stringstream stream;
	v.Serialize(stream);

	ArrayOfObjects *array2 = nullptr;
	Qi::ReflectedVariable v2(array2);
	v2.Deserialize(stream);

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

	Qi::ReflectedVariable v(p);
	std::stringstream stream;
	v.Serialize(stream);

	Pointer *p2 = nullptr;
	Qi::ReflectedVariable v2(p2);
	v2.Deserialize(stream);

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

TEST(Reflection, InheritanceTest)
{
	Derived d;
	d.base1 = false;

	Qi::ReflectedVariable v(d);
	std::stringstream stream;
	v.Serialize(stream);

	Derived *d2 = nullptr;
	Qi::ReflectedVariable v2(d2);
	v2.Deserialize(stream);

	EXPECT_EQ(false, d2->base1);
	EXPECT_EQ(true,  d2->derived1);

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

	Qi::ReflectedVariable v(c);
	std::stringstream stream;
	v.Serialize(stream);

	Complex *c2 = nullptr;
	Qi::ReflectedVariable v2(c2);
	v2.Deserialize(stream);

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

