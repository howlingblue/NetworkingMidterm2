#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "Objective.hpp"

//-----------------------------------------------------------------------------------------------
void Objective::Render() const
{

	float halfWidth = 5.f, halfHeight = 5.f;

	glPushMatrix();
	glTranslatef( m_currentPosition.x, m_currentPosition.y, 0.f );
	glColor4f( m_color.r, m_color.g, m_color.b, m_color.a );

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
void Objective::Update( float )
{
	m_currentPosition += 0.2f * ( m_serverPosition - m_currentPosition );
	//m_currentPosition = m_serverPosition;
}
