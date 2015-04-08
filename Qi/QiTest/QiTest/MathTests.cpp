//
//  MathTests.cpp
//  QiTest
//
//  Created by Cody White on 2/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../Source/Core/Math/Vec4.h"
#include "../../Source/Core/Math/Matrix4.h"
#include "../../Source/Core/Math/Quaternion.h"

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
    
    EXPECT_EQ(70, v1.Dot(v2));
}

TEST(VectorTests, Cross)
{
    Vec4 v1(0.2f, 0.01f, 1.0f);
    Vec4 v2(4.0f, 0.12f, 3.4f);
    
    Vec4 cross = v1.Cross(v2);
    EXPECT_NEAR(-0.086f, cross.x, 0.01f);
    EXPECT_NEAR(3.32f, cross.y, 0.01f);
    EXPECT_NEAR(-0.016f, cross.z, 0.01f);
}

TEST(VectorTests, Normalize)
{
    Vec4 v(1, 2, 3, 4);
    v.Normalize();
    EXPECT_NEAR(0.182556152343f, v.x, 0.001f);
    EXPECT_NEAR(0.365112304687f, v.y, 0.001f);
    EXPECT_NEAR(0.547668457031f, v.z, 0.001f);
    EXPECT_NEAR(0.730224609375f, v.w, 0.001f);
    EXPECT_NEAR(1.0f, v.Length(), 0.001f);
    
//    v = Vec4(1, 2, 3, 4);
//    v.NormalizeAccurate();
//    EXPECT_NEAR(0.182556152343f, v.x, 0.001f);
//    EXPECT_NEAR(0.365112304687f, v.y, 0.001f);
//    EXPECT_NEAR(0.547668457031f, v.z, 0.001f);
//    EXPECT_NEAR(0.730224609375f, v.w, 0.001f);
//    EXPECT_NEAR(1.0f, v.Length(), 0.001f);
}

TEST(MatrixTests, CreateIdentity)
{
    Matrix4 m;
    
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
    Matrix4 m(1,   2,  3,  4,
              5,   6,  7,  8,
              9,  10, 11, 12,
              13, 14, 15, 16);
    
    m.Transpose();
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
    Matrix4 m1(1,   2,  3,  4,
               5,   6,  7,  8,
               9,  10, 11, 12,
               13, 14, 15, 16);
    
    Matrix4 m2(11,   12,  13,  14,
               15,   16,  17,  18,
               19,  110, 111, 112,
               113, 114, 115, 116);
    
    m1.PreMultiply(m2);
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
    Matrix4 m1(1,   2,  3,  4,
               5,   6,  7,  8,
               9,  10, 11, 12,
               13, 14, 15, 16);
    
    Matrix4 m2(11,   12,  13,  14,
               15,   16,  17,  18,
               19,  110, 111, 112,
               113, 114, 115, 116);
    
    m1.PostMultiply(m2);
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
    Matrix4 m(1,   2,  3,  4,
              5,   6,  7,  8,
              9,  10, 11, 12,
              13, 14, 15, 16);
    
    Vec4 v(1, 2, 3, 4);
    
    v = m.Transform(v);
    EXPECT_EQ(30,  v.x);
    EXPECT_EQ(70,  v.y);
    EXPECT_EQ(110, v.z);
    EXPECT_EQ(150, v.w);
}

TEST(QuaternionTests, CreateFromAxisAngle)
{
    Quaternion q;
    q.CreateFromAxisAngle(Vec4(1.0f, 0.0f, 0.0f), 2.15f);
    
    EXPECT_NEAR(0.8795901504337889f, q.x, 0.01f);
    EXPECT_NEAR(0.0f, q.y, 0.01f);
    EXPECT_NEAR(0.0f, q.z, 0.01f);
    EXPECT_NEAR(0.4757322432417888f, q.w, 0.01f);
}

TEST(QuaternionTests, CreateFromEuler)
{
    Quaternion q;
    q.CreateFromEuler(2.15f, 0.0f, 0.0f);
    
    EXPECT_NEAR(0.8795901504337889f, q.x, 0.01f);
    EXPECT_NEAR(0.0f, q.y, 0.01f);
    EXPECT_NEAR(0.0f, q.z, 0.01f);
    EXPECT_NEAR(0.4757322432417888f, q.w, 0.01f);
}

TEST(QuaternionTests, ToMatrix)
{
    Quaternion q;
    q.CreateFromAxisAngle(Vec4(1.0f, 0.0f, 0.0f), 2.15f);
    
    Matrix4 m;
    q.ToMatrix(m);
    
    EXPECT_NEAR(1.0f, m(0, 0), 0.01f);
    EXPECT_NEAR(0.0f, m(1, 0), 0.01f);
    EXPECT_NEAR(0.0f, m(2, 0), 0.01f);
    
    EXPECT_NEAR(0.0f,  m(0, 1), 0.01f);
    EXPECT_NEAR(-0.547357665480271f, m(1, 1), 0.01f);
    EXPECT_NEAR(0.8368987907984977, m(2, 1), 0.01f);
    
    EXPECT_NEAR(0.0f,  m(0, 2), 0.01f);
    EXPECT_NEAR(-0.8368987907984977f, m(1, 2), 0.01f);
    EXPECT_NEAR(-0.547357665480271f, m(2, 2), 0.01f);
}

TEST(QuaternionTests, Slerp)
{
    Quaternion q1(Vec4(1.0, 0.0, 0.0), 0.2f);
    Quaternion q2(Vec4(0.0, 1.0, 0.0), 1.2f);
    
    Quaternion s;
    s.Slerp(q1, q2, 0.3f);
    
    EXPECT_NEAR(0.8256935895660071f, s.x, 0.01f);
    EXPECT_NEAR(0.3997466112926573f, s.y, 0.01f);
    EXPECT_NEAR(0.0000000000000000f, s.z, 0.01f);
    EXPECT_NEAR(0.6448346514643902f, s.w, 0.01f);
}

TEST(QuaternionTests, Multiply)
{
    Quaternion q1(Vec4(3.0f, 2.0f, 0.4f), 1.0f);
    Quaternion q2(Vec4(1.0f, 12.0f, 0.34f), 0.2f);
    
    Quaternion q3 = q1 * q2;
    EXPECT_NEAR(-2.5200000000000005f, q3.x, 0.01f);
    EXPECT_NEAR(11.780000000000001f, q3.y, 0.01f);
    EXPECT_NEAR(34.42f, q3.z, 0.01f);
    EXPECT_NEAR(-26.936f, q3.w, 0.01f);
}

TEST(QuaternionTests, RotateVector)
{
    Quaternion q(Vec4(1.0, 0.0, 0.0), 0.2f);
    q.Normalize();
    
    Vec4 v(0.25f, 0.35f, -0.45f);
    Vec4 rotated = q.Rotate(v);
    
    EXPECT_NEAR(0.25f, rotated.x, 0.01f);
    EXPECT_NEAR(-0.15000003576278687f, rotated.y, 0.01f);
    EXPECT_NEAR(0.54999995231628418f, rotated.z, 0.01f);
    EXPECT_NEAR(1.0f, rotated.w, 0.01f);
}




