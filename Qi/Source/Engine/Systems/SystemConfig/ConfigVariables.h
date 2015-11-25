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

#include "../../../Core/BaseTypes.h"
#include "../../../Core/Defines.h"
#include <string>

namespace Qi
{

// Config variables that should be present in the engine config xml file. The parameters are marked as:
// "VariableName (as it is present in the config file), "VariableType", "VariableDefaultValue". Each system
// should have their own set of config variables.

#define SYSTEM_CONFIG_VARIABLES \
    X(WindowWidth, kInt, 800)     \
    X(WindowHeight, kInt, 800)    \
    X(Fullscreen, kBool, false)   \
    X(GameName, kString, "Qi")    \
    X(MaxWorldEntities, kInt, 10)    

////////////////////////////////////////////////////////////////////////////////

class ConfigVariables
{
    public:

        // All config variables present in the system.
        enum ConfigVariable
        {
            #define X(variableName, type, defaultValue) k##variableName,

                SYSTEM_CONFIG_VARIABLES
                kNumConfigVariables

            #undef X
        };

        ConfigVariables();
        ~ConfigVariables();

        ///
        /// Read the config file and populate the list of config variables.
        /// 
        /// @filename Path to the config XML file which contains the engine configuration.
        /// @return If valid, the file was successfully parsed.
        ///
        Result ParseConfigFile(const std::string &filename);

        template<class T>
        void GetVariableValue(const ConfigVariable &variable, T &value) const
        {
            ReadVariable<T>(variable, value);
        }

    private:

        template<class T>
        void ReadVariable(const ConfigVariable &variable, T &value) const
        {
            QI_ASSERT(0 && "Unknown variable type");
        }

        template<>
        void ReadVariable(const ConfigVariable &variable, int &value) const
        {
            value = ReadIntVariable(variable);
        }

        template<>
        void ReadVariable(const ConfigVariable &variable, uint32 &value) const
        {
            value = ReadIntVariable(variable);
        }

        template<>
        void ReadVariable(const ConfigVariable &variable, bool &value) const
        {
            value = ReadBoolVariable(variable);
        }

        template<>
        void ReadVariable(const ConfigVariable &variable, float &value) const
        {
            value = ReadFloatVariable(variable);
        }

        template<>
        void ReadVariable(const ConfigVariable &variable, std::string &value) const
        {
            value = ReadStringVariable(variable);
        }

        // Type-specific read functions that read a specific config variable's value
        // from the internal table of config variables.
        int ReadIntVariable(const ConfigVariable &variable) const;
        bool ReadBoolVariable(const ConfigVariable &variable) const;
        float ReadFloatVariable(const ConfigVariable &variable) const;
        std::string ReadStringVariable(const ConfigVariable &variable) const;
};

} // namespace Qi
