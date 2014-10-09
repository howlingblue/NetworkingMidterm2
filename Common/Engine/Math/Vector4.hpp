#ifndef INCLUDED_VECTOR_4_HPP
#define INCLUDED_VECTOR_4_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <math.h>
#include "../EngineMacros.hpp"
#include "EngineMath.hpp"
#include "Vector.hpp"
#include "Vector3.hpp"

//-----------------------------------------------------------------------------------------------
template < typename T >
struct Vector4
{
	typedef T NumericType;

	//members
	NumericType x;
	NumericType y;
	NumericType z;
	NumericType w;

	Vector4()
		: x( 0 )
		, y( 0 )
		, z( 0 )
		, w( 1 )
	{ }

	Vector4( const NumericType& X, const NumericType& Y, const NumericType& Z )
		: x( X )
		, y( Y )
		, z( Z )
		, w( 1 )
	{ }

	Vector4( const NumericType& X, const NumericType& Y, const NumericType& Z, const NumericType& W )
		: x( X )
		, y( Y )
		, z( Z )
		, w( W )
	{ }

	//Assume that a conversion from vec3 just wants a homogeneous coordinate in the fourth coordinate
	explicit Vector4( Vector3< NumericType > vec3 )
		: x( vec3.x )
		, y( vec3.y )
		, z( vec3.z )
		, w( 1 )
	{ }

	//Assume that a conversion from vec3 just wants a homogeneous coordinate in the fourth coordinate
	explicit Vector4( Vector< NumericType, 3 > vec3 )
		: x( vec3[0] )
		, y( vec3[1] )
		, z( vec3[2] )
		, w( 1 )
	{ }

	explicit Vector4( Vector< NumericType, 4 > vec4 )
		: x( vec4[0] )
		, y( vec4[1] )
		, z( vec4[2] )
		, w( vec4[3] )
	{ }

	//Functions defined for use in some specific situations.
	NumericType CalculateNorm() const;
	void Normalize();

	//Functions with implementation provided
	NumericType CalculateSquaredNorm() const;

	NumericType& operator[]( unsigned int i );
	const NumericType& operator[]( unsigned int i ) const;

	bool operator==( const Vector4<NumericType>& rhs ) const;
	bool operator!=( const Vector4<NumericType>& rhs ) const;

	Vector4<NumericType> operator-();

	Vector4<NumericType>& operator+=( const Vector4<NumericType>& rhs );
	Vector4<NumericType>& operator-=( const Vector4<NumericType>& rhs );
	Vector4<NumericType>& operator*=( const NumericType& alpha );

	//Other defined operations:
	//const Vector4<NumericType> operator+( const Vector4<NumericType>& u, const Vector4<NumericType>& v );
	//const Vector4<NumericType> operator-( const Vector4<NumericType>& u, const Vector4<NumericType>& v );
	//const Vector4<NumericType> operator*( const Vector4<NumericType>& u, const NumericType& alpha );
	//const Vector4<NumericType> operator*( const NumericType& alpha, const Vector4<NumericType>& u );
	//const Vector4<NumericType> CrossProduct( const Vector4<NumericType>& u, const Vector4<NumericType>& v );
	//const NumericType DotProduct( const Vector4<NumericType>& u, const Vector4<NumericType>& v ) 
};






//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline NumericType Vector4<NumericType>::CalculateSquaredNorm() const
{
	return ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
NumericType& Vector4<NumericType>::operator[]( unsigned int i )
{
	assert( i < 4 );
	return *(&x + i );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
const NumericType& Vector4<NumericType>::operator[]( unsigned int i ) const
{
	assert( i < 4 );
	return *(&x + i );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline bool Vector4<NumericType>::operator==( const Vector4<NumericType>& rhs ) const
{
	if( this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w )
		return true;
	return false;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline bool Vector4<NumericType>::operator!=( const Vector4<NumericType>& rhs ) const
{
	return !( *this == rhs );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector4<NumericType> Vector4<NumericType>::operator-()
{
	Vector4<NumericType> result;

	result.x = -this->x;
	result.y = -this->y;
	result.z = -this->z;

	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector4<NumericType>& Vector4<NumericType>::operator+=( const Vector4<NumericType>& rhs ) 
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector4<NumericType>& Vector4<NumericType>::operator-=( const Vector4<NumericType>& rhs )
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline Vector4<NumericType>& Vector4<NumericType>::operator*=( const NumericType& alpha )
{
	this->x *= alpha;
	this->y *= alpha;
	this->z *= alpha;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector4<NumericType> operator+( const Vector4<NumericType>& u, const Vector4<NumericType>& v ) 
{
	Vector4<NumericType> result = u;
	result.x += v.x;
	result.y += v.y;
	result.z += v.z;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector4<NumericType> operator-( const Vector4<NumericType>& u, const Vector4<NumericType>& v )
{
	Vector4<NumericType> result = u;
	result.x -= v.x;
	result.y -= v.y;
	result.z -= v.z;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector4<NumericType> operator*( const Vector4<NumericType>& u, const NumericType& alpha )
{
	Vector4<NumericType> result = u;
	result.x *= alpha;
	result.y *= alpha;
	result.z *= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector4<NumericType> operator*( const NumericType& alpha, const Vector4<NumericType>& u )
{
	Vector4<NumericType> result = u;
	result.x *= alpha;
	result.y *= alpha;
	result.z *= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const NumericType DotProduct( const Vector4<NumericType>& u, const Vector4<NumericType>& v )
{
	return ( u.x * v.x ) + ( u.y * v.y ) + ( u.z * v.z ) + ( u.w * v.w );
}

//-----------------------------------------------------------------------------------------------
template < typename NumericType >
inline const Vector4<NumericType> CrossProduct( const Vector4<NumericType>& u, Vector4<NumericType>& v )
{
	Vector4<NumericType> result;
	result.x = u.y * v.z - u.z * v.y;
	result.y = u.z * v.x - u.x * v.z;
	result.z = u.x * v.y - u.y * v.x;

	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( DotProduct( result, u ), 0.f ) ) );
	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( DotProduct( result, v ), 0.f ) ) );
	return result;
}


#endif //INCLUDED_VECTOR_4_HPP
