#include "DialogInterface.hpp"

#include "PlatformSpecificHeaders.hpp"

//-----------------------------------------------------------------------------------------------
Dialog::ButtonID PopUpSystemDialog( const std::string& titleText, const std::string& messageText, 
							Dialog::ButtonSet buttonSet, Dialog::Icon icon )
{
#if defined( PLATFORM_WINDOWS )
	return MessageBoxA( NULL, messageText.c_str(), titleText.c_str(), buttonSet | icon );
#else
	return Dialog::ID_Cancel;
#endif
}

//-----------------------------------------------------------------------------------------------
Dialog::ButtonID PopUpSystemDialog( const std::wstring& titleText, const std::wstring& messageText,
							Dialog::ButtonSet buttonSet, Dialog::Icon icon )
{
#if defined( PLATFORM_WINDOWS )
	return MessageBoxW( NULL, messageText.c_str(), titleText.c_str(), buttonSet | icon );
#else
	return Dialog::ID_Cancel;
#endif
}
