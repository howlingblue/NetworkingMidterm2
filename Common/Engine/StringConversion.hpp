#ifndef INCLUDED_STRING_CONVERSION_HPP
#define INCLUDED_STRING_CONVERSION_HPP

#include "Math/FloatVector2.hpp"
#include "Math/FloatVector3.hpp"
#include "Math/FloatVector4.hpp"
#include "Math/IntVector2.hpp"
#include "Math/IntVector3.hpp"
#include "Color.hpp"
#include <sstream>
#include <vector>

//-----------------------------------------------------------------------------------------------
void ConvertStringToLowercase( const std::string& casedString, std::string& out_lowercasedString );
void ConvertWstringToLowercase( const std::wstring& casedString, std::wstring& out_lowercasedString );

//-----------------------------------------------------------------------------------------------
void BuildStringVectorFromCommaSeparatedString( std::vector< std::string >& stringVector, const std::string& commaSeparatedString );
void BuildWstringVectorFromCommaSeparatedWstring( std::vector< std::wstring >& wstringVector, const std::wstring& commaSeparatedWstring );

//-----------------------------------------------------------------------------------------------
inline std::string ConvertFloatToString( float floatingPointNumber )
{
	return std::to_string( static_cast< long double >( floatingPointNumber ) );
}

//-----------------------------------------------------------------------------------------------
inline std::string ConvertIntegerToString( int integer )
{
	return std::to_string( static_cast< long long >( integer ) );
}


//-----------------------------------------------------------------------------------------------
inline bool ConvertStringToBool( const std::string& booleanString )
{
	bool output;
	std::string lowercasedBooleanString;
	ConvertStringToLowercase( booleanString, lowercasedBooleanString );
	std::istringstream stringConverter( lowercasedBooleanString );
	stringConverter >> std::boolalpha >> output;
	return output;
}

//-----------------------------------------------------------------------------------------------
inline int ConvertStringToInteger( const std::string& numberString )
{
	int output;
	std::istringstream stringConverter( numberString );
	stringConverter >> output;
	return output;
}

//-----------------------------------------------------------------------------------------------
inline unsigned int ConvertStringToUnsignedInt( const std::string& numberString )
{
	unsigned int output;
	std::istringstream stringConverter( numberString );
	stringConverter >> output;
	return output;
}

//-----------------------------------------------------------------------------------------------
inline float ConvertStringToFloat( const std::string& numberString )
{
	float output;
	std::istringstream stringConverter( numberString );
	stringConverter >> output;
	return output;
}

//-----------------------------------------------------------------------------------------------
inline double ConvertStringToDouble( const std::string& numberString )
{
	double output;
	std::istringstream stringConverter( numberString );
	stringConverter >> output;
	return output;
}



//-----------------------------------------------------------------------------------------------
Color ConvertStringToColorOrDie( const std::string& colorString );

FloatVector2 ConvertStringToFloatVector2OrDie( const std::string& numberString );

FloatVector3 ConvertStringToFloatVector3OrDie( const std::string& numberString );

FloatVector4 ConvertStringToFloatVector4OrDie( const std::string& numberString );

IntVector2 ConvertStringToIntVector2OrDie( const std::string& numberString );

IntVector3 ConvertStringToIntVector3OrDie( const std::string& numberString );

#endif //INCLUDED_STRING_CONVERSION_HPP
