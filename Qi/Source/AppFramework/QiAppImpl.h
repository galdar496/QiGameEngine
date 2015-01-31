//
//  QiAppImpl.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

///
/// Base class to provide an application framework for creating
/// games with the Qi Game Engine. Inherit from this class
/// and override its members to easily drive the engine.
///

#include "QiEngineConfig.h"

class QiAppImpl
{
    public:
    
        ///
        /// Setup a configuration object for engine config.
        /// This function is called before the engine is initialized.
        ///
        virtual void configure(QiEngineConfig &config) {}
    
        ///
        /// Initialize the application. This function is called
        /// after the engine is initialized.
        /// @return Initialization success.
        ///
        virtual bool init() { return true; }
    
        ///
        /// Deinitialize the application. This function is called
        /// before the engine is deinitialized.
        ///
        virtual void deinit() {}
    
        ///
        /// Run the application. This function is called after the engine
        /// has updated all systems and started rendering.
        /// @return If false, the engine will be shutdown and the application
        /// terminated.
        ///
        virtual bool run() { return true; }
};
