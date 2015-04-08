//
//  Quaternion.h
//  Qi Game Engine
//
//  Created by Cody White on 2/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include "Vec4.h"
#include "Matrix4.h"
#include "../Defines.h"
#include "SSEUtils.h"
#include <cmath>

///
/// Definition of a quaternion and relevant math operations.
///

namespace Qi
{

class QI_ALIGN(QI_SSE_ALIGNMENT) Quaternion
{

    public:
    
        Quaternion() { SetIdentity(); }
        Quaternion(const Vec4 &axis, float angle)
        {
            m_quat = axis.m_sseValue;
            w = angle;
        }
        ~Quaternion() {}
    
        // Copy operations.
        Quaternion(const Quaternion &other)
        {
            m_quat = other.m_quat;
        }
    
        Quaternion &operator=(const Quaternion &other)
        {
            if (this != &other)
            {
                m_quat = other.m_quat;
            }
            
            return *this;
        }
    
        ///
        /// Set this quaternion to be an identity quaternion.
        ///
        inline void SetIdentity()
        {
            m_quat = SSEZero();
            w = 1.0f;
        }

        ///
        /// Create a rotation quaternion based on an angle and axis.
        ///
        /// @param vec 3D vector to rotate about.
        /// @param angle Angle to rotate about.
        ///
        inline void CreateFromAxisAngle(const Vec4 &vec, float angle)
        {
            float halfAngle = angle * 0.5f;
            float sinAngle  = sinf(halfAngle);
            
            m_quat = SSEMultiply(vec.m_sseValue, sinAngle);
            w = cosf(halfAngle);
        }

        ///
        /// Create a quaternion from 3 Euler angles. Angles are specified
        /// in radians. Uses ZYX rotation order.
        ///
        /// @param x Rotation about the x axis (pitch).
        /// @param y Rotation about the y axis (yaw).
        /// @param z Rotation about the z axis (roll).
        ///
        inline void CreateFromEuler(float _x, float _y, float _z)
        {
            float cosX = cosf(0.5f * _x);
            float cosY = cosf(0.5f * _y);
            float cosZ = cosf(0.5f * _z);
            
            float sinX = sinf(0.5f * _x);
            float sinY = sinf(0.5f * _y);
            float sinZ = sinf(0.5f * _z);
            
            w = (cosX * cosY * cosZ) - (sinX * sinY * sinZ);
            x = (sinX * cosY * cosZ) + (cosX * sinY * sinZ);
            y = (cosX * sinY * cosZ) - (sinX * cosY * sinZ);
            z = (cosX * cosY * sinZ) + (sinX * sinY * cosZ);
        }

        ///
        /// Invert this quaternion.
        ///
        inline void Invert()
        {
            float invMag = 1.0f / GetMagnitude();
            Quaternion q = GetConjugate();

            m_quat = SSEMultiply(q.m_quat, invMag);
        }
    
        ///
        /// Normalize the quaternion into a unit quaternion.
        ///
        inline void Normalize()
        {
            float invMag = 1.0f / GetMagnitude();
            m_quat = SSEMultiply(m_quat, invMag);
        }
    
        ///
        /// Get a conjugate of the quaternion.
        ///
        inline Quaternion GetConjugate() const
        {
            return Quaternion(Vec4(x, y, z) * -1.0f, w);
        }

        ///
        /// Get the magnitude of this quaternion.
        ///
        /// @return Magnitude, scalar value.
        ///
        inline float GetMagnitude() const
        {
            return sqrtf(SSEDot4(m_quat, m_quat));
        }
    
        ///
        /// Rotate a vector about this quaternion. This quaternion
        /// is considered to be already unit length (normalized).
        ///
        inline Vec4 Rotate(const Vec4 &v)
        {
            // This is different from the canonical q * p * conjugate(q) for
            // speed reasons.
            // See https://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
//            Vec4 t(SSEMultiply(SSECross(m_quat, v.m_sseValue), 2.0f));
//            return v + (t * w) + Vec4(SSECross(m_quat, t.m_sseValue));
            
            SSEType t = SSEMultiply(SSECross(m_quat, v.m_sseValue), 2.0f);
            SSEType result = SSEAdd(v.m_sseValue, SSEMultiply(t, w));
            result = SSEAdd(result, SSECross(m_quat, t));
            return Vec4(result);
            
//            Vec4 t = m_quat.Cross(v) * 2.0f;
//            return v + (t * m_quat.w) + m_quat.Cross(t);
        }
    
        ///
        /// Convert this quaternion into a row-major matrix.
        ///
        /// @param m Matrix4 to populate with the quaternion.
        ///
        inline void ToMatrix(Matrix4 &m) const
        {
            m(0, 0) = 1.0f - 2.0f * (y * y + z * z);
            m(0, 1) = 2.0f * (x * y - z * w);
            m(0, 2) = 2.0f * (x * z + y * w);
            m(0, 3) = 0.0f;
            
            m(1, 0) = 2.0f * (x * y + z * w);
            m(1, 1) = 1.0f - 2.0f * (x * x + z * z);
            m(1, 2) = 2.0f * (z * y - x * w);
            m(1, 3) = 0.0f;
            
            m(2, 0) = 2.0f * (x * z - y * w);
            m(2, 1) = 2.0f * (y * z + x * w);
            m(2, 2) = 1.0f - 2.0f * (x * x + y * y);
            m(2, 3) = 0.0f;
            
            m(3, 0) = 0.0f;
            m(3, 1) = 0.0f;
            m(3, 2) = 0.0f;
            m(3, 3) = 1.0f;
        }
    
        ///
        /// Perform SLERP (Spherical-linear interpolation) between two quaternions. The
        /// resulting interpolated quaternion will overwrite this instance.
        /// NOTE: The resulting quaternion is _not_ normalized.
        ///
        /// @param start Quaternion which represents an interpolation of 0.0 (start)
        /// @param end Quaternion which represents an interpolation of 1.0 (end)
        /// @param percent Percentage between 'start' and 'end' to interpolate. Value should
        ///        be between 0.0 and 1.0.
        ///
        inline void Slerp(const Quaternion &start, const Quaternion &end, float percent)
        {
            Vec4 tmp;
            float omega, cosOmega, sinOmega, scale0, scale1;
            
            cosOmega = SSEDot4(start.m_quat, end.m_quat);
            
            // Check the signs
            if (cosOmega < 0.0)
            {
                cosOmega = -cosOmega;
                tmp = SSEMultiply(end.m_quat, -1.0f);
            }
            else
            {
                // Set normally.
                tmp = end.m_quat;
            }
            
            // Calculate the coefficients for the SLERP formula.
            if ((1.0f - cosOmega) > FLT_EPSILON)
            {
                // This is the standard case, so apply SLERP.
                omega = acosf(cosOmega);
                sinOmega = sinf(omega);
                scale0 = sinf((1.0f - percent) * omega) / sinOmega;
                scale1 = sinf(percent * omega) / sinOmega;
            }
            else
            {
                // The two quaternions are very close, which means that a normal
                // linear interpolation (LERP) will work just fine and avoid a possible
                // division of 0.
                scale0 = 1.0f - percent;
                scale1 = percent;
            }
            
            // Calculate the resultant quaternion.
            tmp *= scale1;
            m_quat = SSEAdd(SSEMultiply(start.m_quat, scale0), tmp.m_sseValue);
        }
    
        /////// Operator Overloads ////////////////////////////////////
    
        ///
        /// Operator *
        ///
        inline Quaternion operator*(const Quaternion &other) const
        {
            Quaternion q;
            
            // Set the axis.
            q.m_quat = SSECross(m_quat, other.m_quat);
            q.m_quat = SSEAdd(q.m_quat, SSEMultiply(other.m_quat, w));
            q.m_quat = SSEAdd(q.m_quat, SSEMultiply(m_quat, other.w));
            
            // Set the angle.
            q.w = (w * other.w) - SSEDot3(m_quat, other.m_quat);
            
            return q;
        }
    
        ///< Internal quaternion object. x,y,z represents and axis and w a rotation.
        union
        {
            struct {float x, y, z, w; }; ///< Simple access to simd quaternion.
            SSEType m_quat;              ///< SIMD quaternion.
        };

};

} // namespace Qi
