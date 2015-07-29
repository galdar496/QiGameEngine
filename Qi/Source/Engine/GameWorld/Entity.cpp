//
//  Entity.cpp
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Entity.h"

namespace Qi
{

QI_REFLECT_CLASS(Entity)
{
    QI_DECLARE_PARENT(Entity, RefCountedObject);
}

Entity::Entity()
{
}

Entity::~Entity()
{
}

Entity::Entity(const Entity &other)
{
}

Entity &Entity::operator=(const Entity &other)
{
    if (this != &other)
    {
        
    }
    
    return *this;
}

void Entity::Update(const float dt)
{
}

} // namespace Qi