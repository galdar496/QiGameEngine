//
//  ReflectionDataManager.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ReflectionDataManager.h"
#include "ReflectionData.h"
#include "../Utility/StringUtilities.h"

namespace Qi
{
    
ReflectionDataManager::ReflectionDataManager()
{
}

ReflectionDataManager::~ReflectionDataManager()
{
    m_reflectedData.clear();
}

void ReflectionDataManager::AddReflectedData(const ReflectionData *data)
{
    QI_ASSERT(data != nullptr);
    
    uint32 hashedId = Qi::StringHash(data->GetName());
    
    QI_ASSERT(m_reflectedData.find(hashedId) == m_reflectedData.end());
    
    m_reflectedData[hashedId] = data;
}

const ReflectionData *ReflectionDataManager::GetReflectionData(const std::string &name)
{
    uint32 id = Qi::StringHash(name);
    return GetReflectionData(id);
}


const ReflectionData *ReflectionDataManager::GetReflectionData(uint32 hashedName)
{
    ReflectionTable::const_iterator iter = m_reflectedData.find(hashedName);
    if (iter != m_reflectedData.end())
    {
        return iter->second;
    }
    
    return nullptr;
}
    
} // namespace Qi

