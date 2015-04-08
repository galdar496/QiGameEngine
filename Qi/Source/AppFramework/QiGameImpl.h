//
//  QiGameImpl.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Base class to provide an application framework for creating
/// games with the Qi Game Engine. Inherit from this class
/// and override its members to easily drive a engine.
///

#include "../Engine/EngineConfig.h"
#include "../Core/Containers/Array.h"

// Forward declarations.
namespace Qi
{
    class SystemBase;
} // namespace Qi
class QiGameImpl
{
    public:
    
        QiGameImpl() {}
        virtual ~QiGameImpl() {}
    
        ///
        /// Setup a configuration object for engine config.
        /// This function is called before the engine is initialized.
        ///
        virtual void Configure(Qi::EngineConfig &config) const {}
    
        ///
        /// Initialize the game. This function is called
        /// after the engine is initialized.
        /// @return Initialization success.
        ///
        virtual bool Init() { return true; }
    
        ///
        /// Add any custom systems to the engine. The system must conform
        /// to the contract specified in SystemBase.h and should be already
        /// initialized. The engine will manage the system from this point,
        /// including its memory.
        /// @param systems A list of systems to populate. Default is empty.
        ///
        virtual void AddCustomSystems(Qi::Array<Qi::SystemBase *> &systems) const {}
    
        ///
        /// Deinitialize the game. This function is called
        /// before the engine is deinitialized.
        ///
        virtual void Deinit() {}
    
        ///
        /// Step the game. This function is called after the engine
        /// has updated all systems and started rendering.
        /// @param dt Delta time since the last call to "step()".
        /// @return If false, the engine will be shutdown and the game
        /// terminated.
        ///
        virtual bool Step(const float dt) { return true; }
};
