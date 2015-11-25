//
//  SystemBase.cpp
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "SystemBase.h"
#include "../EngineConfig.h"

namespace Qi
{

QI_REFLECT_CLASS(SystemBase)
{
    QI_REFLECT_MEMBER(m_initialized);
}

SystemBase::SystemBase() :
    m_initialized(false),
    m_systemName("UnnamedSystem")
{

}

SystemBase::SystemBase(const std::string &systemName) :
    m_initialized(false),
    m_systemName(systemName)
{
}

SystemBase::~SystemBase()
{
    QI_ASSERT(!m_initialized);
}
    
Result SystemBase::Init(const ConfigVariables &configVariables)
{
    QI_ASSERT(0 && "This function should be overriden");
	return Result(ReturnCode::kUnknownError);
}
    
void SystemBase::Deinit()
{
    QI_ASSERT(0 && "This function should be overriden");
}
    
void SystemBase::Update(const float dt)
{
    QI_ASSERT(0 && "This function should be overriden");
}

const std::string SystemBase::GetName() const
{
    return m_systemName;
}

} // namespace Qi
