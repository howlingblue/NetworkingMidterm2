#pragma once
#ifndef INCLUDED_TENDON_HPP
#define INCLUDED_TENDON_HPP

//-----------------------------------------------------------------------------------------------
struct Tendon
{
	Tendon()
		: boneIndex( 0 )
		, boneWeight( 0.f )
	{ }

	Tendon( int nodeIndex, float weight )
		: boneIndex( nodeIndex )
		, boneWeight( weight )
	{ }

	bool operator<( const Tendon& rhs ) const
	{
		return boneWeight < rhs.boneWeight;
	}

	bool operator>( const Tendon& rhs ) const
	{
		return boneWeight > rhs.boneWeight;
	}

	int boneIndex;
	float boneWeight;
};

#endif //INCLUDED_TENDON_HPP
