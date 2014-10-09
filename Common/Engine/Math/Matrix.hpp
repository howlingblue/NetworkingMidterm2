#ifndef INCLUDED_MATRIX_HPP
#define INCLUDED_MATRIX_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <cassert>
#include "../Constraints.hpp"

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
class Matrix //: Can_Add<T, T, T>, Can_Multiply<T, T, T>
{
protected:
	T m_data[ COLUMN_SIZE * ROW_SIZE ];

public:
	Matrix();
	explicit Matrix( const T* length16Array );

	const T* GetRawBuffer() const { return m_data; }

	T& operator[]( unsigned int i );
	const T& operator[]( unsigned int i ) const;

	T& operator() ( unsigned int row, unsigned int column );
	const T& operator() ( unsigned int row, unsigned int column ) const;

	Matrix<T, COLUMN_SIZE, ROW_SIZE>& operator=( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& other );
	Matrix<T, COLUMN_SIZE, ROW_SIZE>  operator-();

	Matrix<T, COLUMN_SIZE, ROW_SIZE>& operator+=( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& rhs );
	Matrix<T, COLUMN_SIZE, ROW_SIZE>& operator*=( const T& alpha );

	//Other defined operations:
	//Matrix<T, COLUMN_SIZE, ROW_SIZE> operator+( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u, const Matrix<T, COLUMN_SIZE, ROW_SIZE>& v );
	//Matrix<T, COLUMN_SIZE, ROW_SIZE> operator*( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u, const T& alpha );
	//Matrix<T, COLUMN_SIZE, ROW_SIZE> operator*( const T& alpha, const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u );
	//Matrix<T, U_COLUMN_SIZE, V_ROW_SIZE> operator*( const Matrix<T, U_COLUMN_SIZE, SHARED_SIZE>& u, const Matrix<T, SHARED_SIZE, V_ROW_SIZE>& v );
	//Matrix<T, ROW_SIZE, COLUMN_SIZE> Transpose( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u );
};




//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
Matrix<T, COLUMN_SIZE, ROW_SIZE>::Matrix()
{
	assert( ROW_SIZE != 0 );
	assert( COLUMN_SIZE != 0 );
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		m_data[i] = T();
	}
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
Matrix<T, COLUMN_SIZE, ROW_SIZE>::Matrix( const T* length16Array )
{
	assert( ROW_SIZE != 0 );
	assert( COLUMN_SIZE != 0 );
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		m_data[i] = length16Array[ i ];
	}
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline T& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator[]( unsigned int i )
{
	assert( i < (COLUMN_SIZE * ROW_SIZE) );
	return m_data[ i ]; 
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline const T& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator[]( unsigned int i ) const
{
	assert( i < (COLUMN_SIZE * ROW_SIZE) );
	return m_data[ i ]; 
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline T& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator() ( unsigned int row, unsigned int column )
{
	assert( row > 0 && row <= ROW_SIZE );
	assert( column > 0 && column <= COLUMN_SIZE );
	return m_data[ ( (row - 1) * COLUMN_SIZE ) + (column - 1) ];
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline const T& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator() ( unsigned int row, unsigned int column ) const
{
	assert( row > 0 && row <= ROW_SIZE );
	assert( column > 0 && column <= COLUMN_SIZE );
	return m_data[ ( (row - 1) * COLUMN_SIZE ) + (column - 1) ];
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE>& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator=( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& other ) 
{
	if ( this == &other ) 
		return *this;

	for( unsigned int i = 0; i < ( COLUMN_SIZE * ROW_SIZE ); ++i )
	{
		m_data[ i ] = other[ i ];
	}

	return *this;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE> Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator-()
{
	Matrix<T, COLUMN_SIZE, ROW_SIZE> result;
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		result[i] = -m_data[i];
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE>& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator+=( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& rhs )
{
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		m_data[i] += rhs[i];
	}
	return *this;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE>& Matrix<T, COLUMN_SIZE, ROW_SIZE>::operator*=( const T& alpha )
{
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		m_data[i] *= alpha;
	}
	return *this;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE> operator+( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u, const Matrix<T, COLUMN_SIZE, ROW_SIZE>& v )
{
	Matrix<T, COLUMN_SIZE, ROW_SIZE> result = u;
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		result[i] += v[i];
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE> operator*( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u, const T& alpha )
{
	Matrix<T, COLUMN_SIZE, ROW_SIZE> result = u;
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		result[i] *= alpha;
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
inline Matrix<T, COLUMN_SIZE, ROW_SIZE> operator*( const T& alpha, const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u )
{
	Matrix<T, COLUMN_SIZE, ROW_SIZE> result = u;
	for( unsigned int i = 0; i < (COLUMN_SIZE * ROW_SIZE); ++i )
	{
		result[i] *= alpha;
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int U_COLUMN_SIZE, unsigned int SHARED_SIZE, unsigned int V_ROW_SIZE>
Matrix<T, U_COLUMN_SIZE, V_ROW_SIZE> operator*( const Matrix<T, U_COLUMN_SIZE, SHARED_SIZE>& u, const Matrix<T, SHARED_SIZE, V_ROW_SIZE>& v )
{
	Matrix<T, U_COLUMN_SIZE, V_ROW_SIZE> result;
	for( unsigned int i = 0; i < U_COLUMN_SIZE; ++i )
	{
		for( unsigned int j = 0; j < V_ROW_SIZE; ++j )
		{
			for( unsigned int k = 0; k < SHARED_SIZE; ++k )
			{
				result[ i * V_ROW_SIZE + j ] += u[ i * SHARED_SIZE + k ] * v[ k * V_ROW_SIZE + j ];
			}
		}
	}
	return result;
}

//-----------------------------------------------------------------------------------------------
template <typename T, unsigned int COLUMN_SIZE, unsigned int ROW_SIZE>
Matrix<T, ROW_SIZE, COLUMN_SIZE> Transpose( const Matrix<T, COLUMN_SIZE, ROW_SIZE>& u )
{
	Matrix<T, ROW_SIZE, COLUMN_SIZE> result;
	for( unsigned int i = 0; i < COLUMN_SIZE; ++i )
	{
		for( unsigned int j = 0; j < ROW_SIZE; ++j )
		{
			result[ i * ROW_SIZE + j ] = u[ j * COLUMN_SIZE + i ];
		}
	}
	return result;
}

#endif // INCLUDED_MATRIX_HPP
