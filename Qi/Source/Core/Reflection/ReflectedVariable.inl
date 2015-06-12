//
//  ReflectionData.inl
//  Qi Game Engine
//
//  Created by Cody White on 3/30/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "Reflection.h"

#define QI_GET_REFLECTED_TYPE(type) &(Qi::ReflectionDataCreator<typename Qi::QualifierRemover<type>::type>::GetInstance())

namespace Qi
{

template <typename T>
ReflectedVariable::ReflectedVariable(const T &value) :
    m_instanceData((void *)(&value))
{
    m_reflectionData = &(Qi::ReflectionDataCreator<typename Qi::QualifierRemover<T>::type>::GetInstance());
}

template <typename T>
const T &ReflectedVariable::GetValue() const
{		
	return *reinterpret_cast<const T *>(m_instanceData);
}

template <typename T>
T &ReflectedVariable::GetValue()
{
	return *reinterpret_cast<T *>(m_instanceData);
}

} // namespace Qi
