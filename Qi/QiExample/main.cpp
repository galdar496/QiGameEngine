//
//  main.cpp
//  QiExample
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../Source/Core/Math/Vec4.h"

#include <iostream>

using namespace Qi;
using namespace std;

int main(int argc, const char* argv[])
{
    Vec4 vec1(2.0f, 1.0f, 1.0f, 1.0f);
    Vec4 vec2(2.0f, 2.0f, 2.0f, 2.0f);
    
    float dot = vec1.dot(vec2);
    
    cout << dot << endl;
    
    return 0;
}
