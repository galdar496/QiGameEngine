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

