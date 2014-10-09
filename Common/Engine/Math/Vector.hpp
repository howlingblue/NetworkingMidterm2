#ifndef INCLUDED_VECTOR_HPP
#define INCLUDED_VECTOR_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <cassert>
#include "../Constraints.hpp"

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
class Vector : Can_Add<T, T, T>, Can_Multiply<T, T, T>
{
protected:
	T m_data[ SIZE ];

public:
	Vector();

	T GetSquaredNorm() const;

	T& operator[]( unsigned int i );
	const T& operator[]( unsigned int i ) const;

	Vector<T, SIZE> operator-();

	Vector<T, SIZE>& operator+=( const Vector<T, SIZE>& rhs );
	Vector<T, SIZE>& operator*=( const T& alpha );

	//Other defined operations:
	//Vector<T, SIZE> operator+( const Vector<T, SIZE>& u, const Vector<T, SIZE>& v );
	//Vector<T, SIZE> operator*( const Vector<T, SIZE>& u, const T& alpha );
	//Vector<T, SIZE> operator*( const T& alpha, const Vector<T, SIZE>& u );
	//Vector<T, 3> CrossProduct( const Vector<T, 3>& u, const Vector<T, 3>& v );
	//T DotProduct( const Vector<T, SIZE>& u, const Vector<T, SIZE>& v ) 
};

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
Vector<T, SIZE>::Vector()
{
	assert( SIZE != 0 );
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		m_data[i] = T();
	}
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline T Vector<T, SIZE>::GetSquaredNorm() const
{
	T result = T();
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result += m_data[i] * m_data[i];
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline T& Vector<T, SIZE>::operator[]( unsigned int i )
{ 
	assert( i < SIZE );
	return m_data[ i ]; 
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline const T& Vector<T, SIZE>::operator[]( unsigned int i ) const
{
	assert( i < SIZE );
	return m_data[ i ];
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE> Vector<T, SIZE>::operator-()
{
	Vector<T, SIZE> result;
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result[i] = -m_data[i];
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE>& Vector<T, SIZE>::operator+=( const Vector<T, SIZE>& rhs ) 
{
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		m_data[i] += rhs[i];
	}
	return *this;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE>& Vector<T, SIZE>::operator*=( const T& alpha ) 
{
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		m_data[i] *= alpha;
	}
	return *this;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE> operator+( const Vector<T, SIZE>& u, const Vector<T, SIZE>& v ) 
{
	Vector<T, SIZE> result = u;
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result[i] += v[i];
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE> operator-( const Vector<T, SIZE>& u, const Vector<T, SIZE>& v ) 
{
	Vector<T, SIZE> result = u;
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result[i] -= v[i];
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE> operator*( const Vector<T, SIZE>& u, const T& alpha ) 
{
	Vector<T, SIZE> result = u;
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result[i] *= alpha;
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline Vector<T, SIZE> operator*( const T& alpha, const Vector<T, SIZE>& u ) 
{
	Vector<T, SIZE> result = u;
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result[i] *= alpha;
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T>
inline Vector<T, 3> CrossProduct( const Vector<T, 3>& u, const Vector<T, 3>& v ) 
{
	Vector<T, 3> result;
	result[0] = u[1] * v[2] - u[2] * v[1];
	result[1] = u[2] * v[0] - u[0] * v[2];
	result[2] = u[0] * v[1] - u[1] * v[0];
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int SIZE>
inline T DotProduct( const Vector<T, SIZE>& u, const Vector<T, SIZE>& v ) 
{
	T result = T();
	for( unsigned int i = 0; i < SIZE; ++i )
	{
		result += u[i] * v[i];
	}
	return result;
}

#endif //INCLUDED_VECTOR_HPP
