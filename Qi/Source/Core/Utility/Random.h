//
//  Random.h
//  Qi Game Engine
//
//  Random value generator.
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <random>

namespace Qi
{

/**
  * Handles generation of random values.
  */
class Random
{
    public:
    
        /**
          * Default constructor.
          */
        Random();
    
        /**
          * Generate a random integer value.
          * @param min Minimum value that can be generated.
          * @param max Maximum value that can be generated.
          */
        int getRand(int min, int max);
    
        /**
         * Generate a random float value.
         * @param min Minimum value that can be generated.
         * @param max Maximum value that can be generated.
         */
        float getRand(float min, float max);
    
    private:
    
        // Using a normal distribution.
        using Distribution = std::uniform_int_distribution<int>;
        Distribution m_distribution;
        std::default_random_engine m_random_engine; // Random number generation engine.
};

} // namespace Qi
