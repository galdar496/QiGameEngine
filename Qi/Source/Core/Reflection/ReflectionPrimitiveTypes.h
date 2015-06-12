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
#include "PointerTable.h"
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
#define QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(T) \
    Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type> QI_UNIQUE_NAME( )(#T, sizeof(T)); \
    template<> void Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::RegisterReflectionData() \
	{ \
		Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::GetInstance().SetSerializeFunction(SerializePrimitivePointer<Qi::QualifierRemover<T>::type>); \
		Qi::ReflectionDataCreator<Qi::QualifierRemover<T>::type>::GetInstance().SetDeserializeFunction(DeserializePrimitivePointer<Qi::QualifierRemover<T>::type>); \
	}

namespace Qi
{

template<class T>
void SerializePrimitiveValue(const ReflectedVariable *variable, std::ostream &stream, PointerTable &pointerTable)
{
	stream << variable->GetValue<T>() << std::endl;
}

template<class T>
void DeserializePrimitiveValue(ReflectedVariable *variable, std::istream &stream, PointerTable &pointerTable)
{
	stream >> variable->GetValue<T>();
}

//template<class T>
//void SerializePrimitivePointer(const ReflectedVariable *variable, std::ostream &stream, PointerTable &pointerTable)
//{
//	T address = variable->GetValue<T>();
//	ReflectedVariable v(variable->GetReflectionData(), address);
//	stream << "{ " << pointerTable.AddPointer(v, false) << " }" << std::endl;
//}
//
//template<class T>
//void DeserializePrimitivePointer(ReflectedVariable *variable, std::istream &stream, PointerTable &pointerTable)
//{
//	PointerTable::TableIndex index;
//	stream >> index;
//	variable->SetInstanceData(pointerTable.GetPointer(index).GetInstanceData());
//}


// Declare all supported POD reflected types.
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(int);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(int *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(float);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(float *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(double);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(double *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(char);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(char *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(bool);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(bool *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(uint32);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(uint32 *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(size_t);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(size_t *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(long);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(long *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(long long);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(long long *);
QI_DECLARE_REFLECTION_PRIMITIVE_TYPE(std::string);
//QI_DECLARE_REFLECTION_PRIMITIVE_TYPE_POINTER(std::string *);

} // namespace Qi