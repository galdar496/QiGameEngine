//
//  SSEUtils.h
//  Qi Game Engine
//
//  Created by Cody White on 4/7/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

///
/// Utility functions for interacting with the SSE library.
///

#include <smmintrin.h>

namespace Qi
{

#define QI_SSE_ALIGNMENT sizeof(float) * 4
typedef __m128 SSEType;
    
///
/// Mask values to use in all simd operations.
///
enum SIMDMask
{
    AllChannels_StoreLow   = 0xF1, ///< Use all 4 channels of the simd value during the calculation. Store the result in the lowest register of the result value.
    AllChannels_StoreAll   = 0xFF, ///< Use all 4 channels of the simd value during the calculation. Store the result in all registers for the result value.
    ThreeChannels_StoreLow = 0x71, ///< Use lower 3 channels of the simd values during the calcuation. Store the result in the lowest register of the result value.
};

///
/// 4-component dot product.
///
/// @param left SSE value to perform dot product with.
/// @param right SSE value to perform dot product with.
/// @return Dot product of 'left' and 'right'
///
inline float SSEDot4(const SSEType &left, const SSEType &right)
{
    return _mm_cvtss_f32(_mm_dp_ps(left, right, SIMDMask::AllChannels_StoreLow));
}

    
///
/// 3-component dot product.
///
/// @param left SSE value to perform dot product with.
/// @param right SSE value to perform dot product with.
/// @return Dot product of 'left' and 'right'
///
inline float SSEDot3(const SSEType &left, const SSEType &right)
{
    return _mm_cvtss_f32(_mm_dp_ps(left, right, SIMDMask::ThreeChannels_StoreLow));
}

///
/// Cross product. Note that the cross product is only defined in 3 dimensions, therefore
/// the fourth component of each SSEType is ignored.
///
/// @param left First vector
/// @param right Second vector.
/// @return Perpendicular vector.
///
inline SSEType SSECross(const SSEType &left, const SSEType &right)
{
    return _mm_sub_ps(
                      _mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(right, right, _MM_SHUFFLE(3, 1, 0, 2))),
                      _mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(right, right, _MM_SHUFFLE(3, 0, 2, 1)))
                      );
}

///
/// Return a SIMD 0 (all channels are 0).
///
inline SSEType SSEZero()
{
    return _mm_setzero_ps();
}
    
///
/// Add two SSE values.
///
/// @param left SSE value to add.
/// @param right SSE value to add.
/// @return Addition of 'left' and 'right'.
///
inline SSEType SSEAdd(const SSEType &left, const SSEType &right)
{
    return _mm_add_ps(left, right);
}

///
/// Subtract two SSE values.
///
/// @param left SSE value to subtract.
/// @param right SSE value to subtract.
/// @return Subtraction of 'left' and 'right'.
///
inline SSEType SSESubtract(const SSEType &left, const SSEType &right)
{
    return _mm_sub_ps(left, right);
}

///
/// Multiply two SSE values together.
///
/// @param left SSE value to multiply.
/// @param right SSE value to multiply.
/// @return Multiplication of 'left' and 'right'
///
inline SSEType SSEMultiply(const SSEType &left, const SSEType &right)
{
    return _mm_mul_ps(left, right);
}

///
/// Multiply an SSE value with a scalar value.
///
/// @param sseVal SSE value to multiply.
/// @param scalar Scalar value to multiply.
/// @return Multiplication of 'sseVal' and 'sclar'
///
inline SSEType SSEMultiply(const SSEType &sseVal, const float scalar)
{
    return _mm_mul_ps(sseVal, _mm_set1_ps(scalar));
}

///
/// Divide an SSE value by a scalar value.
///
/// @param sseVal SSE value to divide.
/// @param scalar Scalar value to divide with.
/// @return Division of 'sseVal' and 'sclar'
///
inline SSEType SSEDivide(const SSEType &sseVal, const float scalar)
{
    return _mm_div_ps(sseVal, _mm_set1_ps(scalar));
}

    
} // namespace Qi


