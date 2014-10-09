#pragma once
#ifndef INCLUDED_DEBUGGER_INTERFACE_HPP
#define INCLUDED_DEBUGGER_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <string>

//-----------------------------------------------------------------------------------------------
bool DebuggerIsAttached();
void WriteToDebuggerOutput( const std::string& string );
void SetBreakpoint();

//-----------------------------------------------------------------------------------------------
std::string BuildVisualStudioClickableString( const std::string& filename, const std::string& lineNumber, const std::string& errorText = "Unknown Error!" );

#endif //INCLUDED_DEBUGGER_INTERFACE_HPP
