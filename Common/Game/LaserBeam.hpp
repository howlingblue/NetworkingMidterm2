#pragma once
#ifndef INCLUDED_LASER_BEAM_HPP
#define INCLUDED_LASER_BEAM_HPP

//-----------------------------------------------------------------------------------------------
#include "../Engine/Math/EngineMath.hpp"
#include "../Engine/Math/FloatVector2.hpp"
#include "Entity.hpp"

struct MaterialComponent;

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

	const FloatVector2& GetAngleVector() const { return m_fireAngleAsVector; }
	const Entity* GetFirer() const { return m_firer; }
	bool IsDamaging() const { return ( m_secondsSinceFired > DAMAGING_LENGTH_SECONDS ); }
	bool ReadyForCleanup() const { return ( m_secondsSinceFired > LIFETIME_SECONDS ); }

private:
	const Entity* m_firer;
	FloatVector2 m_fireAngleAsVector;
	float m_secondsSinceFired;
	MaterialComponent* m_laserMaterial;
};

#endif //INCLUDED_LASER_BEAM_HPP