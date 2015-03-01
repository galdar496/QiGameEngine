//
//  main.cpp
//  QiTest
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>
#include "../../Source/Core/Utility/Logger.h"
#include "../../Source/Core/Memory/MemoryAllocator.h"

int main(int argc, const char * argv[])
{
    ::testing::InitGoogleTest(&argc, (char **)argv);
    
    // Make sure that the logger and memory allocator has been initialized for
    // any tests that might use it.
    bool ready = Qi::MemoryAllocator::getInstance().init();
    ready &= Qi::Logger::getInstance().init(true);
    assert(ready);
    
    int result = RUN_ALL_TESTS();
    
    Qi::MemoryAllocator::getInstance().deinit();
    Qi::Logger::getInstance().deinit();
    return result;
}
