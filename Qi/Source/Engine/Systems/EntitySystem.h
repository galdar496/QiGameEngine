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
#include "../../Core/Containers/Array.h"
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
        virtual ~EntitySystem();
    
        ////// Overrides for SystemBase API //////
    
        struct EntitySystemCInfo : public Cinfo
        {
            uint32 maxEntities; ///< Maximum number of entities to allow in the game world at any one time.
        };
    
        virtual Result Init(const Cinfo *info) override;
        virtual void Deinit() override;
        virtual void Update(const float dt) override;
        virtual std::string GetName() const override;
        //////////////////////////////////////////
    
		typedef uint32 EntityHandle;
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
    
        static const std::string M_NAME; ///< Name of this class.

		///
		/// Record to use for associated a unique ID with a specific entity.
		///
		struct EntityRecord
		{
			EntityHandle uniqueId; ///< ID that is unique to only this entity.
			Entity entity;         ///< Entity itself.
		};
    
		Array<EntityRecord> m_entities; ///< Array of all entities in the world. This array is allocated once and
                                        ///  handles to entities within are returned. All live entities are guaranteed
                                        ///  to be tightly packed together.
    
		uint32 m_numLiveEntities; ///< Number of live entities in the system.
		Array<EntityHandle> m_idMap; ///< Map of entity handles to indices within 'm_entities'. The index of an entity may
                                     ///  change during removal of dead entities to handle fragmentation.

		Array<EntityHandle> m_entityHandleFreeList; ///< List of free unique IDs for new entities.
		uint32 m_numFreeHandles; ///< Number of free IDs in the freelist.
};

} // namespace Qi
