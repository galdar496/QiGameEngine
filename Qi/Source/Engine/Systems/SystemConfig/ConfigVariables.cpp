//
//  ConfigVariables.h
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "ConfigVariables.h"
#include "ConfigFileReader.h"
#include "../../../Core/Utility/Logger/Logger.h"
#include "../../../ThirdParty/tinyxml2.h"

namespace Qi
{

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
    static Variable g_ConfigVariables[] =
    {
        SYSTEM_CONFIG_VARIABLES
        Variable("INVALID_VALUE", VariableType::kBool, (Variable::Value)10)
    };

#undef X

ConfigVariables::ConfigVariables()
{

}

ConfigVariables::~ConfigVariables()
{

}

Result ConfigVariables::ParseConfigFile(const std::string &filename)
{
    Result result = ReturnCode::kSuccess;

    // Read each known element of the XML config file.
    const tinyxml2::XMLElement *rootConfigNode = nullptr;
    tinyxml2::XMLDocument xmlConfigFile;

    if (xmlConfigFile.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
    {
        Qi_LogError("Unable to load configuration file %s", filename.c_str());
        result = ReturnCode::kMissingConfigNode;
    }
    else
    {
        static const char *rootConfigNodeName = "QiEngineConfig";
        rootConfigNode = xmlConfigFile.FirstChildElement(rootConfigNodeName);
        if (rootConfigNode == nullptr)
        {
            Qi_LogError("Unable to load root config node %s in configuration file %s", rootConfigNodeName, filename.c_str());
            result = ReturnCode::kMissingConfigNode;
        }
        else
        {
            ConfigFileReader reader(rootConfigNode);

            for (int ii = 0; ii < kNumConfigVariables; ++ii)
            {
                Variable *variable = &g_ConfigVariables[ii];

                switch (variable->type)
                {
                    case VariableType::kInt:
                    {
                        reader.ReadVariableValue<int>(variable->name, variable->value.i);
                        break;
                    }
                    case VariableType::kBool:
                    {
                        reader.ReadVariableValue<bool>(variable->name, variable->value.b);
                        break;
                    }
                    case VariableType::kFloat:
                    {
                        reader.ReadVariableValue<float>(variable->name, variable->value.f);
                        break;
                    }
                    case VariableType::kString:
                    {
                        std::string value(variable->value.c);
                        reader.ReadVariableValue<std::string>(variable->name, value);
                        strcpy_s(variable->value.c, value.c_str());
                        break;
                    }
                }
            }
        }
    }

    return result;
}

int ConfigVariables::ReadIntVariable(const ConfigVariable &variable) const
{
    return g_ConfigVariables[variable].value.i;
}

bool ConfigVariables::ReadBoolVariable(const ConfigVariable &variable) const
{
    return g_ConfigVariables[variable].value.b;
}

float ConfigVariables::ReadFloatVariable(const ConfigVariable &variable) const
{
    return g_ConfigVariables[variable].value.f;
}

std::string ConfigVariables::ReadStringVariable(const ConfigVariable &variable) const
{
    return std::string(g_ConfigVariables[variable].value.c);
}

} // namespace Qi