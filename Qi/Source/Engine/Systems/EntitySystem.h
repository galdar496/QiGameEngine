//
//  EntitySystem.h
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Managing system for all entities in the game world. Entities are defined as any objects
/// in the world that can interact with the the player(s) and each other.
///

#include "SystemBase.h"
#include "../../Core/Containers/TightlyPackedArray.h"
#include "../../Core/Reflection/Reflection.h"
#include "../GameWorld/Entity.h"
#include <string>

namespace Qi
{

class EntitySystem : public SystemBase
{
    public:
    
        QI_DECLARE_REFLECTED_CLASS(EntitySystem);
    
        EntitySystem();
        virtual ~EntitySystem() override;
    
        virtual Result Init(const tinyxml2::XMLElement *rootEngineConfig) override;
        virtual void Deinit() override;
        virtual void Update(const float dt) override;
        //////////////////////////////////////////
    
		typedef TightlyPackedArray<Entity>::Handle EntityHandle;
		static const EntityHandle INVALID_HANDLE = UINT_MAX;
    
        ///
        /// Reserve an entity for use in the game world.
        ///
        /// @return Handle to an entity.
        ///
        EntityHandle CreateEntity();
    
        ///
        /// Remove an entity from the world. This cannot be called while
		/// the entities are being updated.
        ///
        /// @param handle Handle to the entity to remove.
        ///
        void RemoveEntity(const EntityHandle &handle);

		///
		/// Get an entity from the system that has already been created.
		///
		/// @param handle Handle to the entity to retrieve.
		/// @return Reference to the internal entity.
		///
		Entity &GetEntity(const EntityHandle &handle);
    
    private:
    
        // This object is non-copyable.
        EntitySystem(const EntitySystem &other) = delete;
        EntitySystem &operator=(const EntitySystem &other) = delete;

		TightlyPackedArray<Entity> m_entities; ///< Entities managed by this system.
};

} // namespace Qi
