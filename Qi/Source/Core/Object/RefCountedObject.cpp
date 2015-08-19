//
//  RefCountedObject.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "RefCountedObject.h"
#include "../Memory/MemorySystem.h"

namespace Qi
{

QI_REFLECT_CLASS(RefCountedObject)
{
	QI_REFLECT_MEMBER(m_refCount);
}

RefCountedObject::RefCountedObject() :
	m_refCount(1)
{
}

RefCountedObject::~RefCountedObject()
{
	// Do nothing here as all deletion should occur from
	// releasing references.
}

RefCountedObject::RefCountedObject(const RefCountedObject &other)
{
	// Do not actually copy here as a new object has it's own referene count.
	// Instead, just set the ref count to 1.
	m_refCount = 1;
}

RefCountedObject &RefCountedObject::operator = (const RefCountedObject &other)
{
	// Do nothing here. The object will set its ref count appropriately and should
	// not copy one from another object.
	return *this;
}

void RefCountedObject::AddReference()
{
	// Increment the ref count using lockless semantics.
	uint32 oldCount = 0;
	do 
	{
		// Read the current ref count.
		oldCount = m_refCount;

		// If the ref count hasn't changed, set it to the previous value plus 1. If it has
		// changed, re-run the loop.
	} while (!std::atomic_compare_exchange_weak(&m_refCount, &oldCount, oldCount + 1));
}

void RefCountedObject::ReleaseReference()
{
	// Decrement the ref count using lockless semantics.
	uint32 oldCount = 0;
	uint32 newCount = 0;
	do
	{
		// Read the current ref count.
		oldCount = m_refCount;
		newCount = oldCount - 1;

		// If the ref count hasn't changed, set it to the previous value plus 1. If it has
		// changed, re-run the loop.
	} while (!std::atomic_compare_exchange_weak(&m_refCount, &oldCount, newCount));

	if (newCount == 0)
	{
		DeleteThisObject();
	}

	QI_ASSERT(m_refCount >= 0 && "Reference count has fallen below 0");
}

uint32 RefCountedObject::GetReferenceCount() const
{
	return m_refCount;
}

void RefCountedObject::DeleteThisObject()
{
	Qi_FreeMemory(this);
}

} // namespace Qi

