#ifndef INCLUDED_AABB_HPP
#define INCLUDED_AABB_HPP

//-----------------------------------------------------------------------------------------------
#include "Math/FloatVector2.hpp"

//-----------------------------------------------------------------------------------------------
struct AABB
{
	FloatVector2 m_min, m_max;

	AABB();
	AABB( float minX, float minY, float maxX, float maxY );
	AABB( const FloatVector2& min, const FloatVector2& max );
};

#endif //INCLUDED_AABB_HPP
