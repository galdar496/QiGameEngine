//
//  ContainerTests.cpp
//  QiTest
//
//  Created by Cody White on 3/1/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../Source/Core/Containers/Array.h"
#include "../../Source/Core/Containers/LocklessQueue.h"
#include <thread>

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
    EXPECT_EQ(2, a[0]);
}

TEST(Array, MultipleElements)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(2);
    a.PushBack(3);
    a.PushBack(4);
    
    EXPECT_EQ(4, a.GetSize());
    EXPECT_EQ(1, a[0]);
    EXPECT_EQ(2, a[1]);
    EXPECT_EQ(3, a[2]);
    EXPECT_EQ(4, a[3]);
}

TEST(Array, Resize)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(2);
    
    a.Resize(2);
    a.PushBack(3);
    a.PushBack(4);
    
    EXPECT_EQ(3, a[0]);
    EXPECT_EQ(4, a[1]);
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
    for (uint32 ii = 1; ii < allocated_size; ++ii)
    {
        a.PushBack(ii);
    }
    
    // At this point, the array should be full. The next
    // allocation will cause an internal reallocation.
    a.PushBack(100);
    
    // Check every value to make sure that it copied correctly.
    uint32 ii = 0;
    for (; ii < allocated_size; ++ii)
    {
        EXPECT_EQ(ii, a[ii]);
    }
    
    EXPECT_EQ(100, a[ii]);
}

TEST(Array, CopyEntireArray)
{
    Array<int> a;
    for (int ii = 0; ii < 10; ++ii)
    {
        a.PushBack(ii);
    }
    
    Array<int> copy1(a);
    for (uint32 ii = 0; ii < a.GetSize(); ++ii)
    {
        EXPECT_EQ(a[0], copy1[0]);
    }
    
    Array<int> copy2 = a;
    for (uint32 ii = 0; ii < a.GetSize(); ++ii)
    {
        EXPECT_EQ(a[0], copy2[0]);
    }
}

TEST(Array, SortAscending)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(10);
    a.PushBack(8);
    a.PushBack(3);
    a.PushBack(7);
    
    a.Sort(Qi::Array<int>::SortOrder::kAscending);
    EXPECT_EQ(1,  a[0]);
    EXPECT_EQ(3,  a[1]);
    EXPECT_EQ(7,  a[2]);
    EXPECT_EQ(8,  a[3]);
    EXPECT_EQ(10, a[4]);
}

TEST(Array, SortDescending)
{
    Array<int> a;
    a.PushBack(1);
    a.PushBack(10);
    a.PushBack(8);
    a.PushBack(3);
    a.PushBack(7);
    
    a.Sort(Qi::Array<int>::SortOrder::kDescending);
    EXPECT_EQ(10, a[0]);
    EXPECT_EQ(8,  a[1]);
    EXPECT_EQ(7,  a[2]);
    EXPECT_EQ(3,  a[3]);
    EXPECT_EQ(1,  a[4]);
}

TEST(LocklessQueue, ZeroSized)
{
    LocklessQueue<int> q;
    q.Init(1);
    EXPECT_EQ(0, q.GetSize());
}

TEST(LocklessQueue, SingleThreadPushPop)
{
    LocklessQueue<int> q;
    q.Init(4);
    for (int ii = 0; ii < 4; ++ii)
    {
        q.Push(ii);
    }
    
    EXPECT_EQ(false, q.Push(1));
    EXPECT_EQ(4, q.GetSize());
    
    int v;
    for (int ii = 0; ii < 4; ++ii)
    {
        q.Pop(v);
        EXPECT_EQ(ii, v);
    }
    
    EXPECT_EQ(false, q.Pop(v));
}

TEST(LocklessQueue, Clear)
{
    LocklessQueue<int> q;
    q.Init(4);
    for (int ii = 0; ii < 4; ++ii)
    {
        q.Push(ii);
    }
    
    EXPECT_EQ(4, q.GetSize());
    
    q.Clear();
    EXPECT_EQ(0, q.GetSize());
}

void ThreadedAddData(int index, LocklessQueue<int> &q)
{
    for (int ii = index; ii < index + 16; ++ii)
    {
        q.Push(ii);
    }
}

void ThreadedRemoveData(int count, LocklessQueue<int> &q)
{
    int v;
    for (int ii = 0; ii < count; ++ii)
    {
        while (!q.Pop(v)) {}
    }
}

TEST(LocklessQueue, ThreadedPush)
{
    LocklessQueue<int> q;
    q.Init(32);
    
    std::thread t1(ThreadedAddData, 0, std::ref(q));
    std::thread t2(ThreadedAddData, 16, std::ref(q));
    t1.join();
    t2.join();
    
    // Since there are 32 values, we can use a 32bit unsigned int to make sure that
    // every value got properly added. After popping all elements off of the queue,
    // the mask should be 0.
    unsigned int mask = ~0;
    for (int ii = 0; ii < 32; ++ii)
    {
        int v;
        q.Pop(v);
        mask &= ~(1 << v);
    }
    
    EXPECT_EQ(0, mask);
}

TEST(LocklessQueue, ThreadedPushPop)
{
    LocklessQueue<int> q;
    q.Init(32);
    
    std::thread t1(ThreadedAddData, 0, std::ref(q));
    std::thread t2(ThreadedAddData, 16, std::ref(q));
    std::thread t3(ThreadedRemoveData, 32, std::ref(q));
    
    t1.join();
    t2.join();
    t3.join();
    
    EXPECT_EQ(0, q.GetSize());
}
