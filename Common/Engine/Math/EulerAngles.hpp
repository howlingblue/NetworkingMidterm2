#ifndef INCLUDED_EULER_ANGLES_HPP
#define INCLUDED_EULER_ANGLES_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "EngineMath.hpp"

//-----------------------------------------------------------------------------------------------
struct EulerAngles
{
	float rollDegreesAboutX;
	float pitchDegreesAboutY;
	float yawDegreesAboutZ;

	EulerAngles()
		: rollDegreesAboutX( 0.f )
		, pitchDegreesAboutY( 0.f )
		, yawDegreesAboutZ( 0.f )
	{ }

	EulerAngles( float rollAboutXDegrees, float pitchAboutYDegrees, float yawAboutZDegrees )
		: rollDegreesAboutX( rollAboutXDegrees )
		, pitchDegreesAboutY( pitchAboutYDegrees )
		, yawDegreesAboutZ( yawAboutZDegrees )
	{ }

	FloatVector3 CalculateUnitDirectionVector() const;
};

//-----------------------------------------------------------------------------------------------
inline FloatVector3 EulerAngles::CalculateUnitDirectionVector() const
{
	float x = cos( ConvertDegreesToRadians( yawDegreesAboutZ ) ) * cos( ConvertDegreesToRadians( pitchDegreesAboutY ) );
	float y = sin( ConvertDegreesToRadians( yawDegreesAboutZ ) ) * cos( ConvertDegreesToRadians( pitchDegreesAboutY ) );
	float z = -sin( ConvertDegreesToRadians( pitchDegreesAboutY ) );
	FloatVector3 unitViewVector( x, y, z );
	unitViewVector.Normalize();
	return unitViewVector;
}

#endif //INCLUDED_EULER_ANGLES_HPP
