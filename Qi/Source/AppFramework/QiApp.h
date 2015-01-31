//
//  QiApp.h
//  Qi Game Engine
//
//  Created by Cody White on 1/31/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

///
/// Application driver for all Qi apps. Invoke the macro QI_IMPLEMENT_APP
/// to define a main function which will properly initialize
/// the engine and invoke your custom QiAppImpl.
///

#include "QiAppImpl.h"

// Forward declarations.
namespace Qi
{
    class Engine;
}

class QiApp
{
    public:
    
        explicit QiApp(QiAppImpl *app);
        ~QiApp();
    
        ///
        /// Initialize the Engine for use.
        /// @return Initialization status.
        ///
        bool init();
    
        ///
        /// Shutdown the engine.
        ///
        void deinit();
    
        ///
        /// Execute the QiAppImpl. This includes initialization
        /// and running of the app.
        ///
        void execute();
    
    private:
    
        Qi::Engine *m_engine;  ///< Game engine object.
        QiAppImpl *m_app;      ///< Customized QiAppImpl object. All custom game logic goes here.
};

#define QI_IMPLEMENT_APP(AppName)           \
    int main(int argc, const char* argv[])  \
    {                                       \
        QiApp app(new AppName());           \
        app.init();                         \
        app.execute();                      \
        app.deinit();                       \
        return 0;                           \
    }

