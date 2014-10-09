#include <algorithm>
#include <math.h>
#include "EngineMath.hpp"
#include "FloatVector2.hpp"

//----------------------------------------------------------------------------------------------------
// This wonderful snippet is borrowed from: http://realtimecollisiondetection.net/blog/?p=89
bool AreAlmostEqual( float a, float b )
{
	float largerOfAandB = std::max( abs( a ), abs( b ) );
	if  ( abs( a - b ) <= EPSILON * std::max( 1.0f, largerOfAandB ) )
		return true;
	return false;
}

//----------------------------------------------------------------------------------------------------
FloatVector2 ConvertAngleToUnitCirclePosition( float angleRadians )
{
	return FloatVector2( cos( angleRadians ), sin( angleRadians ) );
}

//----------------------------------------------------------------------------------------------------
float ConvertVectorToAngleRadians( FloatVector2 vector )
{
	return atan2( vector.y, vector.x );
}

//----------------------------------------------------------------------------------------------------
float CalculateSquaredDistanceBetween( FloatVector2 positionA, FloatVector2 positionB )
{
	float xDifference = positionB.x - positionA.x;
	float yDifference = positionB.y - positionA.y;

	return ( xDifference * xDifference ) + ( yDifference * yDifference );
}

//----------------------------------------------------------------------------------------------------
float CalculateSquaredDistanceBetween( FloatVector3 positionA, FloatVector3 positionB )
{
	float xDifference = positionB.x - positionA.x;
	float yDifference = positionB.y - positionA.y;
	float zDifference = positionB.z - positionA.z;

	return ( xDifference * xDifference ) + ( yDifference * yDifference ) + ( zDifference * zDifference );
}

//----------------------------------------------------------------------------------------------------
//This formula from: http://mathforum.org/library/drmath/view/56002.html.
unsigned int ExtractNthDigitFromInt( int n, int number )
{
	return static_cast< unsigned int >( static_cast< float >( number ) / pow(10.f, ( n - 1 ) ) ) % 10;
}

//----------------------------------------------------------------------------------------------------
float GetRandomFloatBetweenZeroandOne() 
{
	return static_cast< float >( rand() ) * ONE_OVER_RAND_MAX ;
}

//----------------------------------------------------------------------------------------------------
int GetRandomIntBetween( int inclusiveMinimum, int exclusiveMaximum )
{
	int numberSpread = exclusiveMaximum - inclusiveMinimum;
	return ( rand() % numberSpread ) + inclusiveMinimum;
}
