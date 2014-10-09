#ifndef INCLUDED_VECTOR_3_HPP
#define INCLUDED_VECTOR_3_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <math.h>
#include "../EngineMacros.hpp"
#include "Vector.hpp"
#include "Vector2.hpp"

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
struct Vector3
{
	//members
	NumericType x;
	NumericType y;
	NumericType z;

	Vector3()
		: x( 0 )
		, y( 0 )
		, z( 0 )
	{ }

	Vector3( const NumericType& X, const NumericType& Y, const NumericType& Z )
		: x( X )
		, y( Y )
		, z( Z )
	{ }

	explicit Vector3( Vector< NumericType, 3 > vector3 )
		: x( vector3[0] )
		, y( vector3[1] )
		, z( vector3[2] )
	{ }

	explicit Vector3( Vector2< NumericType > vector2 )
		: x( vector2[0] )
		, y( vector2[1] )
		, z( 0 )
	{ }

	NumericType CalculateNorm() const;
	NumericType CalculateSquaredNorm() const;
	void Normalize();
	
	NumericType& operator[]( unsigned int i );
	const NumericType& operator[]( unsigned int i ) const;

	bool operator==( const Vector3<NumericType>& rhs ) const;
	bool operator!=( const Vector3<NumericType>& rhs ) const;

	Vector3<NumericType> operator-();

	Vector3<NumericType>& operator+=( const Vector3<NumericType>& rhs );
	Vector3<NumericType>& operator-=( const Vector3<NumericType>& rhs );
	Vector3<NumericType>& operator*=( const NumericType& alpha );

	//Other defined operations:
	//const Vector3<NumericType> operator+( const Vector3<NumericType>& u, const Vector3<NumericType>& v );
	//const Vector3<NumericType> operator-( const Vector3<NumericType>& u, const Vector3<NumericType>& v );
	//const Vector3<NumericType> operator*( const Vector3<NumericType>& u, const NumericType& alpha );
	//const Vector3<NumericType> operator*( const NumericType& alpha, const Vector3<NumericType>& u );
	//const Vector3<NumericType> operator/( const Vector3<NumericType>& u, const NumericType& alpha );
	//const Vector3<NumericType> ComponentwiseProduct( const Vector3<NumericType>& u, const Vector3<NumericType>& v );
	//const Vector3<NumericType> CrossProduct( const Vector3<NumericType>& u, const Vector3<NumericType>& v );
	//const NumericType DotProduct( const Vector3<NumericType>& u, const Vector3<NumericType>& v ) 
};





//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline NumericType Vector3<NumericType>::CalculateNorm() const
{
	return sqrt( CalculateSquaredNorm() );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline NumericType Vector3<NumericType>::CalculateSquaredNorm() const
{
	return ( x * x ) + ( y * y ) + ( z * z );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline void Vector3<NumericType>::Normalize()
{
	NumericType norm = this->CalculateNorm();
	if( norm == 0.f )
		return;

	x /= norm;
	y /= norm;
	z /= norm;

	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( this->CalculateNorm(), 1.f ) ) );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
NumericType& Vector3<NumericType>::operator[]( unsigned int i )
{
	assert( i < 3 );
	return *(&x + i );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
const NumericType& Vector3<NumericType>::operator[]( unsigned int i ) const
{
	assert( i < 3 );
	return *(&x + i );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline bool Vector3<NumericType>::operator==( const Vector3<NumericType>& rhs ) const
{
	if( this->x == rhs.x && this->y == rhs.y && this->z == rhs.z )
		return true;
	return false;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline bool Vector3<NumericType>::operator!=( const Vector3<NumericType>& rhs ) const
{
	return !( *this == rhs );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector3<NumericType> Vector3<NumericType>::operator-()
{
	Vector3<NumericType> result;

	result.x = -this->x;
	result.y = -this->y;
	result.z = -this->z;

	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector3<NumericType>& Vector3<NumericType>::operator+=( const Vector3<NumericType>& rhs ) 
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector3<NumericType>& Vector3<NumericType>::operator-=( const Vector3<NumericType>& rhs )
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector3<NumericType>& Vector3<NumericType>::operator*=( const NumericType& alpha )
{
	this->x *= alpha;
	this->y *= alpha;
	this->z *= alpha;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> operator+( const Vector3<NumericType>& u, const Vector3<NumericType>& v ) 
{
	Vector3<NumericType> result = u;
	result.x += v.x;
	result.y += v.y;
	result.z += v.z;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> operator-( const Vector3<NumericType>& u, const Vector3<NumericType>& v )
{
	Vector3<NumericType> result = u;
	result.x -= v.x;
	result.y -= v.y;
	result.z -= v.z;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> operator*( const Vector3<NumericType>& u, const NumericType& alpha )
{
	Vector3<NumericType> result = u;
	result.x *= alpha;
	result.y *= alpha;
	result.z *= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> operator*( const NumericType& alpha, const Vector3<NumericType>& u )
{
	Vector3<NumericType> result = u;
	result.x *= alpha;
	result.y *= alpha;
	result.z *= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> operator/( const Vector3<NumericType>& u, const NumericType& alpha )
{
	Vector3<NumericType> result = u;
	result.x /= alpha;
	result.y /= alpha;
	result.z /= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> ComponentwiseProduct( const Vector3<NumericType>& u, const Vector3<NumericType>& v )
{
	return Vector3<NumericType>( u.x * v.x, u.y * v.y, u.z * v.z );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const NumericType DotProduct( const Vector3<NumericType>& u, const Vector3<NumericType>& v )
{
	return ( u.x * v.x ) + ( u.y * v.y ) + ( u.z * v.z );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector3<NumericType> CrossProduct( const Vector3<NumericType>& u, Vector3<NumericType>& v )
{
	Vector3<NumericType> result;
	result.x = u.y * v.z - u.z * v.y;
	result.y = u.z * v.x - u.x * v.z;
	result.z = u.x * v.y - u.y * v.x;

	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( DotProduct( result, u ), 0.f ) ) );
	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( DotProduct( result, v ), 0.f ) ) );
	return result;
}


#endif //INCLUDED_VECTOR_3_HPP
