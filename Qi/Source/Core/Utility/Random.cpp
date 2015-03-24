//
//  Random.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Random.h"
#include "../../Defines.h"

namespace Qi
{

Random::Random()
{
}

Random::~Random()
{
}

int Random::GetRand(int min, int max)
{
    QI_ASSERT(min < max && "min is greater than max");
    return m_distribution(m_randomEngine, Distribution::param_type(min, max));
}

float Random::GetRand(float min, float max)
{
    QI_ASSERT(min < max && "min is greater than max");
    int random = GetRand(0, RAND_MAX);
    float tmp = static_cast<float>(static_cast<float>(random) / RAND_MAX);
    float diff = max - min;
    return min + (tmp * diff);
}

} // namespace Qi