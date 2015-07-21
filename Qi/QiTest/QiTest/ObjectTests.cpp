//
//  ReflectionTests.cpp
//  QiTest
//
//  Created by Cody White on 6/24/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../Source/Core/Object/RefCountedObject.h"
#include "../../Source/Core/Memory/MemoryAllocator.h"
#include <thread>

using namespace Qi;

class TestObject : public RefCountedObject
{
public:

	TestObject()
	{
	}

	virtual ~TestObject()
	{
	}

	float value;
};

TEST(RefCounting, SimpleTest)
{
	TestObject t;
	EXPECT_EQ(1, t.GetReferenceCount());
}

TEST(RefCounting, MultipleReferencesTest)
{
	TestObject *original = Qi_AllocateMemory(TestObject);
	original->value = 1.0f;

	TestObject *t1, *t2, *t3;
	t1 = original; original->AddReference();
	t2 = original; original->AddReference();
	t3 = original; original->AddReference();
	EXPECT_EQ(4, original->GetReferenceCount());

	t1->ReleaseReference();
	EXPECT_EQ(3, original->GetReferenceCount());
	t2->ReleaseReference();
	EXPECT_EQ(2, original->GetReferenceCount());
	t3->ReleaseReference();
	EXPECT_EQ(1, original->GetReferenceCount());

	// Make sure class data is still valid.
	EXPECT_EQ(1.0f, original->value);

	// Now tell the original object to release itself.
	original->ReleaseReference();
}

void ThreadedAddReferences(int count, TestObject *object)
{
	for (int ii = 0; ii < count; ++ii)
	{
		object->AddReference();
	}
}

TEST(RefCounting, MultipleThreads)
{
	TestObject *object = Qi_AllocateMemory(TestObject);

	// Launch two threads that both try and increment the reference counts
	// of the object.
	std::thread t1(ThreadedAddReferences, 100, object);
	std::thread t2(ThreadedAddReferences, 15, object);
	t1.join();
	t2.join();

	// If everything worked properly, the ref count should be 116 (1 for the main thread, 
	// 100 for the second thread, and 15 for the third thread).
	EXPECT_EQ(116, object->GetReferenceCount());

	// Release all references.
	for (int ii = 0; ii < 116; ++ii)
	{
		object->ReleaseReference();
	}
}

