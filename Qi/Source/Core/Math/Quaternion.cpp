//
//  Quaternion.cpp
//  Qi Game Engine
//
//  Created by Cody White on 6/26/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Quaternion.h"

namespace Qi
{

QI_REFLECT_CLASS(Quaternion)
{
    QI_REFLECT_MEMBER(q);
}

Quaternion::Quaternion()
{
    SetIdentity();
}

} // namespace Qi
