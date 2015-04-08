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

#include "../Core/BaseTypes.h"
#include <string>

namespace Qi
{

class SystemBase
{
    public:
    
        virtual ~SystemBase() {}
    
        ///
        /// Initialize the system for use.
        /// @return If the system was properly initialized or not.
        ///
        virtual Result Init() = 0;
        
        ///
        /// Deinitialize the system. At this point, nothing should be allocated
        /// by the system and it should return to a pre-initialized state.
        ///
        virtual void Deinit() = 0;
        
        ///
        /// Update this system. The system is free
        /// to update any of its subsystems as it sees fit.
        /// @param dt Time in seconds since the last call to update() was made.
        ///
        virtual void Update(const float dt) = 0;
    
        ///
        /// Get the name of the system. This is mostly used for logging purposes.
        /// @return Name given to this system.
        ///
        virtual std::string GetName() const = 0;
};

} // namespace Qi
