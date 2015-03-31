//
//  Reflection.h
//  Qi Game Engine
//
//  Created by Cody White on 3/23/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <iostream>
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
#define QI_DECLARE_REFLECTION(classType) \
    static void AddMember(const std::string &name, size_t offset, const Qi::ReflectionData *data); \
    static Qi::QualifierRemover<classType>::type *NullCast(); \
    static void RegisterReflectedData();

///
/// Reflect the data members of class 'classType'. This macro acts like a function
/// which should be called like this:
/// QI_REFLECT_DATA_MEMBERS(SomeClassType)
/// {
///     QI_REFLECT_MEMBER(memberName1);
///     QI_REFLECT_MEMBER(memberName2);
///     QI_REFLECT_MEMBER(memberName3);
///     QI_REFLECT_MEMBER(memberName4);
/// }
///
/// This can ONLY be called on a class that has been declared with reflection data
/// with QI_DECLARE_REFLECTION().
///
#define QI_REFLECT_DATA_MEMBERS(classType) \
    Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type> QI_UNIQUE_NAME( )(#classType, sizeof(classType));\
    Qi::QualifierRemover<classType>::type *classType::NullCast() { return (Qi::QualifierRemover<classType>::type *)(nullptr); }\
    void classType::AddMember(const std::string &name, size_t offset, const Qi::ReflectionData *data) { return Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type>::AddMember(name, offset, data); }\
    template<> void Qi::ReflectionDataCreator<Qi::QualifierRemover<classType>::type>::RegisterReflectedData() { classType::RegisterReflectedData(); }\
    void classType::RegisterReflectedData()

///
/// Reflect a specific member of a class. This must be called within
/// the scope of the macro QI_REFLECT_DATA_MEMBERS.
///
#define QI_REFLECT_MEMBER(memberName) \
    AddMember(#memberName, (size_t)(&(NullCast()->memberName)), &(Qi::ReflectionDataCreator<Qi::QualifierRemover<decltype(NullCast()->memberName)>::type>::GetInstance()));

///
/// Generate a unique name. Make use of QI_UNIQUE_NAME, the other macros
/// should not be directly called. Pass in a class name and get the same
/// name back with a unique number afterwards, e.g. Foo -> Foo1
///
#define QI_APPEND_TOKENS(counter) __Qi_Reflected##counter
#define QI_UNIQUE_NAME_INTERNAL(counter) QI_APPEND_TOKENS(counter)
#define QI_UNIQUE_NAME( ) QI_UNIQUE_NAME_INTERNAL(__COUNTER__)
