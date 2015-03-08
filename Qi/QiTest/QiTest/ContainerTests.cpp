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
    EXPECT_EQ(0, a.GetSize());
}

TEST(Array, OneElement)
{
    Array<int> a;
    a.PushBack(2);
    
    EXPECT_EQ(1, a.GetSize());
    EXPECT_EQ(2, a(0));
}

TEST(Array, MultipleElements)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(2);
    a.PushBack(3);
    a.PushBack(4);
    
    EXPECT_EQ(4, a.GetSize());
    EXPECT_EQ(1, a(0));
    EXPECT_EQ(2, a(1));
    EXPECT_EQ(3, a(2));
    EXPECT_EQ(4, a(3));
}

TEST(Array, Resize)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(2);
    
    a.Resize(2);
    a.PushBack(3);
    a.PushBack(4);
    
    EXPECT_EQ(3, a(0));
    EXPECT_EQ(4, a(1));
}

TEST(Array, Clear)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(2);
    a.PushBack(3);
    
    a.Clear();
    EXPECT_EQ(0, a.GetSize());
}

TEST(Array, Reallocate)
{
    Array<int> a;
    a.PushBack(0);
    
    uint32 allocated_size = a.GetAllocateSize();
    for (int ii = 1; ii < allocated_size; ++ii)
    {
        a.PushBack(ii);
    }
    
    // At this point, the array should be full. The next
    // allocation will cause an internal reallocation.
    a.PushBack(100);
    
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
        a.PushBack(ii);
    }
    
    Array<int> copy1(a);
    for (int ii = 0; ii < a.GetSize(); ++ii)
    {
        EXPECT_EQ(a(0), copy1(0));
    }
    
    Array<int> copy2 = a;
    for (int ii = 0; ii < a.GetSize(); ++ii)
    {
        EXPECT_EQ(a(0), copy2(0));
    }
}

