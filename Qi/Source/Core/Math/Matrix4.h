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

class __attribute__ ((aligned(16))) Matrix4
{
    public:
    
        ///
        /// Initializes the matrix to the identity matrix.
        ///
        Matrix4();
    
        ~Matrix4();
        Matrix4(const Matrix4 &other);
    
        ///
        /// Component-initialized constructor. Note that
        /// the matrix is row-major.
        ///
        Matrix4(float x0, float y0, float z0, float w0,
                float x1, float y1, float z1, float w1,
                float x2, float y2, float z2, float w2,
                float x3, float y3, float z3, float w3);
    
        ///
        /// Initialize the matrix with vectors.
        ///
        Matrix4(const Vec4 &row1, const Vec4 &row2, const Vec4 &row3, const Vec4 &row4);
    
        ///
        /// Transform a vector by this matrix.
        /// @param v Vector to transform.
        /// @return The transformed vector.
        ///
        Vec4 Transform(const Vec4 &v) const;
    
        ///
        /// Transpose the matrix.
        ///
        void Transpose();
    
        ///
        /// Premultiply this matrix.
        ///
        void PreMultiply(const Matrix4 &other);
    
        ///
        /// Post multiply this matrix.
        ///
        void PostMultiply(const Matrix4 &other);
    
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
        /// Get a version of the matrix as row major.
        /// @param rowMajor Copy of the internal matrix ordered row-major.
        ///
        void GetRowMajor(Matrix4 &rowMajor) const;
    
        ///
        /// Get a version of the matrix as colum major.
        /// @return columnMajor Copy of the internal matrix ordered column-major.
        ///
        void GetColumnMajor(Matrix4 &column_Mjor) const;
    
        ///
        /// Definition of the matrix.
        ///
        union
        {
            float m[16];      ///< Array access to the matrix.
            Vec4 m_rows[4];   ///< Rows of the matrix.
        };
};

} // namespace Qi
