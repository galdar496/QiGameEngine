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
#include <ostream>
#include <istream>

///
/// Macro to declare the reflection data for primitive (POD) types. All reflected primitive types are declared in this file.
///
#define QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(T) \
    Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type> QI_UNIQUE_NAME( )(#T, sizeof(T)); \
    template<> void Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::RegisterReflectionData() \
    { \
        Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::GetInstance().SetSerializeFunction(SerializePrimitiveValue<Qi::QualifierRemover<T>::type>); \
		Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::GetInstance().SetDeserializeFunction(DeserializePrimitiveValue<Qi::QualifierRemover<T>::type>); \
    }

namespace Qi
{

template<class T>
void SerializePrimitiveValue(const ReflectedVariable *variable, std::ostream &stream)
{
	stream << variable->GetValue<T>() << std::endl;
}

template<class T>
void DeserializePrimitiveValue(ReflectedVariable *variable, std::istream &stream)
{
	stream >> variable->GetValue<T>();
}

/////// Specializations for serializing string types (have to be able to handle multiple words).
template<>
void SerializePrimitiveValue<std::string>(const ReflectedVariable *variable, std::ostream &stream)
{
	std::string string = variable->GetValue<std::string>();

	stream << string.length() << " " << string << std::endl;
}

template<>
void DeserializePrimitiveValue<std::string>(ReflectedVariable *variable, std::istream &stream)
{
	size_t stringLength = 0;
	stream >> stringLength;

	QI_ASSERT(stringLength > 0);

	// seek ahead one character to avoid reading the space inserted by the serialization function.
	stream.seekg(1, std::ios_base::cur);

	char tmpArray[1024];
	stream.read(tmpArray, stringLength);
	std::string tmp(tmpArray, stringLength);
	variable->GetValue<std::string>() = tmp;
}
////////

// Declare all supported POD reflected types.
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(int);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(float);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(double);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(char);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(bool);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(uint32);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(size_t);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(long);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(long long);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(std::string);

} // namespace Qi