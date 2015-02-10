//
//  QiAppImpl.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Base class to provide an application framework for creating
/// games with the Qi Game Engine. Inherit from this class
/// and override its members to easily drive the engine.
///

#include "../Engine/EngineConfig.h"
#include <vector>

// Forward declarations.
namespace Qi
{
    class SystemBase;
} // namespace Qi

class QiAppImpl
{
    public:
    
        QiAppImpl() {}
        virtual ~QiAppImpl() {}
    
        ///
        /// Setup a configuration object for engine config.
        /// This function is called before the engine is initialized.
        ///
        virtual void configure(Qi::EngineConfig &config) {}
    
        ///
        /// Initialize the application. This function is called
        /// after the engine is initialized.
        /// @return Initialization success.
        ///
        virtual bool init() { return true; }
    
        ///
        /// Add any custom systems to the engine. The system must conform
        /// to the contract specified in SystemBase.h and should be already
        /// initialized. The engine will manage the system from this point,
        /// including its memory.
        /// @param systems A list of systems to populate. Default is empty.
        ///
        virtual void addCustomSystems(std::vector<Qi::SystemBase *> systems) {}
    
        ///
        /// Deinitialize the application. This function is called
        /// before the engine is deinitialized.
        ///
        virtual void deinit() {}
    
        ///
        /// Run the application. This function is called after the engine
        /// has updated all systems and started rendering.
        /// @param dt Delta time since the last call to "run()".
        /// @return If false, the engine will be shutdown and the application
        /// terminated.
        ///
        virtual bool run(const float dt) { return true; }
};
