//
//  EntitySystem.cpp
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "EntitySystem.h"
#include "../../Core/Utility/Logger/Logger.h"
#include "../EngineConfig.h"
#include "SystemConfig/ConfigFileReader.h"

namespace Qi
{

QI_REFLECT_CLASS(EntitySystem)
{
    QI_DECLARE_PARENT(EntitySystem, SystemBase);
    QI_REFLECT_MEMBER(m_entities);
}

EntitySystem::EntitySystem() :
    SystemBase("EntitySystem", "GameSettings")
{
}

EntitySystem::~EntitySystem()
{
}

Result EntitySystem::Init(const tinyxml2::XMLElement *rootEngineConfig)
{
    QI_ASSERT(!m_initialized);

    Result result(ReturnCode::kSuccess);

    // Read the EntitySystems config node.
    const tinyxml2::XMLElement *settings = rootEngineConfig->FirstChildElement(m_configNodeName.c_str());
    if (!settings)
    {
        result = ReturnCode::kMissingConfigNode;
    }
    else
    {
        ConfigFileReader reader(settings);

        {
            int maxEntities = reader.GetVariableValue<int>(g_ConfigVariables[ConfigVariable::kMaxWorldEntities]);

            result = m_entities.SetSize(maxEntities);
        }
    }
    
    m_initialized = result.IsValid();
    return result;
}

void EntitySystem::Deinit()
{
    QI_ASSERT(m_initialized);
    Qi_LogInfo("Deallocating world entities");
    
	m_entities.Clear();
    
    m_initialized = false;
}

void EntitySystem::Update(const float dt)
{
    // For now just loop over the entities and update them. In the
    // future, this function will add jobs to the job queue to update
    // the entities in parallel.
    QI_ASSERT(m_initialized);
    
	for (uint32 ii = 0; ii < m_entities.GetNumValidHandles(); ++ii)
    {
		m_entities[ii].Update(dt);
    }
}

EntitySystem::EntityHandle EntitySystem::CreateEntity()
{
    QI_ASSERT(m_initialized);
	return m_entities.AquireHandle();
}

void EntitySystem::RemoveEntity(const Qi::EntitySystem::EntityHandle &handle)
{
    QI_ASSERT(m_initialized);
	m_entities.ReleaseHandle(handle);
}

Entity &EntitySystem::GetEntity(const EntityHandle &handle)
{
	return m_entities.GetElement(handle);
}

} // namespace Qi
