//
//  ManagerBase.h
//  Qi Game Engine
//
//  Created by Cody White on 1/22/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Base class which all managers must inherit from. This class provides a
/// contract which all managers must obey.
///

namespace Qi
{

class ManagerBase
{
    public:
    
        ///
        /// Initialize the manager for use.
        /// @return If the manager was properly initialized or not.
        ///
        virtual bool initialize() = 0;
    
        ///
        /// Shutdown the manager. At this point, nothing should be allocated
        /// by the manager and it should return to a pre-initialized state.
        ///
        virtual void shutdown() = 0;
    
        ///
        /// Update this manager (called once per-frame). The manager is free
        /// to update any of its subsystems as it sees fit.
        /// @param dt Time in seconds since the last call to update() was made.
        ///
        virtual void update(const float dt) = 0;
};

} // namespace Qi


