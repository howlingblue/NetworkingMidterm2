#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

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
	Vector2 goalPosition = m_serverPosition + ( m_serverVelocity * GUESSTIMATED_LATENCY );

	m_clientPosition += 0.2f * ( goalPosition - m_clientPosition );
	m_clientVelocity += 0.2f * ( m_serverVelocity - m_clientVelocity );
	m_clientOrientationDegrees += 0.2f * ( m_serverOrientationDegrees - m_clientOrientationDegrees );
}