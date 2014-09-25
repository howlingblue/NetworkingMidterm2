#pragma once
#ifndef INCLUDED_CHARACTER_HPP
#define INCLUDED_CHARACTER_HPP

//-----------------------------------------------------------------------------------------------
#include "../../../Common/Engine/Color.hpp"
#include "../../../Common/Engine/Vector2.hpp"

//-----------------------------------------------------------------------------------------------
class Character
{
public:
	Character();
	~Character();

	//Lifecycle
	void Render() const;
	void Update( float deltaSeconds );

	//Gets/Sets
	const Color& GetColor() const { return m_color; }
	const Vector2& GetCurrentPosition() const { return m_currentPosition; }

	void SetColor( unsigned char red, unsigned char green, unsigned char blue );
	void SetPositionOnServer( const Vector2& positionOnServer ) { m_serverPosition = positionOnServer; }
	void SetCurrentPosition( const Vector2& newCurrentPosition ) { m_currentPosition = newCurrentPosition; }

private:
	Color m_color;

	Vector2 m_currentPosition;
	Vector2 m_serverPosition;
};

//-----------------------------------------------------------------------------------------------
inline Character::Character()
	: m_color( 0.f, 0.f, 0.f, 1.f )
	, m_currentPosition( -100.f, -100.f )
	, m_serverPosition( m_currentPosition )
{

}

//-----------------------------------------------------------------------------------------------
inline Character::~Character()
{

}



//-----------------------------------------------------------------------------------------------
inline void Character::SetColor( unsigned char red, unsigned char green, unsigned char blue )
{
	m_color.SetColor( red, green, blue, 255 );
}

#endif //INCLUDED_CHARACTER_HPP
