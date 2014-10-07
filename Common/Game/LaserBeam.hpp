#pragma once
#ifndef INCLUDED_LASER_BEAM_HPP
#define INCLUDED_LASER_BEAM_HPP

//-----------------------------------------------------------------------------------------------
#include "../Engine/EngineMath.hpp"
#include "../Engine/Vector2.hpp"
#include "Entity.hpp"

//-----------------------------------------------------------------------------------------------
class LaserBeam : public Entity
{
	static const float DAMAGING_LENGTH_SECONDS;
	static const float LIFETIME_SECONDS;

public:
	static const unsigned char DAMAGE_DEALT_ON_HIT = 1;

	LaserBeam( const Entity* firer );
	~LaserBeam() { }

	void Render() const;
	void Update( float deltaSeconds );

	const Vector2& GetAngleVector() const { return m_fireAngleAsVector; }
	const Entity* GetFirer() const { return m_firer; }
	bool IsDamaging() const { return ( m_secondsSinceFired > DAMAGING_LENGTH_SECONDS ); }
	bool ReadyForCleanup() const { return ( m_secondsSinceFired > LIFETIME_SECONDS ); }

private:
	const Entity* m_firer;
	Vector2 m_fireAngleAsVector;
	float m_secondsSinceFired;
};



//-----------------------------------------------------------------------------------------------
inline LaserBeam::LaserBeam( const Entity* firer )
	: m_firer( firer )
	, m_fireAngleAsVector( ConvertAngleToUnitCirclePosition( ConvertDegreesToRadians( firer->GetCurrentOrientation() ) ) )
	, m_secondsSinceFired( 0.f )
{
	m_clientPosition = firer->GetCurrentPosition();
	m_serverPosition = m_clientPosition;
}

#endif //INCLUDED_LASER_BEAM_HPP