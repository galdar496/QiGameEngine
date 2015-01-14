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
#include <smmintrin.h>

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
 * Vector initialization.
 */
Matrix::Matrix(const Vec4 &row1, const Vec4 &row2, const Vec4 &row3, const Vec4 &row4)
{
    m_matrix[0] = row1;
    m_matrix[1] = row2;
    m_matrix[2] = row3;
    m_matrix[3] = row4;
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

/**
 * Transpose the matrix.
 */
void Matrix::transpose()
{
    // This code performs a two-stage interleaving process that will
    // leave the matrix transposed. For a better explaination, see
    // http://www.randombit.net/bitbashing/2009/10/08/integer_matrix_transpose_in_sse2.html
    
    __m128 t0 = _mm_unpacklo_ps(m_matrix[0].mm_value, m_matrix[1].mm_value);
    __m128 t1 = _mm_unpacklo_ps(m_matrix[2].mm_value, m_matrix[3].mm_value);
    __m128 t2 = _mm_unpackhi_ps(m_matrix[0].mm_value, m_matrix[1].mm_value);
    __m128 t3 = _mm_unpackhi_ps(m_matrix[2].mm_value, m_matrix[3].mm_value);
    
    m_matrix[0].mm_value = _mm_movelh_ps(t0, t1);
    m_matrix[1].mm_value = _mm_movehl_ps(t1, t0);
    m_matrix[2].mm_value = _mm_movelh_ps(t2, t3);
    m_matrix[3].mm_value = _mm_movehl_ps(t3, t2);
}

/**
 * Premultiply this matrix.
 */
void Matrix::preMultiply(const Matrix &other)
{
    // First transpose this matrix so we can do easy SIMD multiplication.
    Matrix t(*this);
    t.transpose();
    
    for (int ii = 0; ii < 4; ++ii)
    {
        for (int jj = 0; jj < 4; ++jj)
        {
            m_matrix[ii].v[jj] = other.m_matrix[ii].dot(t.m_matrix[jj]);
        }
    }
}

/**
 * Post multiply this matrix.
 */
void Matrix::postMultiply(const Matrix &other)
{
    Matrix t(other);
    t.transpose();
    
    for (int ii = 0; ii < 4; ++ii)
    {
        Vec4 tmp(m_matrix[ii]);
        for (int jj = 0; jj < 4; ++jj)
        {
            m_matrix[ii].v[jj] = tmp.dot(t.m_matrix[jj]);
        }
    }
}

/**
 * Access the matrix by row and column index.
 */
float Matrix::operator()(int row, int col) const
{
    return m_matrix[row].v[col];
}

/**
 * Access the matrix by reference.
 */
float &Matrix::operator()(int row, int col)
{
    return m_matrix[row].v[col];
}
    
} // namespace Qi