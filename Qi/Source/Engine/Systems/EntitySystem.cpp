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
    QI_REFLECT_MEMBER(m_currentIndex);
    QI_REFLECT_MEMBER(m_idMap);
}

EntitySystem::EntitySystem() :
    SystemBase()
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
		if (result.IsValid())
		{
			// Set the id map values to an invalid ID value.
			for (uint32 ii = 0; ii < m_idMap.GetSize(); ++ii)
			{
				m_idMap[ii] = INVALID_HANDLE;
			}
		}
    }
    
    m_initialized = true;

    return result;
}

void EntitySystem::Deinit()
{
    QI_ASSERT(m_initialized);
    Qi_LogInfo("Deallocating world entities");
    
    m_entities.Clear();
    m_idMap.Clear();
}

void EntitySystem::Update(const float dt)
{
    // For now just loop over the entities and update them. In the
    // future, this funtion will add jobs to the job queue to update
    // the entities in parallel.
    QI_ASSERT(m_initialized);
    
    for (uint32 ii = 0; ii < m_currentIndex; ++ii)
    {
        m_entities[ii].Update(dt);
    }
}

EntitySystem::EntityHandle EntitySystem::CreateEntity()
{
    QI_ASSERT(m_initialized);
    
	EntityHandle handle = m_currentIndex;

    // Entities start out with their original location in the map.
    m_idMap[handle] = handle;
    
    ++m_currentIndex;
    return handle;
}

void EntitySystem::RemoveEntity(const Qi::EntitySystem::EntityHandle &handle)
{
    QI_ASSERT(m_initialized);
    QI_ASSERT(handle < m_currentIndex);
    
    uint32 endEntity = m_currentIndex - 1;
    uint32 mappedId  = m_idMap[handle];
    
    // Swap the entity to be removed with the last entity in the list.
    m_entities[mappedId] = m_entities[endEntity];
    
    // Update the id map so that the newly swapped entity knows where it is later on.
    m_idMap[endEntity] = mappedId;
    
    // Decrement the current index now that we've removed an entity.
    --m_currentIndex;
}

Entity &EntitySystem::GetEntity(const EntityHandle &handle)
{
	EntityHandle mappedID = m_idMap[handle];
	QI_ASSERT(mappedID != INVALID_HANDLE);

	return m_entities[mappedID];
}

std::string EntitySystem::GetName() const
{
    return M_NAME;
}

} // namespace Qi
