//
//  Random.h
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <random>

namespace Qi
{

///
/// Handle the generation of random numbers. All random numbers will
/// be generated with a uniform distribution.
///
class Random
{
    public:
    
        Random();
        ~Random();
    
        ///
        /// Generate a random integer value.
        ///
        /// @param min Min value, must be less than 'max'.
        /// @param max Max value, must be greater than 'min'.
        ///
        int GetRand(int min, int max);
    
        ///
        /// Generate a random floating-point value.
        ///
        /// @param min Min value, must be less than 'max'.
        /// @param max Max value, must be greater than 'min'.
        ///
        float GetRand(float min, float max);
    
    private:
    
        // Using a normal distribution.
        using Distribution = std::uniform_int_distribution<int>;
        Distribution m_distribution;
        std::default_random_engine m_randomEngine; ///< Random number generation engine.
};

} // namespace Qi
