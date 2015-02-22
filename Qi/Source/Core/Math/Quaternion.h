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
    
        Quaternion() { setIdentity(); }
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
        inline void setIdentity()
        {
            m_quat.zero();
            m_quat.w = 1.0f;
        }

        ///
        /// Create a rotation quaternion based on an angle and axis.
        /// @param vec 3D vector to rotate about.
        /// @param angle Angle to rotate about.
        ///
        inline void createFromAxisAngle(const Vec4 &vec, float angle)
        {
            float half_angle = angle * 0.5f;
            float sin_angle  = sinf(half_angle);
            
            m_quat = vec * sin_angle;
            m_quat.w = cosf(half_angle);
        }

        ///
        /// Create a quaternion from 3 Euler angles. Angles are specified
        /// in radians. Uses ZYX rotation order.
        /// @param x Rotation about the x axis (pitch).
        /// @param y Rotation about the y axis (yaw).
        /// @param z Rotation about the z axis (roll).
        ///
        inline void createFromEuler(float x, float y, float z)
        {
            float cos_x = cosf(0.5f * x);
            float cos_y = cosf(0.5f * y);
            float cos_z = cosf(0.5f * z);
            
            float sin_x = sinf(0.5f * x);
            float sin_y = sinf(0.5f * y);
            float sin_z = sinf(0.5f * z);
            
            m_quat.w = (cos_x * cos_y * cos_z) - (sin_x * sin_y * sin_z);
            m_quat.x = (sin_x * cos_y * cos_z) + (cos_x * sin_y * sin_z);
            m_quat.y = (cos_x * sin_y * cos_z) - (sin_x * cos_y * sin_z);
            m_quat.z = (cos_x * cos_y * sin_z) + (sin_x * sin_y * cos_z);
        }

        ///
        /// Invert this quaternion.
        ///
        inline void invert()
        {
            float inv_mag = 1.0f / getMagnitude();
            Quaternion q = getConjugate();
            q.m_quat *= inv_mag;
            
            m_quat = q.m_quat;
        }
    
        ///
        /// Normalize the quaternion into a unit quaternion.
        ///
        inline void normalize()
        {
            float inv_mag = 1.0f / getMagnitude();
            m_quat *= inv_mag;
        }
    
        ///
        /// Get a conjugate of the quaternion.
        ///
        inline Quaternion getConjugate() const
        {
            Quaternion q(*this);
            q.m_quat.w *= -1.0f;
            
            return q;
        }

        ///
        /// Get the magnitude of this quaternion.
        /// @return Magnitude, scalar value.
        ///
        inline float getMagnitude() const
        {
            return m_quat.length();
        }
    
        ///
        /// Rotate a vector about this quaternion.
        ///
        inline Vec4 rotate(const Vec4 &v)
        {
            Quaternion q(v, 0.0f);
            q.normalize();

            return (*this * q * getConjugate()).m_quat;
        }
    
        ///
        /// Convert this quaternion into a row-major matrix.
        /// @param m Matrix4 to populate with the quaternion.
        ///
        inline void toMatrix(Matrix4 &m) const
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
        inline void slerp(const Quaternion &start, const Quaternion &end, float percent)
        {
            Vec4 tmp;
            float omega, cos_omega, sin_omega, scale0, scale1;
            
            cos_omega = start.m_quat.dot(end.m_quat);
            
            // Check the signs
            if (cos_omega < 0.0)
            {
                cos_omega = -cos_omega;
                tmp = end.m_quat * -1.0f;
            }
            else
            {
                // Set normally.
                tmp = end.m_quat;
            }
            
            // Calculate the coefficients for the SLERP formula.
            if ((1.0f - cos_omega) > FLT_EPSILON)
            {
                // This is the standard case, so apply SLERP.
                omega = acosf(cos_omega);
                sin_omega = sinf(omega);
                scale0 = sinf((1.0f - percent) * omega) / sin_omega;
                scale1 = sinf(percent * omega) / sin_omega;
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
            q.m_quat = m_quat.cross(other.m_quat) + (other.m_quat * m_quat.w) + (m_quat * other.m_quat.w);
            
            // Set the angle.
            q.m_quat.w = (m_quat.w * other.m_quat.w) - m_quat.dot3(other.m_quat);
            
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
