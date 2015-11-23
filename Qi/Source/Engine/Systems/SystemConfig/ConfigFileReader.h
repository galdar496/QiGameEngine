//
//  ConfigFileReader.h
//  Qi Game Engine
//
//  Created by Cody White on 7/27/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Abstracts the reading of the engine config file.
///

#include "ConfigVariables.h"
#include "../../../Core/Defines.h"
#include "../../../ThirdParty/tinyxml2.h"

namespace Qi
{

class ConfigFileReader
{
    public:

        explicit ConfigFileReader(const tinyxml2::XMLElement *configNode) :
            m_configNode(configNode)
        {}

        ///
        /// Read the config file and return the requested value. If the config variable does not
        /// exist in the config file, then the variable's default value will be used.
        ///
        /// @param variable ConfigVariable to read.
        /// @return Read config value or variable default.
        ///
        template<class T>
        T GetVariableValue(const Variable &variable) const
        {
            T returnValue;

            const tinyxml2::XMLElement *element = m_configNode->FirstChildElement(variable.name.c_str());
            if (element)
            {
                ReadVariable<T>(element, returnValue);
            }
            else
            {
                //returnValue = (T)variable.value;
            }

            return returnValue;
        }

    private:

        template<class T>
        void ReadVariable(const tinyxml2::XMLElement *element, T &value) const
        {
            QI_ASSERT(0 && "Unimplemented ConfigVariable type");
        }

        template<>
        void ReadVariable(const tinyxml2::XMLElement *element, int &value) const
        {
            element->QueryIntText(&value);
            Qi_LogInfo("Config variable %s: %d", element->Name(), value);
        }

        template<>
        void ReadVariable(const tinyxml2::XMLElement *element, float &value) const
        {
            element->QueryFloatText(&value);
            Qi_LogInfo("Config variable %s: %f", element->Name(), value);
        }

        template<>
        void ReadVariable(const tinyxml2::XMLElement *element, bool &value) const
        {
            element->QueryBoolText(&value);
            Qi_LogInfo("Config variable %s: %s", element->Name(), value ? "true" : "false");
        }

        template<>
        void ReadVariable(const tinyxml2::XMLElement *element, std::string &value) const
        {
            value = element->GetText();
            Qi_LogInfo("Config variable %s: %s", element->Name(), value.c_str());
        }

        const tinyxml2::XMLElement *m_configNode; ///< Root config node for a group of options. Should be ready by a call to SystemBase::GetConfigNodeName().
};

} // namespace Qi
