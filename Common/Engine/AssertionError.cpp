#include "AssertionError.hpp"

//#include "BuildPreferences.hpp"
#include "DebuggerInterface.hpp"
#include "DialogInterface.hpp"

//-----------------------------------------------------------------------------------------------
__declspec( noreturn ) void PresentFatalError( const char* nameOfFileWithError, int lineNumberOfError, const std::string& errorTitle, 
												const std::string& errorMessage, const char* conditionText )
{
	std::string completeErrorTitle = /*g_applicationName +*/ " - FATAL ERROR: " + errorTitle;
	std::string completeErrorMessage = "A fatal error has occurred. This application will now close.\n\n";
	completeErrorMessage += "---------------------- Error Details: ----------------------\n";
	completeErrorMessage += "Error: " + errorMessage + "\n";

	if( conditionText != nullptr )
	{
		completeErrorMessage += "The assertion ";
		completeErrorMessage += std::string( conditionText );
		completeErrorMessage += " was not true.\n";
	}

	WriteToDebuggerOutput( BuildVisualStudioClickableString( nameOfFileWithError, std::to_string( long long( lineNumberOfError ) ), errorMessage ) );

	if( DebuggerIsAttached() )
	{
		completeErrorMessage += "DEBUGGER DETECTED: Would you like to set a breakpoint? (No will quit the application.)";

		Dialog::ButtonID pressedButtonID = PopUpSystemDialog( completeErrorTitle, completeErrorMessage, Dialog::BUTTONSET_YesOrNo, Dialog::ICON_Warning );
		switch( pressedButtonID )
		{
		case Dialog::ID_Yes:
			SetBreakpoint();
			break;

		case Dialog::ID_No:
		default:
			exit( -1 );
		}
	}
	else
	{
		PopUpSystemDialog( completeErrorTitle, completeErrorMessage, Dialog::BUTTONSET_OK, Dialog::ICON_Error );
	}
	exit( -1 );
}

//-----------------------------------------------------------------------------------------------
void PresentRecoverableError( const char* nameOfFileWithError, int lineNumberOfError, const std::string& errorTitle, 
								const std::string& errorMessage, const char* conditionText )
{
	std::string completeErrorTitle = /*g_applicationName +*/ " - Recoverable Error: " + errorTitle;
	std::string completeErrorMessage = "A recoverable error has occurred.\n\n";
	completeErrorMessage += "---------------------- Error Details: ----------------------\n";
	completeErrorMessage += "Error: " + errorMessage + "\n";

	if( conditionText != nullptr )
	{
		completeErrorMessage += "The assertion ";
		completeErrorMessage += conditionText;
		completeErrorMessage += " was not true.\n\n";
	}

	WriteToDebuggerOutput( BuildVisualStudioClickableString( nameOfFileWithError, std::to_string( long long( lineNumberOfError ) ), errorMessage ) );

	if( DebuggerIsAttached() )
	{
		completeErrorMessage += "DEBUGGER DETECTED: Would you like to (try again) with a breakpoint, (continue) the application, or (cancel) running?";
		
		Dialog::ButtonID pressedButtonID = PopUpSystemDialog( completeErrorTitle, completeErrorMessage, Dialog::BUTTONSET_CancelTryAgainOrContinue, Dialog::ICON_Warning );
		switch( pressedButtonID )
		{
		case Dialog::ID_TryAgain:
			SetBreakpoint();
			return;

		case Dialog::ID_Continue:
			return;

		case Dialog::ID_Cancel:
		default:
			exit( -1 );
		}
	}
	else
	{
		completeErrorMessage += "Would you like the application to continue despite the error? (No will quit the application.)";

		Dialog::ButtonID pressedButtonID = PopUpSystemDialog( completeErrorTitle, completeErrorMessage, Dialog::BUTTONSET_YesOrNo, Dialog::ICON_Warning );
		switch( pressedButtonID )
		{
		case Dialog::ID_Yes:
			return;

		case Dialog::ID_No:
		default:
			exit( -1 );
		}
	}
}
