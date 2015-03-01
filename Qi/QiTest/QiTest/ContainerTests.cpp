//
//  ContainerTests.cpp
//  QiTest
//
//  Created by Cody White on 3/1/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../Source/Core/Containers/Array.h"

using namespace Qi;

TEST(Array, ZeroSized)
{
    Array<int> a;
    EXPECT_EQ(0, a.getSize());
}

TEST(Array, OneElement)
{
    Array<int> a;
    a.pushBack(2);
    
    EXPECT_EQ(1, a.getSize());
    EXPECT_EQ(2, a(0));
}

TEST(Array, MultipleElements)
{
    Array<int> a;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    a.pushBack(4);
    
    EXPECT_EQ(4, a.getSize());
    EXPECT_EQ(1, a(0));
    EXPECT_EQ(2, a(1));
    EXPECT_EQ(3, a(2));
    EXPECT_EQ(4, a(3));
}

TEST(Array, Resize)
{
    Array<int> a;
    a.pushBack(1);
    a.pushBack(2);
    
    a.resize(2);
    a.pushBack(3);
    a.pushBack(4);
    
    EXPECT_EQ(3, a(0));
    EXPECT_EQ(4, a(1));
}

TEST(Array, Clear)
{
    Array<int> a;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    
    a.clear();
    EXPECT_EQ(0, a.getSize());
}

TEST(Array, Reallocate)
{
    Array<int> a;
    a.pushBack(0);
    
    uint32 allocated_size = a.getAllocateSize();
    for (int ii = 1; ii < allocated_size; ++ii)
    {
        a.pushBack(ii);
    }
    
    // At this point, the array should be full. The next
    // allocation will cause an internal reallocation.
    a.pushBack(100);
    
    // Check every value to make sure that it copied correctly.
    int ii = 0;
    for (; ii < allocated_size; ++ii)
    {
        EXPECT_EQ(ii, a(ii));
    }
    
    EXPECT_EQ(100, a(ii));
}

TEST(Array, CopyEntireArray)
{
    Array<int> a;
    for (int ii = 0; ii < 10; ++ii)
    {
        a.pushBack(ii);
    }
    
    Array<int> copy1(a);
    for (int ii = 0; ii < a.getSize(); ++ii)
    {
        EXPECT_EQ(a(0), copy1(0));
    }
    
    Array<int> copy2 = a;
    for (int ii = 0; ii < a.getSize(); ++ii)
    {
        EXPECT_EQ(a(0), copy2(0));
    }
}

