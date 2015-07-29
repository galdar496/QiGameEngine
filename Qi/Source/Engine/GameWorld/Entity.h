//
//  Entity.h
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// An entity is an object in the game world that can be updated and rendered.
/// Typically, this refers to dynamic objects in the scene.
///

#include "../../Core/Reflection/Reflection.h"

namespace Qi
{

class Entity
{
    public:
    
        QI_DECLARE_REFLECTED_CLASS(Entity);
    
        Entity();
		~Entity();
        Entity(const Entity &other);
		Entity(Entity &&other);
        Entity &operator=(const Entity &other);
		Entity &operator=(Entity &&other);
    
        ///
        /// Update this entity for the current simulation step. The time
        /// since the last update was called is provided as a means for
        /// updating.
        ///
        /// @param dt Time since the last update in seconds.
        ///
        void Update(const float dt);
};

} // namespace Qi
