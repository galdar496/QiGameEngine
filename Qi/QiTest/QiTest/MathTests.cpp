//
//  MathTests.cpp
//  QiTest
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../Source/Core/Math/Vec4.h"
#include "../../Source/Core/Math/Matrix.h"

using namespace Qi;

TEST(VectorTests, Addition)
{
    Vec4 v1(1, 2, 3, 4);
    Vec4 v2(5, 6, 7, 8);
    
    Vec4 sum = v1 + v2;
    EXPECT_EQ(6,  sum.x);
    EXPECT_EQ(8,  sum.y);
    EXPECT_EQ(10, sum.z);
    EXPECT_EQ(12, sum.w);
}

TEST(VectorTests, Subtraction)
{
    Vec4 v1(1, 2, 3, 4);
    Vec4 v2(5, 6, 7, 8);
    
    Vec4 diff = v1 - v2;
    EXPECT_EQ(-4,  diff.x);
    EXPECT_EQ(-4,  diff.y);
    EXPECT_EQ(-4, diff.z);
    EXPECT_EQ(-4, diff.w);
}

TEST(VectorTests, Scale)
{
    Vec4 v(1, 2, 3, 4);
    v = v * 2;
    
    EXPECT_EQ(2, v.x);
    EXPECT_EQ(4, v.y);
    EXPECT_EQ(6, v.z);
    EXPECT_EQ(8, v.w);
}

TEST(VectorTests, Divide)
{
    Vec4 v(1, 2, 3, 4);
    v = v / 2;
    
    EXPECT_EQ(0.5f, v.x);
    EXPECT_EQ(1.0f, v.y);
    EXPECT_EQ(1.5f, v.z);
    EXPECT_EQ(2.0f, v.w);
}

TEST(VectorTests, DotProduct)
{
    Vec4 v1(1, 2, 3, 4);
    Vec4 v2(5, 6, 7, 8);
    
    EXPECT_EQ(70, v1.dot(v2));
}

TEST(VectorTests, Cross)
{
    Vec4 v1(1, 2, 3);
    Vec4 v2(4, 5, 6);
    
    Vec4 cross = v1.cross(v2);
    EXPECT_EQ(-3, cross.x);
    EXPECT_EQ(6, cross.y);
    EXPECT_EQ(-3, cross.z);
}

TEST(VectorTests, Normalize)
{
    Vec4 v(1, 2, 3, 4);
    v.normalize();
    EXPECT_NEAR(0.182556152343f, v.x, 0.001f);
    EXPECT_NEAR(0.365112304687f, v.y, 0.001f);
    EXPECT_NEAR(0.547668457031f, v.z, 0.001f);
    EXPECT_NEAR(0.730224609375f, v.w, 0.001f);
    EXPECT_NEAR(1.0f, v.length(), 0.001f);
    
    v = Vec4(1, 2, 3, 4);
    v.normalizeAccurate();
    EXPECT_NEAR(0.182556152343f, v.x, 0.001f);
    EXPECT_NEAR(0.365112304687f, v.y, 0.001f);
    EXPECT_NEAR(0.547668457031f, v.z, 0.001f);
    EXPECT_NEAR(0.730224609375f, v.w, 0.001f);
    EXPECT_NEAR(1.0f, v.length(), 0.001f);
}

TEST(MatrixTests, CreateIdentity)
{
    Matrix m;
    
    EXPECT_EQ(1, m(0, 0));
    EXPECT_EQ(0, m(1, 0));
    EXPECT_EQ(0, m(2, 0));
    EXPECT_EQ(0, m(3, 0));
    
    EXPECT_EQ(0, m(0, 1));
    EXPECT_EQ(1, m(1, 1));
    EXPECT_EQ(0, m(2, 1));
    EXPECT_EQ(0, m(3, 1));
    
    EXPECT_EQ(0, m(0, 2));
    EXPECT_EQ(0, m(1, 2));
    EXPECT_EQ(1, m(2, 2));
    EXPECT_EQ(0, m(3, 2));
    
    EXPECT_EQ(0, m(0, 3));
    EXPECT_EQ(0, m(1, 3));
    EXPECT_EQ(0, m(2, 3));
    EXPECT_EQ(1, m(3, 3));
}

TEST(MatrixTests, Transpose)
{
    Matrix m(1,   2,  3,  4,
             5,   6,  7,  8,
             9,  10, 11, 12,
             13, 14, 15, 16);
    
    m.transpose();
    EXPECT_EQ(1, m(0, 0));
    EXPECT_EQ(2, m(1, 0));
    EXPECT_EQ(3, m(2, 0));
    EXPECT_EQ(4, m(3, 0));
    
    EXPECT_EQ(5, m(0, 1));
    EXPECT_EQ(6, m(1, 1));
    EXPECT_EQ(7, m(2, 1));
    EXPECT_EQ(8, m(3, 1));
    
    EXPECT_EQ(9,  m(0, 2));
    EXPECT_EQ(10, m(1, 2));
    EXPECT_EQ(11, m(2, 2));
    EXPECT_EQ(12, m(3, 2));
    
    EXPECT_EQ(13, m(0, 3));
    EXPECT_EQ(14, m(1, 3));
    EXPECT_EQ(15, m(2, 3));
    EXPECT_EQ(16, m(3, 3));
}

TEST(MatrixTests, PreMultiply)
{
    Matrix m1(1,   2,  3,  4,
              5,   6,  7,  8,
              9,  10, 11, 12,
              13, 14, 15, 16);
    
    Matrix m2(11,   12,  13,  14,
              15,   16,  17,  18,
              19,  110, 111, 112,
              113, 114, 115, 116);
    
    m1.preMultiply(m2);
    EXPECT_EQ(370,  m1(0, 0));
    EXPECT_EQ(482,  m1(1, 0));
    EXPECT_EQ(3024, m1(2, 0));
    EXPECT_EQ(3226, m1(3, 0));
    
    EXPECT_EQ(420,  m1(0, 1));
    EXPECT_EQ(548,  m1(1, 1));
    EXPECT_EQ(3376, m1(2, 1));
    EXPECT_EQ(3684, m1(3, 1));
    
    EXPECT_EQ(470,  m1(0, 2));
    EXPECT_EQ(614,  m1(1, 2));
    EXPECT_EQ(3728, m1(2, 2));
    EXPECT_EQ(4142, m1(3, 2));
    
    EXPECT_EQ(520,  m1(0, 3));
    EXPECT_EQ(680,  m1(1, 3));
    EXPECT_EQ(4080, m1(2, 3));
    EXPECT_EQ(4600, m1(3, 3));
}

TEST(MatrixTests, PostMultiply)
{
    Matrix m1(1,   2,  3,  4,
              5,   6,  7,  8,
              9,  10, 11, 12,
              13, 14, 15, 16);
    
    Matrix m2(11,   12,  13,  14,
              15,   16,  17,  18,
              19,  110, 111, 112,
              113, 114, 115, 116);
    
    m1.postMultiply(m2);
    EXPECT_EQ(550,  m1(0, 0));
    EXPECT_EQ(1182, m1(1, 0));
    EXPECT_EQ(1814, m1(2, 0));
    EXPECT_EQ(2446, m1(3, 0));
    
    EXPECT_EQ(830,  m1(0, 1));
    EXPECT_EQ(1838, m1(1, 1));
    EXPECT_EQ(2846, m1(2, 1));
    EXPECT_EQ(3854, m1(3, 1));
    
    EXPECT_EQ(840,  m1(0, 2));
    EXPECT_EQ(1864, m1(1, 2));
    EXPECT_EQ(2888, m1(2, 2));
    EXPECT_EQ(3912, m1(3, 2));
    
    EXPECT_EQ(850,  m1(0, 3));
    EXPECT_EQ(1890, m1(1, 3));
    EXPECT_EQ(2930, m1(2, 3));
    EXPECT_EQ(3970, m1(3, 3));
}

TEST(MatrixTests, Transform)
{
    Matrix m(1,   2,  3,  4,
             5,   6,  7,  8,
             9,  10, 11, 12,
             13, 14, 15, 16);
    
    Vec4 v(1, 2, 3, 4);
    
    v = m.transform(v);
    EXPECT_EQ(30,  v.x);
    EXPECT_EQ(70,  v.y);
    EXPECT_EQ(110, v.z);
    EXPECT_EQ(150, v.w);
}




