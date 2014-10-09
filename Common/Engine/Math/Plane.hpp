#ifndef INCLUDED_PLANE_HPP
#define INCLUDED_PLANE_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "FloatVector3.hpp"

//-----------------------------------------------------------------------------------------------
struct Plane
{
	float distanceToOrigin;
	FloatVector3 normal;

	Plane()
		: distanceToOrigin( 0 )
	{ }

	Plane( float distance, const FloatVector3& norm )
		: distanceToOrigin( distance )
		, normal( norm )
	{
		normal.Normalize();
	}

	float CalculateDistanceToPoint(const FloatVector3& point ) const;
};

//-----------------------------------------------------------------------------------------------
inline float Plane::CalculateDistanceToPoint(const FloatVector3& point ) const
{
	return ( normal.x * point.x ) + ( normal.y * point.y ) + ( normal.z * point.z ) + distanceToOrigin;
}

#endif //INCLUDED_PLANE_HPP
