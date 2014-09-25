#ifndef INCLUDED_ENGINE_MATH_HPP
#define INCLUDED_ENGINE_MATH_HPP
#pragma once

//----------------------------------------------------------------------------------------------------
#include "Vector2.hpp"
#include <cstdlib>

//----------------------------------------------------------------------------------------------------
const float PI = 3.14159265359f;
const float ONE_OVER_RAND_MAX = 1.0f / static_cast< float >( RAND_MAX );
const float EPSILON = 0.00001f;

//----------------------------------------------------------------------------------------------------
bool AreAlmostEqual( float a, float b );
Vector2 ConvertAngleToUnitCirclePosition( float angleRadians );
float ConvertVectorToAngleRadians( Vector2 vector );

float ConvertDegreesToRadians( float angleDegrees );
float ConvertRadiansToDegrees( float angleRadians );

float GetRandomFloatBetweenZeroandOne();
int	  GetRandomIntBetween( int inclusiveMinimum, int exclusiveMaximum );

#endif // INCLUDED_ENGINE_MATH_HPP
