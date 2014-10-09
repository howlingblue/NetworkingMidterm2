#ifndef INCLUDED_ENGINE_MATH_HPP
#define INCLUDED_ENGINE_MATH_HPP
#pragma once

//----------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <math.h>
#include "FloatVector2.hpp"
#include "FloatVector3.hpp"

//----------------------------------------------------------------------------------------------------
const float GOLDEN_RATIO = 1.61803398f;
const float PI = 3.14159265f;
const float ONE_OVER_RAND_MAX = 1.0f / static_cast< float >( RAND_MAX );
const float EPSILON = 0.00001f;

//----------------------------------------------------------------------------------------------------
bool	AreAlmostEqual( float a, float b );



//----------------------------------------------------------------------------------------------------
FloatVector2	ConvertAngleToUnitCirclePosition( float angleRadians );

//----------------------------------------------------------------------------------------------------
float			ConvertVectorToAngleRadians( FloatVector2 vector );



//----------------------------------------------------------------------------------------------------
inline float ConvertDegreesToRadians( float angleDegrees )
{
	return angleDegrees * ( PI / 180.f );
}

//----------------------------------------------------------------------------------------------------
inline float ConvertRadiansToDegrees( float angleRadians )
{
	return angleRadians * ( 180.f / PI );
}



//----------------------------------------------------------------------------------------------------
float	CalculateSquaredDistanceBetween( FloatVector2 positionA, FloatVector2 positionB );

//----------------------------------------------------------------------------------------------------
float	CalculateSquaredDistanceBetween( FloatVector3 positionA, FloatVector3 positionB );



//----------------------------------------------------------------------------------------------------
unsigned int ExtractNthDigitFromInt( int n, int number );



//----------------------------------------------------------------------------------------------------
float	GetRandomFloatBetweenZeroandOne();

//----------------------------------------------------------------------------------------------------
int		GetRandomIntBetween( int inclusiveMinimum, int exclusiveMaximum );



//----------------------------------------------------------------------------------------------------
inline float RoundNumberToWhole( float number )
{
	return floor( number + 0.5f );
}



//----------------------------------------------------------------------------------------------------
//This awesome number clamping function courtesy of user justin from StackOverflow:
//http://stackoverflow.com/questions/9323903/most-efficient-elegant-way-to-clip-a-number
template< typename NumericType >
inline NumericType ClampNumberToWithin( NumericType number, NumericType minimumInclusive, NumericType maximumInclusive )
{
	return std::max( minimumInclusive, std::min( number, maximumInclusive ) );
}

//----------------------------------------------------------------------------------------------------
typedef unsigned char Byte;

template< typename PointerType >
inline void AdvancePointerNumberOfBytes( PointerType& pointer, size_t numberOfBytesToAdvance )
{
	Byte* pointerAsBytes = reinterpret_cast< Byte* >( pointer );
	pointerAsBytes += numberOfBytesToAdvance;
	pointer = reinterpret_cast< PointerType >( pointerAsBytes );
}

template< typename PointerType >
inline void RetreatPointerNumberOfBytes( PointerType& pointer, size_t numberOfBytesToAdvance )
{
	Byte* pointerAsBytes = reinterpret_cast< Byte* >( pointer );
	pointerAsBytes -= numberOfBytesToAdvance;
	pointer = reinterpret_cast< PointerType >( pointerAsBytes );
}

//-----------------------------------------------------------------------------------------------
inline void GramSchmidt3D( FloatVector3& vector1, FloatVector3& vector2, FloatVector3& vector3 )
{
	vector1.Normalize();
	vector2 = vector2 - ( DotProduct( vector2, vector1 ) / DotProduct( vector1, vector1 ) ) * vector1;
	vector2.Normalize();
	vector3 = vector3 - ( DotProduct( vector3, vector2 ) / DotProduct( vector2, vector2 ) ) * vector2 - ( DotProduct( vector3, vector1 ) / DotProduct( vector1, vector1 ) ) * vector1;
	vector3.Normalize();
}


#endif // INCLUDED_ENGINE_MATH_HPP
