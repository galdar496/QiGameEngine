//
//  SystemBase.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Base class for all systems. This class provides a contract
/// for all systems that must be obeyed.
///

#include "SystemConfig/ConfigVariables.h"
#include "../../Core/BaseTypes.h"
#include "../../Core/Defines.h"
#include "../../Core/Reflection/Reflection.h"
#include <string>

namespace Qi
{

// Forward declaration.
class Engine;

class SystemBase
{
    public:
    
        QI_DECLARE_REFLECTED_CLASS(SystemBase);
    
        SystemBase();
        SystemBase(const std::string &systemName);
        virtual ~SystemBase();

        ///
        /// Initialization struct to be passed to the Init() function.
        ///
        struct CInfo
        {
            const ConfigVariables *configVariables; // List of all config variables supplied by the user.
            const Engine          *engine;          // Reference to the main engine object.
        };
    
        ///
        /// Initialize the system for use.
        ///
        /// @param cinfo Configuration object to use to setup the system.
        /// @return If the system was properly initialized or not.
        ///
        virtual Result Init(const CInfo &cinfo);
        
        ///
        /// Deinitialize the system. At this point, nothing should be allocated
        /// by the system and it should return to a pre-initialized state.
        ///
        virtual void Deinit();
        
        ///
        /// Update this system. The system is free
        /// to update any of its subsystems as it sees fit.
        ///
        /// @param dt Time in seconds since the last call to update() was made.
        ///
        virtual void Update(const float dt);
    
        ///
        /// Get the name of the system. This is mostly used for logging purposes.
        ///
        /// @return Name given to this system.
        ///
        const std::string GetName() const;
    
    protected:
    
        bool m_initialized; ///< If this class has been initialized, set this value to true.

        const std::string m_systemName;     ///< Name of this system.
};

} // namespace Qi
