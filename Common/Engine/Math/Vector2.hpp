#ifndef INCLUDED_VECTOR_2_HPP
#define INCLUDED_VECTOR_2_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <math.h>
#include "../EngineMacros.hpp"
#include "Vector.hpp"

//-----------------------------------------------------------------------------------------------
template < typename T >
struct Vector2
{
	//members
	T x;
	T y;

	Vector2()
		: x( 0 )
		, y( 0 )
	{ }

	Vector2( const T& X, const T& Y )
		: x( X )
		, y( Y )
	{ }

	explicit Vector2( const Vector< T, 2 >& vectorTypeName2 )
		: x( vectorTypeName2[0] )
		, y( vectorTypeName2[1] )
	{ }

	T CalculateNorm() const;
	T CalculateSquaredNorm() const;
	void Normalize();

	T& operator[]( unsigned int i );
	const T& operator[]( unsigned int i ) const;

	bool operator==( const Vector2<T>& rhs ) const;
	bool operator!=( const Vector2<T>& rhs ) const;
	bool operator< ( const Vector2<T>& rhs ) const;

	Vector2<T> operator-();

	Vector2<T>& operator+=( const Vector2<T>& rhs );
	Vector2<T>& operator-=( const Vector2<T>& rhs );
	Vector2<T>& operator*=( const T& alpha );

	//Other defined operations:
	//const Vector2<T> operator+( const Vector2<T>& u,	const Vector2<T>& v );
	//const Vector2<T> operator-( const Vector2<T>& u,	const Vector2<T>& v );
	//const Vector2<T> operator*( const Vector2<T>& u,	const T& alpha );
	//const Vector2<T> operator*( const T& alpha,		const Vector2<T>& u );
	//const Vector2<T> operator/( const Vector2<T>& u,	const T& alpha );
	//const Vector2<T> operator/( const T& alpha,		const Vector2<T>& u );
	//const Vector2<T> MultiplyComponents( const Vector2<T>& u, const Vector2<T>& v );
	//const T DotProduct( const Vector2<T>& u,			const Vector2<T>& v ) 
};





//-----------------------------------------------------------------------------------------------
template < typename T >
inline T Vector2<T>::CalculateNorm() const
{
	return sqrt( CalculateSquaredNorm() );
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline T Vector2<T>::CalculateSquaredNorm() const
{
	return ( x * x ) + ( y * y );
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline void Vector2<T>::Normalize()
{
	T norm = this->CalculateNorm();
	if( norm == 0.f )
		return;

	x /= norm;
	y /= norm;

	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( this->CalculateNorm(), 1.f ) ) );
}

//-----------------------------------------------------------------------------------------------
template < typename T >
T& Vector2< T >::operator[]( unsigned int i )
{
	assert( i < 2 );
	return *(&x + i );
}

//-----------------------------------------------------------------------------------------------
template < typename T >
const T& Vector2< T >::operator[]( unsigned int i ) const
{
	assert( i < 2 );
	return *(&x + i );
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline bool Vector2<T>::operator==( const Vector2<T>& rhs ) const
{
	if( this->x == rhs.x && this->y == rhs.y  )
		return true;
	return false;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline bool Vector2<T>::operator!=( const Vector2<T>& rhs ) const
{
	return !( *this == rhs );
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline bool Vector2<T>::operator< ( const Vector2<T>& rhs ) const
{
	if( this->x < rhs.x )
	{
		return true;
	}
	else if ( this->x == rhs.x )
	{
		if( this->y < rhs.y )
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline Vector2<T> Vector2<T>::operator-()
{
	Vector2<T> result;

	result.x = -this->x;
	result.y = -this->y;

	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline Vector2<T>& Vector2<T>::operator+=( const Vector2<T>& rhs ) 
{
	this->x += rhs.x;
	this->y += rhs.y;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline Vector2<T>& Vector2<T>::operator-=( const Vector2<T>& rhs )
{
	this->x -= rhs.x;
	this->y -= rhs.y;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline Vector2<T>& Vector2<T>::operator*=( const T& alpha )
{
	this->x *= alpha;
	this->y *= alpha;

	return *this;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> operator+( const Vector2<T>& u, const Vector2<T>& v ) 
{
	Vector2<T> result = u;
	result.x += v.x;
	result.y += v.y;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> operator-( const Vector2<T>& u, const Vector2<T>& v )
{
	Vector2<T> result = u;
	result.x -= v.x;
	result.y -= v.y;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> operator*( const Vector2<T>& u, const T& alpha )
{
	Vector2<T> result = u;
	result.x *= alpha;
	result.y *= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> operator*( const T& alpha, const Vector2<T>& u )
{
	Vector2<T> result = u;
	result.x *= alpha;
	result.y *= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> operator/( const Vector2<T>& u, const T& alpha )
{
	Vector2<T> result = u;
	result.x /= alpha;
	result.y /= alpha;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> operator/( const T& alpha, const Vector2<T>& u )
{
	Vector2<T> result = u;
	result.x = alpha / u.x;
	result.y = alpha / u.y;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const Vector2<T> MultiplyComponents( const Vector2<T>& u, const Vector2<T>& v )
{
	Vector2<T> result;
	result.x = u.x * v.x;
	result.y = u.y * v.y;
	return result;
}

//-----------------------------------------------------------------------------------------------
template < typename T >
inline const T DotProduct( const Vector2<T>& u, const Vector2<T>& v )
{
	return ( u.x * v.x ) + ( u.y * v.y );
}

#endif //INCLUDED_VECTOR_2_HPP
