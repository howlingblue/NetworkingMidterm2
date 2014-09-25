#ifndef INCLUDED_VECTOR_2_HPP
#define INCLUDED_VECTOR_2_HPP
#pragma once

//----------------------------------------------------------------------------------------------------
struct Vector2 { 
	float x, y; 

	Vector2();
	Vector2( float initialX, float initialY );

	float DotProduct( const Vector2& rhs) const;
	float Length() const;
	void Normalize();
	float SquaredDistanceTo( const Vector2& rhs ) const;

	Vector2& operator+= ( const Vector2& rhs );
	Vector2  operator+  ( const Vector2& rhs ) const;
	Vector2& operator-= ( const Vector2& rhs );
	Vector2  operator-  ( const Vector2& rhs ) const;
	Vector2& operator*= ( const Vector2& rhs );
	Vector2  operator*  ( const Vector2& rhs ) const;
	Vector2& operator/= ( const Vector2& rhs );
	Vector2  operator/  ( const Vector2& rhs ) const;
	Vector2& operator*= ( float rhs );
	Vector2  operator*  ( float rhs ) const;
	Vector2& operator/= ( float rhs );
	Vector2  operator/  ( float rhs ) const;
};

Vector2 operator* ( float lhs, const Vector2& rhs);
Vector2 operator/ ( float lhs, const Vector2& rhs);

#endif //INCLUDED_VECTOR_2_HPP
