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
    m_rows[0].Zero();
    m_rows[1].Zero();
    m_rows[2].Zero();
    m_rows[3].Zero();
    
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

Vec4 Matrix4::Transform(const Vec4 &v) const
{
    Vec4 result;
    result.x = m_rows[0].Dot(v);
    result.y = m_rows[1].Dot(v);
    result.z = m_rows[2].Dot(v);
    result.w = m_rows[3].Dot(v);
    
    return result;
}

void Matrix4::Transpose()
{
    _MM_TRANSPOSE4_PS(m_rows[0].m_sseValue,
                      m_rows[1].m_sseValue,
                      m_rows[2].m_sseValue,
                      m_rows[3].m_sseValue);
}

void Matrix4::PreMultiply(const Matrix4 &other)
{
    // First transpose this matrix so we can do easy SIMD multiplication.
    Matrix4 t(*this);
    t.Transpose();
    
    for (int ii = 0; ii < 4; ++ii)
    {
        for (int jj = 0; jj < 4; ++jj)
        {
            m_rows[ii].v[jj] = other.m_rows[ii].Dot(t.m_rows[jj]);
        }
    }
}

void Matrix4::PostMultiply(const Matrix4 &other)
{
    Matrix4 t(other);
    t.Transpose();
    
    for (int ii = 0; ii < 4; ++ii)
    {
        Vec4 tmp(m_rows[ii]);
        for (int jj = 0; jj < 4; ++jj)
        {
            m_rows[ii].v[jj] = tmp.Dot(t.m_rows[jj]);
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

void Matrix4::GetRowMajor(Matrix4 &rowMajor) const
{
    // Internal matrix is already in row-major, just copy
    // the rows directly.
    
    rowMajor.m_rows[0] = m_rows[0];
    rowMajor.m_rows[1] = m_rows[1];
    rowMajor.m_rows[2] = m_rows[2];
    rowMajor.m_rows[3] = m_rows[3];
}

void Matrix4::GetColumnMajor(Matrix4 &columnMajor) const
{
    // Internal matrix is in row-major, copy the matrix directly
    // into 'column_major' and transpose it to get the final result.
    
    columnMajor.m_rows[0] = m_rows[0];
    columnMajor.m_rows[1] = m_rows[1];
    columnMajor.m_rows[2] = m_rows[2];
    columnMajor.m_rows[3] = m_rows[3];
    
    columnMajor.Transpose();
}
    
} // namespace Qi