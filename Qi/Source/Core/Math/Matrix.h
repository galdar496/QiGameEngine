//
//  Matrix.h
//  Qi Game Engine
//
//  4x4 float matrix
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "Vec4.h"

namespace Qi
{

/**
  * 4x4 matrix class. Matrices are stored as row-major.
  */
class Matrix
{
    public:
    
        /**
          * Default constructor. Initializes the matrix to identity.
          */
        Matrix();
    
        /**
          * Destructor.
          */
        ~Matrix();
    
        /**
          * Copy constructor.
          */
        Matrix(const Matrix &other);
    
        /**
          * Component-initialized constructor.
          */
        Matrix(float x0, float y0, float z0, float w0,
               float x1, float y1, float z1, float w1,
               float x2, float y2, float z2, float w2,
               float x3, float y3, float z3, float w3);
    
        /**
          * Transform a vector by this matrix. Assumes a right-handed coordinate system.
          */
        Vec4 transform(const Vec4 &v) const;
        
    
        Vec4 m_matrix[4]; // Internal matrix object.
};

} // namespace Qi
