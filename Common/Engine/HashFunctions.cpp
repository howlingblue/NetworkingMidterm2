#include "HashFunctions.hpp"

#pragma region DJB2
//-----------------------------------------------------------------------------------------------
Hash HashWithDJB2( unsigned char* string )
{
	Hash hash = 5381;
	int c;

	while (c = *string++)
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

//-----------------------------------------------------------------------------------------------
Hash HashWithDJB2( unsigned char* buffer, unsigned int bufferSize )
{
	Hash hash = 5381;
	int c;
	unsigned int bufferPosition = 0;

	while ( bufferPosition < bufferSize )
	{
		c = *buffer++;
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		++bufferPosition;
	}

	return hash;
}
#pragma endregion



#pragma region Eiserloh
//-----------------------------------------------------------------------------------------------
Hash HashWithEiserloh( unsigned char* string )
{
	Hash hashValue = 0;

	for( unsigned int i = 0; string[ i ] != '\0'; ++i )
	{
		hashValue &= 0x07ffffff;
		hashValue *= 31;
		hashValue += string[ i ];
	}
	return hashValue;
}

//-----------------------------------------------------------------------------------------------
Hash HashWithEiserloh( unsigned char* buffer, unsigned int bufferSize )
{
	Hash hashValue = 0;
	for( unsigned int i = 0; i < bufferSize; ++i )
	{
		hashValue &= 0x07ffffff;
		hashValue *= 31;
		hashValue += buffer[ i ];
	}
	return hashValue;
}
#pragma endregion



#pragma region Hsieh
// This function currently unimplemented (link to eventual implementation is in header)
//-----------------------------------------------------------------------------------------------
Hash HashWithHsieh( unsigned char* /*string*/ )
{
	return 0;
}

//-----------------------------------------------------------------------------------------------
Hash HashWithHsieh( unsigned char* /*buffer*/, unsigned int /*bufferSize*/ )
{
	return 0;
}
#pragma endregion



#pragma region SDBM
//-----------------------------------------------------------------------------------------------
Hash HashWithSDBM( unsigned char* string )
{
	unsigned long hash = 0;
	int c;

	while (c = *string++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

//-----------------------------------------------------------------------------------------------
Hash HashWithSDBM( unsigned char* buffer, unsigned int bufferSize )
{
	unsigned long hash = 0;
	int c;
	unsigned int bufferPosition = 0;

	while ( bufferPosition < bufferSize )
	{
		c = *buffer++;
		hash = c + (hash << 6) + (hash << 16) - hash;
		++bufferPosition;
	}

	return hash;
}
#pragma endregion
