//
//  SystemBase.cpp
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "SystemBase.h"

namespace Qi
{

QI_REFLECT_CLASS(SystemBase)
{
    QI_REFLECT_MEMBER(m_initialized);
}

SystemBase::SystemBase() :
    m_initialized(false)
{
}

SystemBase::~SystemBase()
{
    QI_ASSERT(!m_initialized);
}
    
Result SystemBase::Init(const Cinfo *info)
{
    QI_ASSERT(0 && "This function should be overriden");
}
    
void SystemBase::Deinit()
{
    QI_ASSERT(0 && "This function should be overriden");
}
    
void SystemBase::Update(const float dt)
{
    QI_ASSERT(0 && "This function should be overriden");
}

std::string SystemBase::GetName() const
{
    QI_ASSERT(0 && "This function should be overriden");
}

} // namespace Qi
