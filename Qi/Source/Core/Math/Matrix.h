//
//  Matrix.h
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "Vec4.h"

namespace Qi
{

///
/// 4x4 matrix that is stored in row-major order.
///
class __attribute__ ((aligned(16))) Matrix
{
    public:
    
        ///
        /// Initializes the matrix to the identity matrix.
        ///
        Matrix();
    
        ~Matrix();
        Matrix(const Matrix &other);
    
        ///
        /// Component-initialized constructor. Note that
        /// the matrix is row-major.
        ///
        Matrix(float x0, float y0, float z0, float w0,
               float x1, float y1, float z1, float w1,
               float x2, float y2, float z2, float w2,
               float x3, float y3, float z3, float w3);
    
        ///
        /// Initialize the matrix with vectors.
        ///
        Matrix(const Vec4 &row1, const Vec4 &row2, const Vec4 &row3, const Vec4 &row4);
    
        ///
        /// Transform a vector by this matrix.
        /// @param v Vector to transform.
        /// @return The transformed vector.
        ///
        Vec4 transform(const Vec4 &v) const;
    
        ///
        /// Transpose the matrix.
        ///
        void transpose();
    
        ///
        /// Premultiply this matrix.
        ///
        void preMultiply(const Matrix &other);
    
        ///
        /// Post multiply this matrix.
        ///
        void postMultiply(const Matrix &other);
    
        ///
        /// Access the matrix by row and column.
        ///
        float operator()(int row, int col) const;
    
        ///
        /// Access the matrix by row and column.
        /// @return Reference to the specified location.
        ///
        float &operator()(int row, int col);
    
        ///
        /// Definition of the matrix.
        ///
        union
        {
            float m[16];      ///< Array access to the matrix.
            Vec4 m_matrix[4]; ///< Internal matrix object.
        };
};

} // namespace Qi
