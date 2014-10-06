#pragma once
#ifndef INCLUDED_ENTITY_HPP
#define INCLUDED_ENTITY_HPP

#include "../Engine/Color.hpp"
#include "../Engine/Vector2.hpp"

//-----------------------------------------------------------------------------------------------
class Entity
{
public:
	Entity();
	~Entity() {}

	void Render() const;
	void Update( float deltaSeconds );

	const Vector2& GetCurrentPosition() const { return m_clientPosition; }
	const Vector2& GetCurrentVelocity() const { return m_clientVelocity; }
	const Vector2& GetCurrentAcceleration() const { return m_clientAcceleration; }
	float GetCurrentOrientation() const { return m_clientOrientationDegrees; }

	const Color& GetColor() const { return m_color; }
	unsigned char GetHealth() const { return m_health; }
	unsigned char GetID() const { return m_playerID; }
	bool IsIt() const { return m_isIt; }
	unsigned char GetScore() { return m_score; }

	void SetColor( const Color& color ) { m_color = color; }
	void SetHealth( unsigned char health ) { m_health = health; }
	void SetID( unsigned char newID ) { m_playerID = newID; }
	void SetItStatus( bool itStatus ) { m_isIt = itStatus; }
	void SetScore( unsigned char score ) { m_score = score; }

	void SetClientPosition( float x, float y );
	void SetClientVelocity( float vx, float vy );
	void SetClientAcceleration( float ax, float ay );
	void SetClientOrientation( float orientationDegrees ) { m_clientOrientationDegrees = orientationDegrees; }

	void SetServerPosition( float x, float y );
	void SetServerVelocity( float vx, float vy );
	void SetServerAcceleration( float ax, float ay );
	void SetServerOrientation( float orientationDegrees ) { m_serverOrientationDegrees = orientationDegrees; }

private:
	unsigned char m_playerID;

	bool m_isIt;
	Color m_color;
	unsigned char m_health;
	unsigned char m_score;

	Vector2 m_clientPosition;
	Vector2 m_clientVelocity;
	Vector2 m_clientAcceleration;
	float m_clientOrientationDegrees;

	Vector2 m_serverPosition;
	Vector2 m_serverVelocity;
	Vector2 m_serverAcceleration;
	float m_serverOrientationDegrees;
};



//-----------------------------------------------------------------------------------------------
inline Entity::Entity()
	: m_playerID( 0 )
	, m_isIt( false )
	, m_clientOrientationDegrees( 0 )
	, m_serverOrientationDegrees( 0 )
	, m_color( (unsigned char)0, 0, 0, 255 )
{ }

//-----------------------------------------------------------------------------------------------
inline void Entity::SetClientPosition( float x, float y )
{
	m_clientPosition.x = x;
	m_clientPosition.y = y;
}

//-----------------------------------------------------------------------------------------------
inline void Entity::SetClientVelocity( float vx, float vy )
{
	m_clientVelocity.x = vx;
	m_clientVelocity.y = vy;
}

//-----------------------------------------------------------------------------------------------
inline void Entity::SetClientAcceleration( float ax, float ay )
{
	m_clientAcceleration.x = ax;
	m_clientAcceleration.y = ay;
}

//-----------------------------------------------------------------------------------------------
inline void Entity::SetServerPosition( float x, float y )
{
	m_serverPosition.x = x;
	m_serverPosition.y = y;
}

//-----------------------------------------------------------------------------------------------
inline void Entity::SetServerVelocity( float vx, float vy )
{
	m_serverVelocity.x = vx;
	m_serverVelocity.y = vy;
}

//-----------------------------------------------------------------------------------------------
inline void Entity::SetServerAcceleration( float ax, float ay )
{
	m_serverAcceleration.x = ax;
	m_serverAcceleration.y = ay;
}

#endif //INCLUDED_ENTITY_HPP
