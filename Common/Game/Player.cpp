#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include <cmath>
#include "Player.hpp"

//-----------------------------------------------------------------------------------------------
void Player::Render() const
{
	float halfWidth = 5.f, halfHeight = 5.f;

	glPushMatrix();
	glTranslatef( m_clientPosition.x, m_clientPosition.y, 0.f );
	glRotatef( m_clientOrientationDegrees, 0.f, 0.f, 1.f ); //FIX: rotation is incorrect. Bad reference frame?

	//glColor4f( (float)m_color.r / 255.f, (float)m_color.g / 255.f, (float)m_color.b / 255.f, (float)m_color.a / 255.f );
	if( m_isIt )
		glColor4f( 1.f, 0.f, 0.f, 1.f );
	else
		glColor4f( 1.f, 1.f, 1.f, 1.f );

	glBegin( GL_QUADS );
	{
		glVertex2f( -halfWidth, halfHeight );

		glVertex2f( halfWidth, halfHeight );

		glVertex2f( halfWidth, -halfHeight );

		glVertex2f( -halfWidth, -halfHeight );
	}
	glEnd();

	glPopMatrix();
}

//-----------------------------------------------------------------------------------------------
void Player::Update( float deltaSeconds )
{
	//Janky Dead Reckoning
	static const float GUESSTIMATED_LATENCY = .001f;
	static const float PERCENT_TO_INTERPOLATE_PER_FRAME = 0.2f;
	Vector2 goalPosition = m_serverPosition + ( m_serverVelocity * GUESSTIMATED_LATENCY );

	m_clientPosition += PERCENT_TO_INTERPOLATE_PER_FRAME * ( goalPosition - m_clientPosition );
	m_clientVelocity += PERCENT_TO_INTERPOLATE_PER_FRAME * ( m_serverVelocity - m_clientVelocity );

	//Interpolate rotation
	float angularDisplacementDegrees = m_serverOrientationDegrees - m_clientOrientationDegrees;
	float angularRotationThisFrame = PERCENT_TO_INTERPOLATE_PER_FRAME * angularDisplacementDegrees;

	if( abs( angularDisplacementDegrees ) < angularRotationThisFrame )
		return;

	while( angularDisplacementDegrees > 180.f )
		angularDisplacementDegrees -= 360.f;

	while( angularDisplacementDegrees < -180.f )
		angularDisplacementDegrees += 360.f;

	if( angularDisplacementDegrees < 0 )
		m_clientOrientationDegrees -= angularDisplacementDegrees;

	if( angularDisplacementDegrees > 0 )
		m_clientOrientationDegrees += angularDisplacementDegrees;
}