//
//  main.cpp
//  QiExample
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/Managers/System/SystemManager.h"

#include <iostream>
#include <string>

using namespace Qi;
using namespace std;

int main(int argc, const char* argv[])
{
    SystemManager engine;
    std::string config_file = "engineConfig.xml";
    if (engine.initialize(config_file))
    {
        engine.run();
        engine.shutdown();
    }
    
    return 0;
}
