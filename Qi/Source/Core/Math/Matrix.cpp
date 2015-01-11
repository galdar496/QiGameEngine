//
//  Matrix.cpp
//  Qi Game Engine
//
//  4x4 float matrix
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Matrix.h"

namespace Qi
{

/**
 * Default constructor. Initializes the matrix to identity.
 */
Matrix::Matrix()
{
    m_matrix[0].zero();
    m_matrix[1].zero();
    m_matrix[2].zero();
    m_matrix[3].zero();
    
    m_matrix[0].x = 1.0f;
    m_matrix[1].y = 1.0f;
    m_matrix[2].z = 1.0f;
    m_matrix[3].w = 1.0f;
}

/**
 * Destructor.
 */
Matrix::~Matrix()
{
}

/**
 * Copy constructor.
 */
Matrix::Matrix(const Matrix &other)
{
    m_matrix[0] = other.m_matrix[0];
    m_matrix[1] = other.m_matrix[1];
    m_matrix[2] = other.m_matrix[2];
    m_matrix[3] = other.m_matrix[3];
}

/**
 * Component-initialized constructor.
 */
Matrix::Matrix(float x0, float y0, float z0, float w0,
               float x1, float y1, float z1, float w1,
               float x2, float y2, float z2, float w2,
               float x3, float y3, float z3, float w3)
{
    m_matrix[0].x = x0; m_matrix[0].y = y0; m_matrix[0].z = z0; m_matrix[0].w = w0;
    m_matrix[1].x = x1; m_matrix[1].y = y1; m_matrix[1].z = z1; m_matrix[1].w = w1;
    m_matrix[2].x = x2; m_matrix[2].y = y2; m_matrix[2].z = z2; m_matrix[2].w = w2;
    m_matrix[3].x = x3; m_matrix[3].y = y3; m_matrix[3].z = z3; m_matrix[3].w = w3;
}

/**
 * Transform a vector by this matrix. Assumes a right-handed coordinate system.
 */
Vec4 Matrix::transform(const Vec4 &v) const
{
    Vec4 result;
    result.x = m_matrix[0].dot(v);
    result.y = m_matrix[1].dot(v);
    result.z = m_matrix[2].dot(v);
    result.w = m_matrix[3].dot(v);
    
    return result;
}
    
} // namespace Qi