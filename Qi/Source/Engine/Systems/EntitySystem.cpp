//
//  EntitySystem.cpp
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "EntitySystem.h"
#include "../../Core/Utility/Logger/Logger.h"

namespace Qi
{

const std::string EntitySystem::M_NAME = "EntitySystem";

QI_REFLECT_CLASS(EntitySystem)
{
    QI_DECLARE_PARENT(EntitySystem, SystemBase);
    QI_REFLECT_MEMBER(m_entities);
	QI_REFLECT_MEMBER(m_idMap);
	QI_REFLECT_MEMBER(m_numFreeHandles);
	QI_REFLECT_MEMBER(m_numLiveEntities);
	QI_REFLECT_MEMBER(m_entityHandleFreeList);
}

EntitySystem::EntitySystem() :
    SystemBase(),
	m_numFreeHandles(0),
	m_numLiveEntities(0)
{
}

EntitySystem::~EntitySystem()
{
}

Result EntitySystem::Init(const Cinfo *info)
{
    QI_ASSERT(!m_initialized);
    
    const EntitySystemCInfo *cinfo = static_cast<const EntitySystemCInfo *>(info);
    
    Qi_LogInfo("Allocating space for %d entities", cinfo->maxEntities);
    
    Result result;
    
    // Perform the allocations for the world entities just once in this function.
    result = m_entities.Resize(cinfo->maxEntities);
    if (result.IsValid())
    {
        result = m_idMap.Resize(cinfo->maxEntities);
		m_entityHandleFreeList.Resize(cinfo->maxEntities);
		if (result.IsValid())
		{
			// Set the id map values to an invalid ID value.
			for (uint32 ii = 0; ii < cinfo->maxEntities; ++ii)
			{
				m_idMap[ii] = INVALID_HANDLE;
				m_entityHandleFreeList[ii] = cinfo->maxEntities - ii - 1; // Insert backwards for easier tracking.
			}
		}

		m_numFreeHandles = cinfo->maxEntities;
    }
    
    m_initialized = result.IsValid();

    return result;
}

void EntitySystem::Deinit()
{
    QI_ASSERT(m_initialized);
    Qi_LogInfo("Deallocating world entities");
    
    m_entities.Clear();
    m_idMap.Clear();
	m_entityHandleFreeList.Clear();
}

void EntitySystem::Update(const float dt)
{
    // For now just loop over the entities and update them. In the
    // future, this function will add jobs to the job queue to update
    // the entities in parallel.
    QI_ASSERT(m_initialized);
    
	for (uint32 ii = 0; ii < m_numLiveEntities; ++ii)
    {
        m_entities[ii].entity.Update(dt);
    }
}

EntitySystem::EntityHandle EntitySystem::CreateEntity()
{
    QI_ASSERT(m_initialized);
	QI_ASSERT(m_numFreeHandles > 0);
    
	EntityHandle handle = m_entityHandleFreeList[m_numFreeHandles - 1];
	--m_numFreeHandles;

    m_idMap[handle] = m_numLiveEntities;
	m_entities[m_numLiveEntities].uniqueId = handle;

	Qi_LogInfo("Created entity with handle %u", handle);
    
	++m_numLiveEntities;
    return handle;
}

void EntitySystem::RemoveEntity(const Qi::EntitySystem::EntityHandle &handle)
{
    QI_ASSERT(m_initialized);
	QI_ASSERT(handle < m_numLiveEntities);
    
	uint32 endEntity = m_numLiveEntities - 1;
    uint32 mappedId  = m_idMap[handle];

	QI_ASSERT(mappedId != INVALID_HANDLE);

	Qi_LogInfo("Removing entity with handle %u", handle);
    
    // Swap the entity to be removed with the last entity in the list.
	std::swap(m_entities[mappedId], m_entities[endEntity]);
	
    // Update the id map so that the newly swapped entity knows where it is later on.
	m_idMap[mappedId] = INVALID_HANDLE;
    m_idMap[m_entities[mappedId].uniqueId] = mappedId;
    
    // Decrement the number of live entities now that we've removed one.
	--m_numLiveEntities;

	m_entityHandleFreeList[m_numFreeHandles] = handle;
	++m_numFreeHandles;
}

Entity &EntitySystem::GetEntity(const EntityHandle &handle)
{
	EntityHandle mappedID = m_idMap[handle];
	QI_ASSERT(mappedID != INVALID_HANDLE);

	return m_entities[mappedID].entity;
}

std::string EntitySystem::GetName() const
{
    return M_NAME;
}

} // namespace Qi
