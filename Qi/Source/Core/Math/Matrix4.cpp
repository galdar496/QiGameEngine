//
//  Matrix.cpp
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#include "Matrix4.h"
#include <smmintrin.h>

namespace Qi
{

Matrix4::Matrix4()
{
    m_rows[0].zero();
    m_rows[1].zero();
    m_rows[2].zero();
    m_rows[3].zero();
    
    m_rows[0].x = 1.0f;
    m_rows[1].y = 1.0f;
    m_rows[2].z = 1.0f;
    m_rows[3].w = 1.0f;
}

Matrix4::~Matrix4()
{
}

Matrix4::Matrix4(const Matrix4 &other)
{
    m_rows[0] = other.m_rows[0];
    m_rows[1] = other.m_rows[1];
    m_rows[2] = other.m_rows[2];
    m_rows[3] = other.m_rows[3];
}

Matrix4::Matrix4(float x0, float y0, float z0, float w0,
                float x1, float y1, float z1, float w1,
                float x2, float y2, float z2, float w2,
                float x3, float y3, float z3, float w3)
{
    m_rows[0].x = x0; m_rows[0].y = y0; m_rows[0].z = z0; m_rows[0].w = w0;
    m_rows[1].x = x1; m_rows[1].y = y1; m_rows[1].z = z1; m_rows[1].w = w1;
    m_rows[2].x = x2; m_rows[2].y = y2; m_rows[2].z = z2; m_rows[2].w = w2;
    m_rows[3].x = x3; m_rows[3].y = y3; m_rows[3].z = z3; m_rows[3].w = w3;
}

Matrix4::Matrix4(const Vec4 &row1, const Vec4 &row2, const Vec4 &row3, const Vec4 &row4)
{
    m_rows[0] = row1;
    m_rows[1] = row2;
    m_rows[2] = row3;
    m_rows[3] = row4;
}

Vec4 Matrix4::transform(const Vec4 &v) const
{
    Vec4 result;
    result.x = m_rows[0].dot(v);
    result.y = m_rows[1].dot(v);
    result.z = m_rows[2].dot(v);
    result.w = m_rows[3].dot(v);
    
    return result;
}

void Matrix4::transpose()
{
    _MM_TRANSPOSE4_PS(m_rows[0].mm_value,
                      m_rows[1].mm_value,
                      m_rows[2].mm_value,
                      m_rows[3].mm_value);
}

void Matrix4::preMultiply(const Matrix4 &other)
{
    // First transpose this matrix so we can do easy SIMD multiplication.
    Matrix4 t(*this);
    t.transpose();
    
    for (int ii = 0; ii < 4; ++ii)
    {
        for (int jj = 0; jj < 4; ++jj)
        {
            m_rows[ii].v[jj] = other.m_rows[ii].dot(t.m_rows[jj]);
        }
    }
}

void Matrix4::postMultiply(const Matrix4 &other)
{
    Matrix4 t(other);
    t.transpose();
    
    for (int ii = 0; ii < 4; ++ii)
    {
        Vec4 tmp(m_rows[ii]);
        for (int jj = 0; jj < 4; ++jj)
        {
            m_rows[ii].v[jj] = tmp.dot(t.m_rows[jj]);
        }
    }
}

float Matrix4::operator()(int row, int col) const
{
    return m_rows[row].v[col];
}

float &Matrix4::operator()(int row, int col)
{
    return m_rows[row].v[col];
}

void Matrix4::getRowMajor(Matrix4 &row_major) const
{
    // Internal matrix is already in row-major, just copy
    // the rows directly.
    
    row_major.m_rows[0] = m_rows[0];
    row_major.m_rows[1] = m_rows[1];
    row_major.m_rows[2] = m_rows[2];
    row_major.m_rows[3] = m_rows[3];
}

void Matrix4::getColumnMajor(Matrix4 &column_major) const
{
    // Internal matrix is in row-major, copy the matrix directly
    // into 'column_major' and transpose it to get the final result.
    
    column_major.m_rows[0] = m_rows[0];
    column_major.m_rows[1] = m_rows[1];
    column_major.m_rows[2] = m_rows[2];
    column_major.m_rows[3] = m_rows[3];
    
    column_major.transpose();
}
    
} // namespace Qi