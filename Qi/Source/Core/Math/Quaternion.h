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
#include <math.h>

///
/// Definition of a quaternion and relevant math operations.
///

namespace Qi
{

class Quaternion
{

    public:
    
        Quaternion() { SetIdentity(); }
        Quaternion(const Vec4 &axis, float angle)
        {
            m_quat = axis;
            m_quat.w = angle;
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
            m_quat.Zero();
            m_quat.w = 1.0f;
        }

        ///
        /// Create a rotation quaternion based on an angle and axis.
        /// @param vec 3D vector to rotate about.
        /// @param angle Angle to rotate about.
        ///
        inline void CreateFromAxisAngle(const Vec4 &vec, float angle)
        {
            float halfAngle = angle * 0.5f;
            float sinAngle  = sinf(halfAngle);
            
            m_quat = vec * sinAngle;
            m_quat.w = cosf(halfAngle);
        }

        ///
        /// Create a quaternion from 3 Euler angles. Angles are specified
        /// in radians. Uses ZYX rotation order.
        /// @param x Rotation about the x axis (pitch).
        /// @param y Rotation about the y axis (yaw).
        /// @param z Rotation about the z axis (roll).
        ///
        inline void CreateFromEuler(float x, float y, float z)
        {
            float cosX = cosf(0.5f * x);
            float cosY = cosf(0.5f * y);
            float cosZ = cosf(0.5f * z);
            
            float sinX = sinf(0.5f * x);
            float sinY = sinf(0.5f * y);
            float sinZ = sinf(0.5f * z);
            
            m_quat.w = (cosX * cosY * cosZ) - (sinX * sinY * sinZ);
            m_quat.x = (sinX * cosY * cosZ) + (cosX * sinY * sinZ);
            m_quat.y = (cosX * sinY * cosZ) - (sinX * cosY * sinZ);
            m_quat.z = (cosX * cosY * sinZ) + (sinX * sinY * cosZ);
        }

        ///
        /// Invert this quaternion.
        ///
        inline void Invert()
        {
            float invMag = 1.0f / GetMagnitude();
            Quaternion q = GetConjugate();
            q.m_quat *= invMag;
            
            m_quat = q.m_quat;
        }
    
        ///
        /// Normalize the quaternion into a unit quaternion.
        ///
        inline void Normalize()
        {
            float invMag = 1.0f / GetMagnitude();
            m_quat *= invMag;
        }
    
        ///
        /// Get a conjugate of the quaternion.
        ///
        inline Quaternion GetConjugate() const
        {
            return Quaternion(Vec4(m_quat.x, m_quat.y, m_quat.z) * -1.0f, m_quat.w);
        }

        ///
        /// Get the magnitude of this quaternion.
        /// @return Magnitude, scalar value.
        ///
        inline float GetMagnitude() const
        {
            return m_quat.Length();
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
            Vec4 t = m_quat.Cross(v) * 2.0f;
            return v + (t * m_quat.w) + m_quat.Cross(t);
        }
    
        ///
        /// Convert this quaternion into a row-major matrix.
        /// @param m Matrix4 to populate with the quaternion.
        ///
        inline void ToMatrix(Matrix4 &m) const
        {
            m(0, 0) = 1.0f - 2.0f * (m_quat.y * m_quat.y + m_quat.z * m_quat.z);
            m(0, 1) = 2.0f * (m_quat.x * m_quat.y - m_quat.z * m_quat.w);
            m(0, 2) = 2.0f * (m_quat.x * m_quat.z + m_quat.y * m_quat.w);
            m(0, 3) = 0.0f;
            
            m(1, 0) = 2.0f * (m_quat.x * m_quat.y + m_quat.z * m_quat.w);
            m(1, 1) = 1.0f - 2.0f * (m_quat.x * m_quat.x + m_quat.z * m_quat.z);
            m(1, 2) = 2.0f * (m_quat.z * m_quat.y - m_quat.x * m_quat.w);
            m(1, 3) = 0.0f;
            
            m(2, 0) = 2.0f * (m_quat.x * m_quat.z - m_quat.y * m_quat.w);
            m(2, 1) = 2.0f * (m_quat.y * m_quat.z + m_quat.x * m_quat.w);
            m(2, 2) = 1.0f - 2.0f * (m_quat.x * m_quat.x + m_quat.y * m_quat.y);
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
        /// @param start Quaternion which represents an interpolation of 0.0 (start)
        /// @param end Quaternion which represents an interpolation of 1.0 (end)
        /// @param percent Percentage between 'start' and 'end' to interpolate. Value should
        ///        be between 0.0 and 1.0.
        ///
        inline void Slerp(const Quaternion &start, const Quaternion &end, float percent)
        {
            Vec4 tmp;
            float omega, cosOmega, sinOmega, scale0, scale1;
            
            cosOmega = start.m_quat.Dot(end.m_quat);
            
            // Check the signs
            if (cosOmega < 0.0)
            {
                cosOmega = -cosOmega;
                tmp = end.m_quat * -1.0f;
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
            m_quat = (start.m_quat * scale0) + tmp;
        }
    
        /////// Operator Overloads ////////////////////////////////////
    
        ///
        /// Operator *
        ///
        inline Quaternion operator*(const Quaternion &other) const
        {
            Quaternion q;
            
            // Set the axis.
            q.m_quat = m_quat.Cross(other.m_quat) + (other.m_quat * m_quat.w) + (m_quat * other.m_quat.w);
            
            // Set the angle.
            q.m_quat.w = (m_quat.w * other.m_quat.w) - m_quat.Dot3(other.m_quat);
            
            return q;
        }
    
        ///< Internal quaternion object. x,y,z represents and axis and w a rotation.
        union
        {
            struct {float x, y, z, w; }; ///< Simple access to simd quaternion.
            Vec4 m_quat;                 ///< SIMD quaternion.
        };

};

} // namespace Qi
