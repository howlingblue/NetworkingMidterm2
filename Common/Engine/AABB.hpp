#ifndef INCLUDED_AABB_HPP
#define INCLUDED_AABB_HPP

//-----------------------------------------------------------------------------------------------
#include "Vector2.hpp"

//-----------------------------------------------------------------------------------------------
struct AABB
{
	Vector2 m_min, m_max;

	AABB();
	AABB( float minX, float minY, float maxX, float maxY );
	AABB( const Vector2& min, const Vector2& max );
};

#endif //INCLUDED_AABB_HPP
