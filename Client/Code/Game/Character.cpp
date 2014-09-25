#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "Character.hpp"

//-----------------------------------------------------------------------------------------------
void Character::Render() const
{
	float halfWidth = 5.f, halfHeight = 5.f;

	glPushMatrix();
	glTranslatef( m_currentPosition.x, m_currentPosition.y, 0.f );
	glColor4f( (float)m_color.r / 255.f, (float)m_color.g / 255.f, (float)m_color.b / 255.f, (float)m_color.a / 255.f );

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
void Character::Update( float )
{
	m_currentPosition += 0.2f * ( m_serverPosition - m_currentPosition );
	//m_currentPosition = m_serverPosition;
}
