//
//  ConfigVariables.h
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Define a configuration variables to read from an XML config file for each system.
///

#include <string>

namespace Qi
{

// Config variables that should be present in the engine config xml file. The parameters are marked as:
// "VariableName (as it is present in the config file), "VariableType", "VariableDefaultValue". Each system
// should have their own set of config variables.

#define RENDERING_SYSTEM_CONFIG_VARIABLES \
    X(WindowWidth, kInt, 800) \
    X(WindowHeight, kInt, 800) \
    X(Fullscreen, kBool, false) \
    X(GameName, kString, "Qi")

#define ENTITY_SYSTEM_CONFIG_VARIABLES \
    X(MaxWorldEntities, kInt, 10)

////////////////////////////////////////////////////////////////////////////////

// All config variables present in the system.
enum ConfigVariable
{
#define X(variableName, type, defaultValue) k##variableName,

    RENDERING_SYSTEM_CONFIG_VARIABLES
    ENTITY_SYSTEM_CONFIG_VARIABLES

#undef X
};

enum class VariableType
{
    kInt,
    kFloat,
    kBool,
    kString
};

struct Variable
{
    union Value
    {
        Value() {}
        Value(int integer) { i = integer; }
        Value(bool boolean) { b = boolean; }
        Value(float decimal) { f = decimal; }
        Value(const char *string) { strcpy_s(c, string); }

        int    i;
        bool   b;
        float  f;
        char   c[256];
    };

    explicit Variable(const std::string varName, VariableType varType, Value varDefaultValue) :
        name(varName),
        type(varType),
        value(varDefaultValue)
    {
    }

    VariableType type;
    const std::string  name;
    Value value;
};

#define X(variableName, type, defaultValue) Variable(#variableName, VariableType::type, (Variable::Value)defaultValue),
    
    // Table of all config variables in the system along with their default values.
    static const Variable g_ConfigVariables[] =
    {
        RENDERING_SYSTEM_CONFIG_VARIABLES
        ENTITY_SYSTEM_CONFIG_VARIABLES
        Variable("INVALID_VALUE", VariableType::kBool, (Variable::Value)10)
    };

#undef X

} // namespace Qi
