//
//  Vec4.h
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "SSEUtils.h"
#include "../Defines.h"
#include "../Reflection/Reflection.h"
#include <cmath>

namespace Qi
{

///
/// Stores the information for a 4 component vector. When possible,
/// the vector will use SSE instructions. Requires SSE 4.1.
///

class QI_ALIGN(QI_SSE_ALIGNMENT) Vec4
{
    public:

		QI_DECLARE_REFLECTED_CLASS(Vec4);
    
		Vec4();
        
    
        ~Vec4() {}
    
        Vec4(const Vec4 &other)
        {
            m_sseValue = other.m_sseValue;
        }
    
        ///
        /// Construction from a SIMD type.
        ///
        Vec4(SSEType value) : m_sseValue(value) {}
    
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
            m_sseValue = SSEZero();
        }
    
        // Math Operations //////////////////////////////////////
    
        ///
        /// Operator+
        ///
        /// @param other Vector to add.
        ///
        inline Vec4 operator+(const Vec4 &other) const
        {
            return SSEAdd(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// Operator-
        ///
        /// @param other Vector to subtract.
        ///
        inline Vec4 operator-(const Vec4 &other) const
        {
            return SSESubtract(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// Operator*
        ///
        /// @param scalar Scalar value to multiply the vector by.
        ///
        inline Vec4 operator*(float scalar) const
        {
            return SSEMultiply(m_sseValue, scalar);
        }
    
        ///
        /// Operator/
        ///
        /// @param scalar Scalar value to divide the vector by.
        ///
        inline Vec4 operator/(float scalar) const
        {
            return SSEDivide(m_sseValue, scalar);
        }
    
        ///
        /// Operator*=
        ///
        /// @param scalar Scalar value to multiply the vector by.
        ///
        inline void operator*=(float scalar)
        {
            m_sseValue = SSEMultiply(m_sseValue, scalar);
        }
    
        ///
        /// Operator/=
        ///
        /// @param scalar Scalar value to divide the vector by.
        ///
        inline void operator/=(float scalar)
        {
            m_sseValue = SSEMultiply(m_sseValue, scalar);
        }
    
        ///
        /// Dot product.
        ///
        /// @paramm other Vector to perform dot product with.
        /// @return Scalar value denoting the dot product.
        ///
        inline float Dot(const Vec4 &other) const
        {
            return SSEDot4(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// 3-component dot product.
        ///
        /// @paramm other Vector to perform dot product with.
        /// @return Scalar value denoting the dot product.
        ///
        inline float Dot3(const Vec4 &other) const
        {
            return SSEDot3(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// Cross product. Note that the cross product is only defined in 3 dimensions, therefore
        /// the fourth component of each vector is ignored.
        ///
        /// @param other Vector to cross with.
        /// @return Perpendicular vector.
        ///
        inline Vec4 Cross(const Vec4 &other) const
        {
            return SSECross(m_sseValue, other.m_sseValue);
        }
    
        ///
        /// Normalize the vector. The normalized version will overwrite the previous version.
        /// Note that this function is optimized for speed and may produce a normal that does not quite have a unit
        /// length (but is very close).
        ///
        inline void Normalize()
        {
            float dot = SSEDot4(m_sseValue, m_sseValue);
            m_sseValue = SSEDivide(m_sseValue, sqrtf(dot));
        }
    
        ///
        /// Get the length of the vector.
        ///
        inline float Length() const
        {
            float dot = SSEDot4(m_sseValue, m_sseValue);
            return sqrtf(dot);
        }
    
        ///
        /// Get the length of the first 3 components of the vector.
        ///
        inline float Length3() const
        {
            float dot = SSEDot3(m_sseValue, m_sseValue);
            return sqrtf(dot);
        }
    
        ///
        /// Get the inverse length of the vector.
        ///
        inline float LengthInverse() const
        {
            float dot = SSEDot4(m_sseValue, m_sseValue);
            return (1.0f / sqrtf(dot));
        }
    
        ///
        /// Get the squared length of the vector.
        ///
        inline float LengthSquared() const
        {
            return SSEDot4(m_sseValue, m_sseValue);
        }
    
    
        ///
        /// Get the squared length of the vector.
        ///
        inline float LengthSquared3() const
        {
            return SSEDot3(m_sseValue, m_sseValue);
        }
    
        ///
        /// Definition of the vector.
        ///
        union
        {
            struct {float x, y, z, w; }; ///< Simple access to simd vector.
            float v[4];                  ///< Array access.
            SSEType m_sseValue;          ///< SIMD version of the vector.
        };
};

} // Namespace Qi
