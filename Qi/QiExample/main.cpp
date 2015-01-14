//
//  main.cpp
//  QiExample
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/Core/Math/Vec4.h"
#include "../Source/Core/Math/Matrix.h"

#include "../Source/Managers/System/SystemManager.h"
#include "../Source/Core/Utility/Logger.h"

#include <iostream>

using namespace Qi;
using namespace std;

int main(int argc, const char* argv[])
{
    Matrix m(1.0f, 2.0f, 3.0f, 4.0f,
             5.0f, 6.0f, 7.0f, 8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
             13.0f, 14.0f, 15.0f, 16.0f);
    
    Matrix m2(17.0f, 18.0f, 19.0f, 20.0f,
              21.0f, 22.0f, 23.0f, 24.0f,
              25.0f, 26.0f, 27.0f, 28.0f,
              29.0f, 30.0f, 31.0f, 32.0f);
    
    m.preMultiply(m2);
    
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            cout << m(r, c) << " ";
        }
        
        cout << endl;
    }
    
    SystemManager engine;
    if (engine.initialize())
    {
        engine.run();
    }
    
    const Logger::MessageBuffer &messages = Logger::getInstance().getAllMessages(Logger::Channel::kInfo);
    for (size_t ii = 0; ii < messages.size(); ++ii)
    {
        cout << messages[ii] << endl;
    }
    
    return 0;
}
