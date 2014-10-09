#pragma once
#ifndef INCLUDED_BONE_HPP
#define INCLUDED_BONE_HPP

//-----------------------------------------------------------------------------------------------
#include "../Math/Float4x4Matrix.hpp"

struct TransformComponent;

//-----------------------------------------------------------------------------------------------
struct Bone
{
	Bone();
	Bone( const Float4x4Matrix& invRestTransform, const TransformComponent* pointerToTransform );

	//Data Members
	Float4x4Matrix inverseRestTransform;
	const TransformComponent* transformPointer;
};



//-----------------------------------------------------------------------------------------------
inline Bone::Bone()
	: transformPointer( nullptr )
{ }

//-----------------------------------------------------------------------------------------------
inline Bone::Bone( const Float4x4Matrix& invRestTransform, const TransformComponent* pointerToTransform )
	: inverseRestTransform( invRestTransform )
	, transformPointer( pointerToTransform )
{ }

#endif //INCLUDED_BONE_HPP
