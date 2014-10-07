#include <algorithm>
#include <math.h>
#include "EngineMath.hpp"

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
Vector2 ConvertAngleToUnitCirclePosition( float angleRadians )
{
	return Vector2( cos( angleRadians ), -sin( angleRadians ) );
}

//----------------------------------------------------------------------------------------------------
float ConvertVectorToAngleRadians( Vector2 vector )
{
	return atan2( vector.y, vector.x );
}

//----------------------------------------------------------------------------------------------------
float ConvertDegreesToRadians( float angleDegrees )
{
	return angleDegrees * ( PI / 180.f );
}

//----------------------------------------------------------------------------------------------------
float ConvertRadiansToDegrees( float angleRadians )
{
	return angleRadians * ( 180.f / PI );
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
