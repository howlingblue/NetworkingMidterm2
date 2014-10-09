#include "AABB.hpp"

//-----------------------------------------------------------------------------------------------
AABB::AABB()
{ }

//-----------------------------------------------------------------------------------------------
AABB::AABB( float minX, float minY, float maxX, float maxY )
	: m_min( minX, minY )
	, m_max( maxX, maxY )
{ }

//-----------------------------------------------------------------------------------------------
AABB::AABB( const FloatVector2& min, const FloatVector2& max )
	: m_min( min )
	, m_max( max )
{ }
