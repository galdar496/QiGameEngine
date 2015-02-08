//
//  main.cpp
//  QiTest
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

int main(int argc, const char * argv[])
{
    ::testing::InitGoogleTest(&argc, (char **)argv);
    return RUN_ALL_TESTS();
}
