#ifndef INCLUDED_BITMASKING_HPP
#define INCLUDED_BITMASKING_HPP

template < typename PrimitiveNumericType >
inline void SetBitInMask( const PrimitiveNumericType& bit, PrimitiveNumericType& mask )
{
	mask |= bit;
}

template < typename PrimitiveNumericType >
inline void UnsetBitInMask( const PrimitiveNumericType& bit, PrimitiveNumericType& mask )
{
	mask &= ~bit;
}

template < typename PrimitiveNumericType >
inline bool ToggleBitInMask( const PrimitiveNumericType& bit, PrimitiveNumericType& mask )
{
	mask ^= bit;
}

template < typename PrimitiveNumericType >
inline bool IsBitSetInMask( const PrimitiveNumericType& bit, PrimitiveNumericType& mask )
{
	return ( mask & bit ) != 0;
}

//This is a cleaned-up, templatized version of Brian Kernighan's bit counting method.
//See the second response at: http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
template < typename PrimitiveNumericType >
inline PrimitiveNumericType CalculateNumberOfBitsSetIn( PrimitiveNumericType mask ) 
{     
	PrimitiveNumericType numberOfSetBits; 

	for (numberOfSetBits = 0; mask; ++numberOfSetBits ) 
	{
		mask &= mask - 1; //clear least significant set bit: 10100b & 10011b = 10000b
	}

	return numberOfSetBits;
}

#endif //INCLUDED_BITMASKING_HPP