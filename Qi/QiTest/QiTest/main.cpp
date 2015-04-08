//
//  main.cpp
//  QiTest
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "../../Source/Core/Utility/Logger/Logger.h"
#include "../../Source/Core/Memory/MemoryAllocator.h"
#include <gtest/gtest.h>
#include <iostream>

int main(int argc, const char * argv[])
{
    ::testing::InitGoogleTest(&argc, (char **)argv);
    
    // Make sure that the logger and memory allocator has been initialized for
    // any tests that might use it.
    bool ready = Qi::MemoryAllocator::GetInstance().Init().IsValid();
    ready &= Qi::Logger::GetInstance().Init(Qi::Logger::LogFileType::kHTML, true).IsValid();
    QI_ASSERT(ready);
    
    int result = RUN_ALL_TESTS();
    
    Qi::MemoryAllocator::GetInstance().Deinit();
    Qi::Logger::GetInstance().Deinit();

#ifdef QI_WINDOWS
	int x;
	std::cin >> x; // pause when running from Visual Studio.
#endif

    return result;
}
