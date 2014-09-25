#pragma once
#ifndef INCLUDED_PLAYER_HPP
#define INCLUDED_PLAYER_HPP

#include "../Engine/Color.hpp"
#include "../Engine/Vector2.hpp"

//-----------------------------------------------------------------------------------------------
class Player
{
public:
	Player();
	~Player() {}

	void Render() const;
	void Update( float deltaSeconds );

	const Color& GetColor() { return m_color; }
	float GetCurrentOrientation() { return m_clientOrientationDegrees; }
	const Vector2& GetCurrentPosition() { return m_clientPosition; }
	const Vector2& GetCurrentVelocity() { return m_clientVelocity; }
	unsigned short GetID() const { return m_playerID; }

	void SetID( unsigned short newID ) { m_playerID = newID; }

	void SetClientPosition( float x, float y );
	void SetClientVelocity( float vx, float vy );
	void SetClientOrientation( float orientationDegrees ) { m_clientOrientationDegrees = orientationDegrees; }

	void SetServerPosition( float x, float y );
	void SetServerVelocity( float vx, float vy );
	void SetServerOrientation( float orientationDegrees ) { m_serverOrientationDegrees = orientationDegrees; }

private:
	unsigned short m_playerID;
	Color m_color;
	Vector2 m_clientPosition;
	Vector2 m_clientVelocity;
	float m_clientOrientationDegrees;

	Vector2 m_serverPosition;
	Vector2 m_serverVelocity;
	float m_serverOrientationDegrees;
};



//-----------------------------------------------------------------------------------------------
inline Player::Player()
	: m_playerID( 0 )
	, m_clientOrientationDegrees( 0 )
	, m_serverOrientationDegrees( 0 )
	, m_color( (unsigned char)0, 0, 0, 255 )
{ }

//-----------------------------------------------------------------------------------------------
inline void Player::SetClientPosition( float x, float y )
{
	m_clientPosition.x = x;
	m_clientPosition.y = y;
}

//-----------------------------------------------------------------------------------------------
inline void Player::SetClientVelocity( float vx, float vy )
{
	m_clientVelocity.x = vx;
	m_clientVelocity.y = vy;
}

//-----------------------------------------------------------------------------------------------
inline void Player::SetServerPosition( float x, float y )
{
	m_serverPosition.x = x;
	m_serverPosition.y = y;
}

//-----------------------------------------------------------------------------------------------
inline void Player::SetServerVelocity( float vx, float vy )
{
	m_serverVelocity.x = vx;
	m_serverVelocity.y = vy;
}

#endif //INCLUDED_PLAYER_HPP
