//
//  Random.cpp
//  Qi Game Engine
//
//  Random value generator.
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Random.h"
#include <assert.h>

namespace Qi
{

//    static std::default_random_engine randomEngine;
//    using Distribution = std::uniform_int_distribution<int>;
//    static Distribution rand;
//    return rand(randomEngine, Distribution::param_type(nMin, nMax));

/**
  * Default constructor.
  */
Random::Random()
{
}

/**
 * Generate a random integer value.
 * @param min Minimum value that can be generated.
 * @param max Maximum value that can be generated.
 */
int Random::getRand(int min, int max)
{
    return m_distribution(m_random_engine, Distribution::param_type(min, max));
}

/**
 * Generate a random float value.
 * @param min Minimum value that can be generated.
 * @param max Maximum value that can be generated.
 */
float Random::getRand(float min, float max)
{
    int random = getRand(0, RAND_MAX);
    float tmp = static_cast<float>(static_cast<float>(random) / RAND_MAX);
    float diff = max - min;
    return min + (tmp * diff);
}

} // namespace Qi