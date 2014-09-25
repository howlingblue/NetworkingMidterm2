#include <math.h>
#include "Vector2.hpp"

//----------------------------------------------------------------------------------------------------
Vector2::Vector2() 
	: x(0)
	, y(0)
{ }

//----------------------------------------------------------------------------------------------------
Vector2::Vector2(float initialX, float initialY) 
	: x( initialX )
	, y( initialY )
{ }

//----------------------------------------------------------------------------------------------------
float Vector2::DotProduct( const Vector2& rhs) const
{
	return this->x * rhs.x + this->y * rhs.y;
}

//----------------------------------------------------------------------------------------------------
float Vector2::Length() const
{
	return sqrt( ( x * x ) + ( y * y ) );
}

//----------------------------------------------------------------------------------------------------
void Vector2::Normalize()
{
	float norm = this->Length();
	if( norm == 0.f )
		return;
	*this /= norm;
}

//----------------------------------------------------------------------------------------------------
float Vector2::SquaredDistanceTo( const Vector2& rhs ) const
{
	float newX = rhs.x - this->x;
	float newY = rhs.y - this->y;
	return ( newX * newX ) + ( newY * newY );
}

//----------------------------------------------------------------------------------------------------
Vector2& Vector2::operator+= ( const Vector2& rhs ) 
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Vector2 Vector2::operator+ (const Vector2& rhs) const
{
	Vector2 result = *this;
	result += rhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2& Vector2::operator-= ( const Vector2& rhs ) 
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Vector2 Vector2::operator- (const Vector2& rhs) const
{
	Vector2 result = *this;
	result -= rhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2& Vector2::operator*= ( const Vector2& rhs ) 
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Vector2 Vector2::operator* (const Vector2& rhs) const
{
	Vector2 result = *this;
	result *= rhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2& Vector2::operator/= ( const Vector2& rhs ) 
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Vector2 Vector2::operator/ (const Vector2& rhs) const
{
	Vector2 result = *this;
	result /= rhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2& Vector2::operator*= ( float rhs ) 
{
	this->x *= rhs;
	this->y *= rhs;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Vector2 Vector2::operator* ( float rhs ) const
{
	Vector2 result = *this;
	result *= rhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2& Vector2::operator/= ( float rhs ) 
{
	this->x /= rhs;
	this->y /= rhs;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Vector2 Vector2::operator/ ( float rhs ) const
{
	Vector2 result = *this;
	result /= rhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2 operator* ( float lhs, const Vector2& rhs)
{
	Vector2 result = rhs;
	result *= lhs;
	return result;
}

//----------------------------------------------------------------------------------------------------
Vector2 operator/ ( float lhs, const Vector2& rhs)
{
	Vector2 result = rhs;
	result.x = lhs / result.x;
	result.y = lhs / result.y;
	return result;
}
