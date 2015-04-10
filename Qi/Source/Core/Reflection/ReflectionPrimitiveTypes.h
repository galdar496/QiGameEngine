//
//  ReflectionPrimitiveTypes.h
//  Qi Game Engine
//
//  Created by Cody White on 3/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Definitions for all primitive types supported by the reflection system.
///

#include "Reflection.h"
#include "ReflectedVariable.h"

///
/// Macro to declare the reflection data for POD types. All reflected POD types are declared in this file.
///
#define QI_DECLARE_REFLECTION_POD(T) \
    Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type> QI_UNIQUE_NAME( )(#T, sizeof(T)); \
    template<> void Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::RegisterReflectedData() \
    { \
        Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::GetInstance().SetSerializeFunction(SerializePrimitiveValue<Qi::QualifierRemover<T>::type>); \
    }

namespace Qi
{

template<class T>
void SerializePrimitiveValue(const ReflectedVariable *variable, std::ostream &stream)
{
    stream << variable->GetValue<T>() << std::endl;
}

// Declare all supported POD reflected types.
QI_DECLARE_REFLECTION_POD(int);
QI_DECLARE_REFLECTION_POD(int *);
QI_DECLARE_REFLECTION_POD(float);
QI_DECLARE_REFLECTION_POD(float *);
QI_DECLARE_REFLECTION_POD(double);
QI_DECLARE_REFLECTION_POD(double *);
QI_DECLARE_REFLECTION_POD(char);
QI_DECLARE_REFLECTION_POD(char *);
QI_DECLARE_REFLECTION_POD(uint32);
QI_DECLARE_REFLECTION_POD(uint32 *);
QI_DECLARE_REFLECTION_POD(size_t);
QI_DECLARE_REFLECTION_POD(size_t *);
QI_DECLARE_REFLECTION_POD(long);
QI_DECLARE_REFLECTION_POD(long *);
QI_DECLARE_REFLECTION_POD(long long);
QI_DECLARE_REFLECTION_POD(long long *);
QI_DECLARE_REFLECTION_POD(std::string);
QI_DECLARE_REFLECTION_POD(std::string *);

} // namespace Qi