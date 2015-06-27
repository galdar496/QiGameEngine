//
//  Reflection.h
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "ReflectionData.h"
#include "QualifierRemover.h"

///
/// Specify the macros and classes necessary to generate reflection
/// information.
///

///
/// Declare functions necessary to reflect class 'classType'. This must be placed
/// in the 'public' section of a class otherwise Qi will fail to compile.
///
#define QI_DECLARE_REFLECTED_CLASS(classType) \
    static void AddMember(const std::string &name, size_t offset, size_t size, bool isPointer, const Qi::ReflectionData *data); \
    static Qi::QualifierRemover<classType>::type *NullCast(); \
    static void RegisterReflectionData();

///
/// Reflect the of class 'classType'. This macro acts like a function 
/// which should be called like this:
/// QI_REFLECT_CLASS(SomeClassType)
/// {
///     QI_REFLECT_MEMBER(memberName1);
///     QI_REFLECT_MEMBER(memberName2);
///     QI_REFLECT_MEMBER(memberName3);
///     QI_REFLECT_MEMBER(memberName4);
/// }
///
/// This can ONLY be called on a class that has been declared with reflection data
/// with QI_DECLARE_REFLECTED_CLASS().
///
#define QI_REFLECT_CLASS(classType) \
    const Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type> QI_UNIQUE_NAME( )(#classType, sizeof(classType)); \
    Qi::QualifierRemover<classType>::type *classType::NullCast() { return (Qi::QualifierRemover<classType>::type *)(nullptr); } \
    void classType::AddMember(const std::string &name, size_t offset, size_t size, bool isPointer, const Qi::ReflectionData *data) { return Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type>::AddMember(name, offset, size, isPointer, data); } \
    template<> void Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type>::RegisterReflectionData() { classType::RegisterReflectionData(); } \
    void classType::RegisterReflectionData()

///
/// Declare a parent type for the type specified by classType. Must be called within the scope
/// of the macro QI_REFLECT_CLASS.
///
#define QI_DECLARE_PARENT(classType, parentType) \
	Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type>::DeclareParent(&Qi::ReflectionDataCreator<Qi::QualifierRemover<parentType>::type>::GetInstance());

///
/// Reflect a specific member of a class. This must be called within
/// the scope of the macro QI_REFLECT_CLASS.
///
// sizeof(Qi::QualifierRemover<std::remove_all_extents<decltype(NullCast()->memberName)>::type >::type)
#define QI_REFLECT_MEMBER(memberName) \
    AddMember(#memberName, (size_t)(&(NullCast()->memberName)), \
              sizeof(NullCast()->memberName), \
			  Qi::QualifierRemover<decltype(NullCast()->memberName)>::IsPointer, \
             &(Qi::ReflectionDataCreator<Qi::QualifierRemover<std::remove_all_extents<decltype(NullCast()->memberName)>::type >::type>::GetInstance()));

///
/// Generate a unique name. Make use of QI_UNIQUE_NAME, the other macros
/// should not be directly called. Generates a name such as 
/// __Qi_Reflected1 etc.
///
#define QI_APPEND_TOKENS(counter) __Qi_Reflected##counter
#define QI_UNIQUE_NAME_INTERNAL(counter) QI_APPEND_TOKENS(counter)
#define QI_UNIQUE_NAME( ) QI_UNIQUE_NAME_INTERNAL(__COUNTER__)
