#include "DebuggerInterface.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//-----------------------------------------------------------------------------------------------
bool DebuggerIsAttached()
{
	//Windows debugger detection code written by Squirrel Eiserloh
#ifdef _WIN32
	typedef BOOL (CALLBACK IsDebugFuncType)();

	// Get a handle to KERNEL32.DLL
	HINSTANCE hInstanceKernel32 = GetModuleHandle( TEXT( "KERNEL32" ) );
	if( !hInstanceKernel32 )
		return false;

	// Get a handle to the IsDebuggerPresent() function in KERNEL32.DLL
	IsDebugFuncType* isDebuggerPresentFunc = (IsDebugFuncType*) GetProcAddress( hInstanceKernel32, "IsDebuggerPresent" );
	if( !isDebuggerPresentFunc )
		return false;

	// Now CALL that function and return its result
	BOOL isDebuggerAvailable = isDebuggerPresentFunc();
	return( isDebuggerAvailable == TRUE );
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------------------------
void WriteToDebuggerOutput( const std::string& string )
{
#ifdef _WIN32
	OutputDebugStringA( string.c_str() );
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------------------------
void SetBreakpoint()
{
#ifdef _WIN32
	__debugbreak();
#else
	return;
#endif
}

//-----------------------------------------------------------------------------------------------
std::string BuildVisualStudioClickableString( const std::string& filename, const std::string& lineNumber, const std::string& errorText )
{
	std::string visualStudioString( filename );
	visualStudioString += "(";
	visualStudioString += lineNumber;
	visualStudioString += ") : ";
	visualStudioString += errorText;
	visualStudioString += "\n";

	return visualStudioString;
}
