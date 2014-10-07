#include "LaserBeam.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "../Engine/EngineCommon.hpp"

STATIC const float LaserBeam::DAMAGING_LENGTH_SECONDS = 0.2f;
STATIC const float LaserBeam::LIFETIME_SECONDS = 3.f;

void LaserBeam::Render() const
{
	glPushMatrix();
	glTranslatef( m_clientPosition.x, m_clientPosition.y, 0.f );

	glColor4f( 1.f, 0.f, 0.3f, ( LIFETIME_SECONDS - m_secondsSinceFired ) / LIFETIME_SECONDS );

	glBegin( GL_LINES );
	{
		glVertex2f( 0.f, 0.f );

		glVertex2f( 600.f * m_fireAngleAsVector.x, 600.f * m_fireAngleAsVector.y );
	}
	glEnd();

	glPopMatrix();
}

void LaserBeam::Update( float deltaSeconds )
{
	m_secondsSinceFired += deltaSeconds;
}
