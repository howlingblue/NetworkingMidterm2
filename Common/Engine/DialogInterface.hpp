#ifndef INCLUDED_DIALOG_INTERFACE_HPP
#define INCLUDED_DIALOG_INTERFACE_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <string>

//-----------------------------------------------------------------------------------------------
namespace Dialog
{
	//These enumerations correspond with Windows' enumerations for these settings.
	typedef int ButtonID;
	static const ButtonID ID_OK = 1;		//IDOK
	static const ButtonID ID_Cancel = 2;	//IDCANCEL
	static const ButtonID ID_Retry = 4;		//IDRETRY
	static const ButtonID ID_Yes = 6;		//IDYES
	static const ButtonID ID_No = 7;		//IDNO
	static const ButtonID ID_TryAgain = 10;	//IDTRYAGAIN
	static const ButtonID ID_Continue = 11; //IDCONTINUE

	enum ButtonSet
	{
		BUTTONSET_OK = 0x0,							//MB_OK
		BUTTONSET_OKOrCancel = 0x1,					//MB_OKCANCEL
		BUTTONSET_YesOrNo = 0x4,					//MB_YESNO
		BUTTONSET_RetryOrCancel = 0x5,				//MB_RETRYCANCEL
		BUTTONSET_CancelTryAgainOrContinue = 0x6	//MB_CANCELTRYCONTINUE
	};

	enum Icon
	{
		ICON_None = 0x0,			//No equivalent Windows definition
		ICON_Information = 0x40,	//MB_ICONINFORMATION
		ICON_Question = 0x20,		//MB_ICONQUESTION
		ICON_Warning = 0x30,		//MB_ICONWARNING
		ICON_Error = 0x10			//MB_ICONERROR
	};
}

//-----------------------------------------------------------------------------------------------
Dialog::ButtonID PopUpSystemDialog( const std::string& titleText, const std::string& messageText,
							Dialog::ButtonSet buttonSet = Dialog::BUTTONSET_OK, Dialog::Icon icon = Dialog::ICON_None );

Dialog::ButtonID PopUpSystemDialog( const std::wstring& titleText, const std::wstring& messageText,
							Dialog::ButtonSet buttonSet = Dialog::BUTTONSET_OK, Dialog::Icon icon = Dialog::ICON_None );

#endif //INCLUDED_DIALOG_INTERFACE_HPP
