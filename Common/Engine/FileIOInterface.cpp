#include "FileIOInterface.hpp"

#include <algorithm>

#include "EngineMacros.hpp"
#include "PlatformSpecificHeaders.hpp"

#include "AssertionError.hpp"
#include "StringConversion.hpp"
#include "DialogInterface.hpp"

#pragma region File Extension Validation
//-----------------------------------------------------------------------------------------------
bool FileHasValidExtension( const std::string& fileNameOrPath, const std::string& commaSeparatedValidExtensions )
{
	std::string fileExtension = GetFileExtensionForFilePath( fileNameOrPath );
	std::string lowercasedFileExtension;
	ConvertStringToLowercase( fileExtension, lowercasedFileExtension );

	std::vector< std::string > validExtensions;
	std::string lowercasedValidExtensions;
	ConvertStringToLowercase( commaSeparatedValidExtensions, lowercasedValidExtensions );
	BuildStringVectorFromCommaSeparatedString( validExtensions, lowercasedValidExtensions );

	for( unsigned int i = 0; i < validExtensions.size(); ++i )
	{
		if( lowercasedFileExtension.compare( validExtensions[ i ] ) == 0 )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
bool FileHasValidExtension( const std::wstring& fileNameOrPath, const std::wstring& commaSeparatedValidExtensions )
{
	std::wstring fileExtension = GetFileExtensionForFilePath( fileNameOrPath );
	std::wstring lowercasedFileExtension;
	ConvertWstringToLowercase( fileExtension, lowercasedFileExtension );

	std::vector< std::wstring > validExtensions;
	std::wstring lowercasedValidExtensions;
	ConvertWstringToLowercase( commaSeparatedValidExtensions, lowercasedValidExtensions );
	BuildWstringVectorFromCommaSeparatedWstring( validExtensions, lowercasedValidExtensions );

	for( unsigned int i = 0; i < validExtensions.size(); ++i )
	{
		if( lowercasedFileExtension.compare( validExtensions[ i ] ) == 0 )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
void VerifyFileExtensionValidityOrDie( const std::string& fileNameOrPath, const std::string& commaSeparatedValidExtensions )
{
	static const std::string errorMessageBoxTitle = "Vingine Error: Bad File Extension";

	if( !FileHasValidExtension( fileNameOrPath, commaSeparatedValidExtensions ) )
	{
		std::string errorString = "ERROR: Bad File Extension!\n\n";
		errorString.append( "File \"" + fileNameOrPath + "\" has an invalid extension.\n\n" );
		errorString.append( "Valid Extensions: ");
		errorString.append( commaSeparatedValidExtensions );

		FATAL_ERROR( errorMessageBoxTitle, errorString );
	}
}

//-----------------------------------------------------------------------------------------------
void VerifyFileExtensionValidityOrDie( const std::wstring& fileNameOrPath, const std::wstring& commaSeparatedValidExtensions )
{
	static const std::wstring errorMessageBoxTitle = L"Vingine Error: Bad File Extension";

	if( !FileHasValidExtension( fileNameOrPath, commaSeparatedValidExtensions ) )
	{
		std::wstring errorString = L"ERROR: Bad File Extension!\n\n";
		errorString.append( L"File \"" + fileNameOrPath + L"\" has an invalid extension.\n\n" );
		errorString.append( L"Valid Extensions: ");
		errorString.append( commaSeparatedValidExtensions );

		PopUpSystemDialog( errorMessageBoxTitle, errorString );
		exit( -5 );
	}
}
#pragma endregion



#pragma region File Opening and Closure
//-----------------------------------------------------------------------------------------------
void CloseFileOrDie( FILE* file )
{
	int returnCode = fclose( file );

	if( returnCode != 0 )
	{
		FATAL_ERROR( "File Error", "Unable to close a file." );
	}
}

//-----------------------------------------------------------------------------------------------
long GetSizeOfFileInBytes( FILE* file )
{
	long originalFilePosition = ftell( file );

	fseek( file, 0L, SEEK_END );
	long fileSizeInBytes = ftell( file );

	fseek( file, 0L, originalFilePosition );
	return fileSizeInBytes;
}

//-----------------------------------------------------------------------------------------------
void OpenFileAtPathOrDie( FILE** file, const std::string& filePath, FileMode fileMode, bool isBinaryFile )
{
	std::string fileModeAsChar;

	switch( fileMode )
	{
	case FILEMODE_ReadOnly:
		fileModeAsChar += "r";
		break;
	case FILEMODE_WriteOnly:
		fileModeAsChar += "w";
		break;
	case FILEMODE_ReadAndWrite:
		fileModeAsChar += "r+";
		break;
	case FILEMODE_AppendingOnly:
		fileModeAsChar += "a";
		break;
	case FILEMODE_ReadingAndAppending:
		fileModeAsChar += "a+";
		break;
	case FILEMODE_ClearFileThenReadWrite:
		fileModeAsChar += "w+";
		break;
	default:
		RECOVERABLE_ERROR( "File Opening Error", "Unable to determine file mode." );
		fileModeAsChar = "r+";
	}

	if( isBinaryFile )
		fileModeAsChar.insert( 1, "b" );

	int openingErrorCode = fopen_s( file, filePath.c_str(), fileModeAsChar.c_str() );
	if( openingErrorCode != 0 )
	{
		FATAL_ERROR( "File Opening Error", "Unable to open file." );
	}
}
#pragma endregion



//-----------------------------------------------------------------------------------------------
void FindAllFilesInDirectoryContainingString( std::vector< std::string >& out_filePaths, const std::string& directoryPath, const std::string& searchString )
{
#if defined( PLATFORM_WINDOWS )
	int errorCode = 1;
	WIN32_FIND_DATAA fileInfo;
	HANDLE searchHandle;
	std::string filename;
	std::string searchParameters( directoryPath + searchString );

	searchHandle = FindFirstFileA( searchParameters.c_str(), &fileInfo );
	while( searchHandle != INVALID_HANDLE_VALUE && errorCode != 0 )
	{
		filename = fileInfo.cFileName;
		out_filePaths.push_back( directoryPath + filename );
		errorCode = FindNextFileA( searchHandle, &fileInfo );
	}
#endif
}



//-----------------------------------------------------------------------------------------------
void GenerateDummyDataFileOfSize( const std::string& dummyFilePath, unsigned int fileSizeInKB )
{
	static const unsigned int dummyStringLength = 50;
	static const unsigned int randomCharRange = ( sizeof( char ) * 256 ) - 1;
	
	FILE* dummyFile;
	OpenFileAtPathOrDie( &dummyFile, dummyFilePath, FILEMODE_WriteOnly );

	unsigned int numberOfDummyStringsToPaste = 20 * fileSizeInKB;
	char dummyString[ dummyStringLength ] = "This is a 50 character string for testing usage.\n";
	for( unsigned int i = 0; i < numberOfDummyStringsToPaste; ++i )
	{
		for( unsigned int j = 0; j < dummyStringLength; ++j )
		{
			dummyString[ j ] = static_cast< char >( GetRandomIntBetween( 0, randomCharRange ) );
		}

		fwrite( dummyString, sizeof( char ), dummyStringLength, dummyFile );
	}

	CloseFileOrDie( dummyFile );
}
