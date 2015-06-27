//
//  Vec4.cpp
//  Qi Game Engine
//
//  Created by Cody White on 6/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Vec4.h"

namespace Qi
{

QI_REFLECT_CLASS(Vec4)
{
    QI_REFLECT_MEMBER(v);
}

Vec4::Vec4()
{
    Zero();
}

} // namespace Qi
