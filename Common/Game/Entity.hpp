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

	const Color& GetColor() const { return m_color; }
	float GetCurrentOrientation() const { return m_clientOrientationDegrees; }
	const Vector2& GetCurrentPosition() const { return m_clientPosition; }
	const Vector2& GetCurrentVelocity() const { return m_clientVelocity; }
	unsigned char GetID() const { return m_playerID; }
	bool IsIt() const { return m_isIt; }

	void SetID( unsigned char newID ) { m_playerID = newID; }
	void SetItStatus( bool itStatus ) { m_isIt = itStatus; }

	void SetClientPosition( float x, float y );
	void SetClientVelocity( float vx, float vy );
	void SetClientOrientation( float orientationDegrees ) { m_clientOrientationDegrees = orientationDegrees; }

	void SetServerPosition( float x, float y );
	void SetServerVelocity( float vx, float vy );
	void SetServerOrientation( float orientationDegrees ) { m_serverOrientationDegrees = orientationDegrees; }

private:
	unsigned char m_playerID;
	bool m_isIt;
	Color m_color;
	Vector2 m_clientPosition;
	Vector2 m_clientVelocity;
	float m_clientOrientationDegrees;

	Vector2 m_serverPosition;
	Vector2 m_serverVelocity;
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

#endif //INCLUDED_ENTITY_HPP
