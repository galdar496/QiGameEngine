//
//  Vec4.h
//  Qi Game Engine
//
//  Stores the information for a 4 component vector. When possible,
//  the vector will use SSE instructions.
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <smmintrin.h>

namespace Qi
{

/**
  * Contains a 4 component vector and all math operations
  */
class Vec4
{
    public:
    
        /**
          * Default constructor.
          */
        Vec4()
        {
            zero();
        }
    
        /**
          * Constructor with per-component initialization.
          */
        Vec4(float x, float y, float z, float w)
        {
            m_vector.x = x;
            m_vector.y = y;
            m_vector.z = z;
            m_vector.w = w;
        }
    
        /**
          * Copy constructor.
          */
        Vec4(const Vec4 &other)
        {
            m_vector.mm_value = other.m_vector.mm_value;
        }
    
        /**
          * Zero the vector.
          */
        inline void zero()
        {
            m_vector.mm_value = _mm_setzero_ps();
        }
    
        // Math Operations //////////////////////////////////////
    
        /** 
          * Dot product
          * @param other Vec4 object to perform the dot product with.
          * @return Scalar value denoting the dot product.
          */
        float dot(const Vec4 &other)
        {
            // Mask 0xF1 means to perform a dot product with all 4 components (0xF) and store it in the
            // lowest float value of the result (0x1).
            return _mm_cvtss_f32(_mm_dp_ps(m_vector.mm_value, other.m_vector.mm_value, 0xF1));
        }
    
        /**
          * Cross product. Note that cross product is only defined in 3 dimensions, therefore
          * the fourth component of each vector is ignored.
          * @param other Vec4 object to perform a cross product with.
          * @param result Resulting vector from the cross product.
          */
        void cross(const Vec4 &other, Vec4 &result)
        {
            result.m_vector.mm_value = _mm_sub_ps(
                _mm_mul_ps(_mm_shuffle_ps(m_vector.mm_value, m_vector.mm_value, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(other.m_vector.mm_value, other.m_vector.mm_value, _MM_SHUFFLE(3, 1, 0, 2))),
                _mm_mul_ps(_mm_shuffle_ps(m_vector.mm_value, m_vector.mm_value, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(other.m_vector.mm_value, other.m_vector.mm_value, _MM_SHUFFLE(3, 0, 2, 1)))
                                                 );
        }
    
    private:
    
        // Definition of the vector.
        union
        {
            struct {float x, y, z, w; }; // Simple access to simd vector.
            __m128 mm_value;    // SIMD version of the vector.
        } m_vector;  // Internal vector storage.
};

} // Namespace Qi
