#ifndef INCLUDED_CAMERA_HPP
#define INCLUDED_CAMERA_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"
#include "Math/EngineMath.hpp"
#include "Math/EulerAngles.hpp"
#include "Math/FloatVector3.hpp"
#include "Math/Plane.hpp"

//-----------------------------------------------------------------------------------------------
class Camera
{
	//Constants
	static const float MOVEMENT_SPEED;
	static const float ROTATION_SPEED;
	static const float DEGREES_ROTATED_PER_PIXEL;

	//Data Members
	FloatVector3 m_position;
	EulerAngles m_heading;
	Plane leftClippingPlane;
	Plane rightClippingPlane;

	void ClampPitchToFrontCircleHalf();
	void GenerateClippingPlanes();

public:
	//Constructors
	Camera()
		: m_heading( 0.f, 0.f, 0.f )
	{
		GenerateClippingPlanes();
	}

	Camera( float x, float y, float z )
		: m_position( x, y, z )
		, m_heading( 0.f, 0.f, 0.f )
	{
		GenerateClippingPlanes();
	}

	explicit Camera( FloatVector3 position )
		: m_position( position.x, position.y, position.z )
		, m_heading( 0.f, 0.f, 0.f )
	{
		GenerateClippingPlanes();
	}

	FloatVector3		CalculateUnitViewDirVector() const  { return m_heading.CalculateUnitDirectionVector(); }
	const EulerAngles&	GetHeading() const					{ return m_heading; }
	const FloatVector3& GetPosition() const					{ return m_position; }

	void MoveByVector( FloatVector3 movementVector ) { m_position += movementVector; }
	void MoveInXYPlane( float angleDegrees, float magnitudeBetweenZeroAndOne, float deltaSeconds );
	void MoveOnZAxis( float magnitudeBetweenNegOneAndOne, float deltaSeconds );
	
	void RotateYawBy( float magnitudeNegOneToOne, float deltaSeconds );
	void RotatePitchBy( float magnitudeNegOneToOne, float deltaSeconds );
	void RotateRollBy( float magnitudeNegOneToOne, float deltaSeconds );

	void SetRotation( float roll, float pitch, float yaw ) 
	{ 
		m_heading.rollDegreesAboutX = roll;
		m_heading.pitchDegreesAboutY = pitch;
		m_heading.yawDegreesAboutZ = yaw; 
	}

	bool CanSeeObject( FloatVector3 position, float radius ) const;
	void ViewWorldThrough() const;
};



//-----------------------------------------------------------------------------------------------
inline void Camera::ClampPitchToFrontCircleHalf()
{
	if( m_heading.pitchDegreesAboutY > 90.f ) 
		m_heading.pitchDegreesAboutY = 89.9f;
	if( m_heading.pitchDegreesAboutY < -90.f ) 
		m_heading.pitchDegreesAboutY = -89.9f;
}

//-----------------------------------------------------------------------------------------------
inline void Camera::GenerateClippingPlanes()
{
	float currentAngleAroundZRadians = ConvertDegreesToRadians( m_heading.yawDegreesAboutZ );
	float halfHorizontalFieldOfViewRadians = ConvertDegreesToRadians( 45.f );
	leftClippingPlane.normal  = FloatVector3( -sin( currentAngleAroundZRadians + halfHorizontalFieldOfViewRadians ), cos( currentAngleAroundZRadians + halfHorizontalFieldOfViewRadians ), 0.f );
	leftClippingPlane.normal.Normalize();
	rightClippingPlane.normal = FloatVector3( sin( currentAngleAroundZRadians - halfHorizontalFieldOfViewRadians ), -cos( currentAngleAroundZRadians - halfHorizontalFieldOfViewRadians ), 0.f );
	rightClippingPlane.normal.Normalize();

	leftClippingPlane.distanceToOrigin = -DotProduct( m_position, leftClippingPlane.normal );
	rightClippingPlane.distanceToOrigin = -DotProduct( m_position, rightClippingPlane.normal );
}

//-----------------------------------------------------------------------------------------------
inline void Camera::MoveInXYPlane( float angleDegrees, float magnitudeBetweenZeroAndOne, float deltaSeconds )
{
	float adjustedAngleDegrees = angleDegrees + m_heading.yawDegreesAboutZ - 90.f;
	FloatVector3 movementVector( cos( ConvertDegreesToRadians( adjustedAngleDegrees ) ), sin( ConvertDegreesToRadians( adjustedAngleDegrees ) ), 0.f );
	m_position += ( movementVector ) * magnitudeBetweenZeroAndOne * MOVEMENT_SPEED * deltaSeconds;
	GenerateClippingPlanes();
}

//-----------------------------------------------------------------------------------------------
inline void Camera::MoveOnZAxis( float magnitudeBetweenNegOneAndOne, float deltaSeconds )
{
	m_position.z += magnitudeBetweenNegOneAndOne * MOVEMENT_SPEED * deltaSeconds;
}

//-----------------------------------------------------------------------------------------------
inline void Camera::RotateYawBy( float magnitudeNegOneToOne, float deltaSeconds )
{
	m_heading.yawDegreesAboutZ += magnitudeNegOneToOne * ROTATION_SPEED * deltaSeconds;
	GenerateClippingPlanes();
}

//-----------------------------------------------------------------------------------------------
inline void Camera::RotatePitchBy( float magnitudeNegOneToOne, float deltaSeconds )
{
	m_heading.pitchDegreesAboutY += magnitudeNegOneToOne * ROTATION_SPEED * deltaSeconds;
	ClampPitchToFrontCircleHalf();
}

//-----------------------------------------------------------------------------------------------
inline void Camera::RotateRollBy( float magnitudeNegOneToOne, float deltaSeconds )
{
	m_heading.rollDegreesAboutX += magnitudeNegOneToOne * ROTATION_SPEED * deltaSeconds;
}


#endif //INCLUDED_CAMERA_HPP
