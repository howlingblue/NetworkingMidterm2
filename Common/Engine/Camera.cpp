#include "Graphics/Renderer.hpp"
#include "EngineMacros.hpp"
#include "Camera.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float Camera::MOVEMENT_SPEED = 15.f;
STATIC const float Camera::ROTATION_SPEED = 90.f;
STATIC const float Camera::DEGREES_ROTATED_PER_PIXEL = 2.f;

//-----------------------------------------------------------------------------------------------
bool Camera::CanSeeObject( FloatVector3 objectPosition, float objectRadius ) const
{
	FloatVector3 positionZeroedZ( m_position.x, m_position.y, 0.f );
	float squaredDistanceToCamera = CalculateSquaredDistanceBetween( positionZeroedZ, objectPosition );
	float doubledSquareRadius = 9 * objectRadius * objectRadius;
	if( squaredDistanceToCamera < doubledSquareRadius )
		return true;

	float distanceToPlane;
	distanceToPlane = leftClippingPlane.CalculateDistanceToPoint( objectPosition );
	if( distanceToPlane > objectRadius )
		return false;

	distanceToPlane = rightClippingPlane.CalculateDistanceToPoint( objectPosition );
	if( distanceToPlane > objectRadius )
		return false;

	return true;
}

//-----------------------------------------------------------------------------------------------
void Camera::ViewWorldThrough() const
{
	static const FloatVector3 X_AXIS( 1.f, 0.f, 0.f );
	static const FloatVector3 Y_AXIS( 0.f, 1.f, 0.f );
	static const FloatVector3 Z_AXIS( 0.f, 0.f, 1.f );

 	Renderer* renderer = Renderer::GetRenderer();
// 	renderer->RotateWorldAboutAxisDegrees( X_AXIS, -m_heading.rollDegreesAboutX );
// 	renderer->RotateWorldAboutAxisDegrees( Y_AXIS, -m_heading.pitchDegreesAboutY );
// 	renderer->RotateWorldAboutAxisDegrees( Z_AXIS, -m_heading.yawDegreesAboutZ );
// 	renderer->TranslateWorld( FloatVector3( -m_position.x, -m_position.y, -m_position.z ) );

	renderer->SetViewMatrixFromCamera( *this );
}
