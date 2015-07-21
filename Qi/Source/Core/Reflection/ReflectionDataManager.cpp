//
//  ReflectionDataManager.cpp
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ReflectionDataManager.h"
#include "../Utility/StringUtilities.h"
#include "ReflectionPrimitiveTypes.h"

namespace Qi
{
    
ReflectionDataManager::ReflectionDataManager()
{
}

ReflectionDataManager::~ReflectionDataManager()
{
    m_reflectedData.clear();
}

ReflectionDataManager &ReflectionDataManager::GetInstance()
{
    static ReflectionDataManager manager;
    return manager;
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

void ReflectionDataManager::GetAllTypenames(Typenames &typenames) const
{
	QI_ASSERT(!m_reflectedData.empty());

	typenames.resize(m_reflectedData.size());

	// Add all typenames to the list by simply iterating over them.
	int index = 0;
	ReflectionTable::const_iterator iter = m_reflectedData.begin();
	for (; iter != m_reflectedData.end(); ++iter)
	{
		typenames[index++] = iter->second->GetName();
	}
}
    
} // namespace Qi

