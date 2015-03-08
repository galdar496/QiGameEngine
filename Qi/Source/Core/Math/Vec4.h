//
//  Vec4.h
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <smmintrin.h>

namespace Qi
{

///
/// Stores the information for a 4 component vector. When possible,
/// the vector will use SSE instructions. Requires SSE 4.1.
///

class __attribute__ ((aligned(16))) Vec4
{
    public:
    
        Vec4()
        {
            Zero();
        }
    
        ~Vec4() {}
    
        Vec4(const Vec4 &other)
        {
            m_sseValue = other.m_sseValue;
        }
    
        Vec4 &operator=(const Vec4 &other)
        {
            if (this != &other)
            {
                m_sseValue = other.m_sseValue;
            }
            
            return *this;
        }
    
        ///
        /// Per-component initialization constructor.
        ///
        Vec4(float _x, float _y, float _z, float _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
    
        ///
        /// 3-component initialization. The final component
        /// is initialized to 1.
        ///
        Vec4(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
            w = 1.0f;
        }
    
        ///
        /// Zero the vector.
        ///
        inline void Zero()
        {
            m_sseValue = _mm_setzero_ps();
        }
    
        // Math Operations //////////////////////////////////////
    
        ///
        /// Operator+
        /// @param other Vector to add.
        ///
        inline Vec4 operator+(const Vec4 &other) const
        {
            return _mm_add_ps(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// Operator-
        /// @param other Vector to subtract.
        ///
        inline Vec4 operator-(const Vec4 &other) const
        {
            return _mm_sub_ps(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// Operator*
        /// @param scalar Scalar value to multiply the vector by.
        ///
        inline Vec4 operator*(float scalar) const
        {
            return _mm_mul_ps(m_sseValue, _mm_set1_ps(scalar));
        }
    
        ///
        /// Operator/
        /// @param scalar Scalar value to divide the vector by.
        ///
        inline Vec4 operator/(float scalar) const
        {
            return _mm_div_ps(m_sseValue, _mm_set1_ps(scalar));
        }
    
        ///
        /// Operator*=
        /// @param scalar Scalar value to multiply the vector by.
        ///
        inline void operator*=(float scalar)
        {
            m_sseValue = _mm_mul_ps(m_sseValue, _mm_set1_ps(scalar));
        }
    
        ///
        /// Operator/=
        /// @param scalar Scalar value to divide the vector by.
        ///
        inline void operator/=(float scalar)
        {
            m_sseValue = _mm_div_ps(m_sseValue, _mm_set1_ps(scalar));
        }
    
        ///
        /// Dot product.
        /// @paramm other Vector to perform dot product with.
        /// @return Scalar value denoting the dot product.
        ///
        inline float Dot(const Vec4 &other) const
        {
            return _mm_cvtss_f32(_mm_dp_ps(m_sseValue, other.m_sseValue, SIMDMask::AllChannels_StoreLow));
        }
    
        ///
        /// 3-component dot product.
        /// @paramm other Vector to perform dot product with.
        /// @return Scalar value denoting the dot product.
        ///
        inline float Dot3(const Vec4 &other) const
        {
            return _mm_cvtss_f32(_mm_dp_ps(m_sseValue, other.m_sseValue, SIMDMask::ThreeChannels_StoreLow));
        }
    
        ///
        /// Cross product. Note that the cross product is only defined in 3 dimensions, therefore
        /// the fourth component of each vector is ignored.
        /// @param other Vector to cross with.
        /// @return Perpendicular vector.
        ///
        inline Vec4 Cross(const Vec4 &other) const
        {
            return _mm_sub_ps(
                _mm_mul_ps(_mm_shuffle_ps(m_sseValue, m_sseValue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(other.m_sseValue, other.m_sseValue, _MM_SHUFFLE(3, 1, 0, 2))),
                _mm_mul_ps(_mm_shuffle_ps(m_sseValue, m_sseValue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(other.m_sseValue, other.m_sseValue, _MM_SHUFFLE(3, 0, 2, 1)))
                             );
        }
    
        ///
        /// Normalize the vector. The normalized version will overwrite the previous version.
        /// Note that this function is optimized for speed and may produce a normal that does not quite have a unit
        /// length (but is very close).
        ///
        inline void Normalize()
        {
            __m128 inverse = _mm_rsqrt_ps(_mm_dp_ps(m_sseValue, m_sseValue, SIMDMask::AllChannels_StoreAll));
            m_sseValue = _mm_mul_ps(m_sseValue, inverse);
        }
    
        ///
        /// Normalize the vector accurately. This differs from standard "Normalize" in that it will produce a mathematically correct
        /// normal but at a much higher cost.
        ///
        inline void NormalizeAccurate()
        {
            __m128 sqrt = _mm_sqrt_ps(_mm_dp_ps(m_sseValue, m_sseValue, SIMDMask::AllChannels_StoreAll));
            m_sseValue = _mm_div_ps(m_sseValue, sqrt);
        }
    
        ///
        /// Get the length of the vector.
        ///
        inline float Length() const
        {
            __m128 lengthSquared = _mm_dp_ps(m_sseValue, m_sseValue, SIMDMask::AllChannels_StoreLow);
            return _mm_cvtss_f32(_mm_sqrt_ps(lengthSquared));
        }
    
        ///
        /// Get the length of the first 3 components of the vector.
        ///
        inline float Length3() const
        {
            __m128 lengthSquared = _mm_dp_ps(m_sseValue, m_sseValue, SIMDMask::ThreeChannels_StoreLow);
            return _mm_cvtss_f32(_mm_sqrt_ps(lengthSquared));
        }
    
        ///
        /// Get the inverse length of the vector.
        ///
        inline float LengthInverse() const
        {
            __m128 invLengthSquared = _mm_dp_ps(m_sseValue, m_sseValue, SIMDMask::AllChannels_StoreLow);
            return _mm_cvtss_f32(_mm_rsqrt_ps(invLengthSquared));
        }
        
        ///
        /// Definition of the vector.
        ///
        union
        {
            struct {float x, y, z, w; }; ///< Simple access to simd vector.
            float v[4];                  ///< Array access.
            __m128 m_sseValue;           ///< SIMD version of the vector.
        };
    
    private:
    
        ///
        /// Construction from a SIMD type.
        ///
        Vec4(__m128 value) : m_sseValue(value) {}
    
        ///
        /// Mask values to use in all simd operations.
        ///
        enum SIMDMask
        {
            AllChannels_StoreLow   = 0xF1, ///< Use all 4 channels of the simd value during the calculation. Store the result in the lowest register of the result value.
            AllChannels_StoreAll   = 0xFF, ///< Use all 4 channels of the simd value during the calculation. Store the result in all registers for the result value.
            ThreeChannels_StoreLow = 0x71, ///< Use lower 3 channels of the simd values during the calcuation. Store the result in the lowest register of the result value.
        };
};

} // Namespace Qi
