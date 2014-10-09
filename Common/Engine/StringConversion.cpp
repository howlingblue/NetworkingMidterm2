#include "StringConversion.hpp"
#include "DialogInterface.hpp"
#include <algorithm>

//Helper function for attribute and child lowercasing
//-----------------------------------------------------------------------------------------------
void ConvertStringToLowercase( const std::string& casedString, std::string& out_lowercasedString )
{
	//Thanks to http://stackoverflow.com/questions/313970/stl-string-to-lower-case
	out_lowercasedString.assign( casedString );
	std::transform( out_lowercasedString.begin(), out_lowercasedString.end(), out_lowercasedString.begin(), ::tolower );
}

//-----------------------------------------------------------------------------------------------
void ConvertWstringToLowercase( const std::wstring& casedString, std::wstring& out_lowercasedString )
{
	//Thanks to http://stackoverflow.com/questions/313970/stl-string-to-lower-case
	out_lowercasedString.assign( casedString );
	std::transform( out_lowercasedString.begin(), out_lowercasedString.end(), out_lowercasedString.begin(), ::tolower );
}

//-----------------------------------------------------------------------------------------------
std::string StripStartAndEndParenthesisOrDie( const std::string& stringWithPossibleParenthesis )
{
	if( stringWithPossibleParenthesis[ 0 ] == '(' )
	{
		//if we have an opening parenthesis, we better damn well have a closing one
		if( stringWithPossibleParenthesis[ stringWithPossibleParenthesis.size() - 1 ] != ')' )
		{
			std::string errorMessageBoxTitle( "Vingine Error: Bad String Conversion" );
			std::string errorMessage( "ERROR: Mismatched parenthesis in string to vector conversion!\n" );
			errorMessage.append( "Vector has opening parenthesis without a  closing parenthesis." );
			errorMessage.append( "\nLocation info not available. Use debugger breaks to determine cause." );
			assert( false );
			exit( -1 );
		}

		return std::string( stringWithPossibleParenthesis, 1, stringWithPossibleParenthesis.length() - 2 );
	}
	//if we have no opening parenthesis, we better damn well not have an ending one
	else if( stringWithPossibleParenthesis[ stringWithPossibleParenthesis.size() - 1 ] == ')' )
	{
		std::string errorMessageBoxTitle( "Vingine Error: Bad String Conversion" );
		std::string errorMessage( "ERROR: Mismatched parenthesis in string to vector conversion!\n" );
		errorMessage.append( "Vector has closing parenthesis without an opening parenthesis." );
		errorMessage.append( "\nLocation info not available. Use debugger breaks to determine cause." );
		assert( false );
		exit( -1 );
	}
	return stringWithPossibleParenthesis;
}

//-----------------------------------------------------------------------------------------------
void BuildStringVectorFromCommaSeparatedString( std::vector< std::string >& stringVector, const std::string& commaSeparatedString )
{
	size_t currentCommaLocation = 0, nextCommaLocation = 0;

	if( commaSeparatedString == "" )
		return;

	do
	{
		nextCommaLocation = commaSeparatedString.find( ",", currentCommaLocation );

		std::string substring = commaSeparatedString.substr( currentCommaLocation, nextCommaLocation - currentCommaLocation );
		substring.erase( std::remove_if( substring.begin(), substring.end(), isspace ), substring.end() ); //One liner to strip white space from string
		stringVector.push_back( substring );

		currentCommaLocation = nextCommaLocation + 1; 
		//When nextCommaLocation = npos, currentCommaLocation = 0, since npos = INT_MAX
	} while( currentCommaLocation != 0 );
}

//-----------------------------------------------------------------------------------------------
void BuildWstringVectorFromCommaSeparatedWstring( std::vector< std::wstring >& wstringVector, const std::wstring& commaSeparatedWstring )
{
	size_t currentCommaLocation = 0, nextCommaLocation = 0;

	if( commaSeparatedWstring == L"" )
		return;

	do
	{
		nextCommaLocation = commaSeparatedWstring.find( L",", currentCommaLocation );

		std::wstring substring = commaSeparatedWstring.substr( currentCommaLocation, nextCommaLocation - currentCommaLocation );
		substring.erase( std::remove_if( substring.begin(), substring.end(), isspace ), substring.end() ); //One liner to strip white space from string
		wstringVector.push_back( substring );

		currentCommaLocation = nextCommaLocation + 1; 
		//When nextCommaLocation = npos, currentCommaLocation = 0, since npos = INT_MAX
	} while( currentCommaLocation != 0 );
}

//-----------------------------------------------------------------------------------------------
void SendErrorMessageAboutMismatchedVectorSizesThenDie( unsigned int expectedSize, unsigned int actualSize )
{
	std::string errorMessageBoxTitle( "Vingine Error: Bad String Conversion" );
	std::string errorMessage( "ERROR: Mismatched size in string to vector conversion!\n" );
	errorMessage.append( "\tExpected number of numbers in vector: " + expectedSize );
	errorMessage.append( "\n\tActual number of numbers in vector: " + actualSize );
	errorMessage.append( "\nLocation info not available. Use debugger breaks to determine cause." );
	assert( false );
	exit( -1 );
}

//-----------------------------------------------------------------------------------------------
Color ConvertStringToColor( const std::string& colorString )
{
	std::vector< std::string > stringVector;
	std::string strippedColorString = StripStartAndEndParenthesisOrDie( colorString );
	BuildStringVectorFromCommaSeparatedString( stringVector, strippedColorString );

	Color outputColor;
	std::istringstream stringConverter;
	if( stringVector.size() == 3 )
	{
		stringConverter.str( stringVector[ 0 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.r;

		stringConverter.str( stringVector[ 1 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.g;

		stringConverter.str( stringVector[ 2 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.b;

		outputColor.a = 255;
	}
	else if( stringVector.size() == 4 )
	{
		stringConverter.str( stringVector[ 0 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.r;

		stringConverter.str( stringVector[ 1 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.g;

		stringConverter.str( stringVector[ 2 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.b;

		stringConverter.str( stringVector[ 3 ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputColor.a;
	}
	else
	{
		std::string errorMessageBoxTitle( "Vingine Error: Bad String Conversion" );
		std::string errorMessage( "ERROR: Mismatched size in string to color conversion!\n" );
		errorMessage.append( "\tColor expects 3 or 4 numbers in vector" );
		errorMessage.append( "\n\tActual number of numbers in vector: " + stringVector.size() );
		errorMessage.append( "\nLocation info not available. Use debugger breaks to determine cause." );
		assert( false );
		exit( -1 );
	}

	return outputColor;
}

//-----------------------------------------------------------------------------------------------
FloatVector2 ConvertStringToFloatVector2OrDie( const std::string& numberString )
{
	static const unsigned int EXPECTED_VECTOR_SIZE = 2;

	std::vector< std::string > stringVector;
	std::string strippedNumberString = StripStartAndEndParenthesisOrDie( numberString );
	BuildStringVectorFromCommaSeparatedString( stringVector, strippedNumberString );

	if( stringVector.size() != EXPECTED_VECTOR_SIZE )
	{
		SendErrorMessageAboutMismatchedVectorSizesThenDie( EXPECTED_VECTOR_SIZE, stringVector.size() );
		exit( -1 );
	}

	FloatVector2 outputVector;
	std::istringstream stringConverter;
	for( unsigned int i = 0; i < EXPECTED_VECTOR_SIZE; ++i )
	{
		stringConverter.str( stringVector[ i ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputVector[ i ];
	}
	return outputVector;
}

//-----------------------------------------------------------------------------------------------
FloatVector3 ConvertStringToFloatVector3OrDie( const std::string& numberString )
{
	static const unsigned int EXPECTED_VECTOR_SIZE = 3;

	std::vector< std::string > stringVector;
	std::string strippedNumberString = StripStartAndEndParenthesisOrDie( numberString );
	BuildStringVectorFromCommaSeparatedString( stringVector, strippedNumberString );

	if( stringVector.size() != EXPECTED_VECTOR_SIZE )
	{
		SendErrorMessageAboutMismatchedVectorSizesThenDie( EXPECTED_VECTOR_SIZE, stringVector.size() );
		exit( -1 );
	}

	FloatVector3 outputVector;
	std::istringstream stringConverter;
	for( unsigned int i = 0; i < EXPECTED_VECTOR_SIZE; ++i )
	{
		stringConverter.str( stringVector[ i ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputVector[ i ];
	}
	return outputVector;
}

//-----------------------------------------------------------------------------------------------
FloatVector4 ConvertStringToFloatVector4( const std::string& numberString )
{
	static const unsigned int EXPECTED_VECTOR_SIZE = 4;

	std::vector< std::string > stringVector;
	std::string strippedNumberString = StripStartAndEndParenthesisOrDie( numberString );
	BuildStringVectorFromCommaSeparatedString( stringVector, strippedNumberString );

	if( stringVector.size() != EXPECTED_VECTOR_SIZE )
	{
		SendErrorMessageAboutMismatchedVectorSizesThenDie( EXPECTED_VECTOR_SIZE, stringVector.size() );
		exit( -1 );
	}

	FloatVector4 outputVector;
	std::istringstream stringConverter;
	for( unsigned int i = 0; i < EXPECTED_VECTOR_SIZE; ++i )
	{
		stringConverter.str( stringVector[ i ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputVector[ i ];
	}
	return outputVector;
}

//-----------------------------------------------------------------------------------------------
IntVector2 ConvertStringToIntVector2OrDie( const std::string& numberString )
{
	static const unsigned int EXPECTED_VECTOR_SIZE = 2;

	std::vector< std::string > stringVector;
	std::string strippedNumberString = StripStartAndEndParenthesisOrDie( numberString );
	BuildStringVectorFromCommaSeparatedString( stringVector, strippedNumberString );

	if( stringVector.size() != EXPECTED_VECTOR_SIZE )
	{
		SendErrorMessageAboutMismatchedVectorSizesThenDie( EXPECTED_VECTOR_SIZE, stringVector.size() );
		exit( -1 );
	}

	IntVector2 outputVector;
	std::istringstream stringConverter;
	for( unsigned int i = 0; i < EXPECTED_VECTOR_SIZE; ++i )
	{
		stringConverter.str( stringVector[ i ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputVector[ i ];
	}
	return outputVector;
}

//-----------------------------------------------------------------------------------------------
IntVector3 ConvertStringToIntVector3OrDie( const std::string& numberString )
{
	static const unsigned int EXPECTED_VECTOR_SIZE = 3;

	std::vector< std::string > stringVector;
	std::string strippedNumberString = StripStartAndEndParenthesisOrDie( numberString );
	BuildStringVectorFromCommaSeparatedString( stringVector, strippedNumberString );

	if( stringVector.size() != EXPECTED_VECTOR_SIZE )
	{
		SendErrorMessageAboutMismatchedVectorSizesThenDie( EXPECTED_VECTOR_SIZE, stringVector.size() );
		exit( -1 );
	}

	IntVector3 outputVector;
	std::istringstream stringConverter;
	for( unsigned int i = 0; i < EXPECTED_VECTOR_SIZE; ++i )
	{
		stringConverter.str( stringVector[ i ] );
		stringConverter.clear(); //Clear any eof flags, since we probably reached the end of the stream after each conversion
		stringConverter >> outputVector[ i ];
	}
	return outputVector;
}
