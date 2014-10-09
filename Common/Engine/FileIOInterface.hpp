#pragma once
#ifndef INCLUDED_FILE_IO_INTERFACE_HPP
#define INCLUDED_FILE_IO_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>

//-----------------------------------------------------------------------------------------------
inline int fpeek( FILE* file )
{
	int upcastedChar;

	upcastedChar = fgetc( file );
	ungetc( upcastedChar, file );

	return upcastedChar;
}



//-----------------------------------------------------------------------------------------------
inline std::string GetFileExtensionForFilePath( const std::string& filePath )
{
	return filePath.substr( filePath.size() - 3 );
}

inline std::wstring GetFileExtensionForFilePath( const std::wstring& filePath )
{
	return filePath.substr( filePath.size() - 3 );
}



//File Extension Validation
//-----------------------------------------------------------------------------------------------
bool FileHasValidExtension( const std::string& fileNameOrPath, const std::string& commaSeparatedValidExtensions );
bool FileHasValidExtension( const std::wstring& fileNameOrPath, const std::wstring& commaSeparatedValidExtensions );

void VerifyFileExtensionValidityOrDie( const std::string& fileNameOrPath, const std::string& commaSeparatedValidExtensions );
void VerifyFileExtensionValidityOrDie( const std::wstring& fileNameOrPath, const std::wstring& commaSeparatedValidExtensions );



//File Opening and Closure
//-----------------------------------------------------------------------------------------------
enum FileMode
{
	FILEMODE_ReadOnly = 1,
	FILEMODE_WriteOnly = 2,
	FILEMODE_ReadAndWrite = 3,
	FILEMODE_AppendingOnly = 4,
	FILEMODE_ReadingAndAppending = 5,
	FILEMODE_ClearFileThenReadWrite = 6
};

void CloseFileOrDie( FILE* file );
long GetSizeOfFileInBytes( FILE* file );
void OpenFileAtPathOrDie( FILE** file, const std::string& filePath, FileMode fileMode = FILEMODE_ReadAndWrite, bool isBinaryFile = false );



//Directories
//-----------------------------------------------------------------------------------------------
void FindAllFilesInDirectoryContainingString( std::vector< std::string >& out_filePaths, const std::string& directoryPath, const std::string& searchString );

//-----------------------------------------------------------------------------------------------
void GenerateDummyDataFileOfSize( const std::string& dummyFilePath, unsigned int fileSizeInKB );

#endif //INCLUDED_FILE_IO_INTERFACE_HPP
