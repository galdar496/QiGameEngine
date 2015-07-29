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
    
        struct EntityHandle
        {
            Entity *entity;
            uint32 id;
        };
    
        ///
        /// Reserve an entity for use in the game world.
        ///
        /// @return Handle to an entity.
        ///
        EntityHandle CreateEntity();
    
        ///
        /// Remove an entity from the world.
        ///
        /// @param handle Handle to the entity to remove.
        ///
        void RemoveEntity(const EntityHandle &handle);
    
    private:
    
        // This object is non-copyable.
        EntitySystem(const EntitySystem &other) = delete;
        EntitySystem &operator=(const EntitySystem &other) = delete;
    
        static const std::string M_NAME; ///< Name of this class.
    
        Array<Entity> m_entities; ///< Array of all entities in the world. This array is allocated once and
                                  ///  handles to entities within are returned. All live entities are guaranteed
                                  ///  to be tightly packed together.
    
        uint32 m_currentIndex; ///< Index in m_entities immediately following the last entity in the world.
        Array<uint32> m_idMap; ///< Map of entity handles to indices within 'm_entities'. The index of an entity may
                               ///  change during removal of dead entites to handle fragmentation.

};

} // namespace Qi
