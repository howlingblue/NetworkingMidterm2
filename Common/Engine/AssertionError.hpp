#pragma once
#ifndef INCLUDED_ASSERTION_ERROR_HPP
#define INCLUDED_ASSERTION_ERROR_HPP

//-----------------------------------------------------------------------------------------------
#include <string>

//-----------------------------------------------------------------------------------------------
__declspec( noreturn ) void PresentFatalError( const char* nameOfFileWithError, int lineNumberOfError, const std::string& errorTitle, 
												const std::string& errorMessage, const char* conditionText = nullptr );

void PresentRecoverableError( const char* nameOfFileWithError, int lineNumberOfError, const std::string& errorTitle, 
								const std::string& errorMessage, const char* conditionText = nullptr );

#pragma region Errors
//-----------------------------------------------------------------------------------------------
#define FATAL_ERROR( errorTitle, errorMessageText )							\
{																			\
	PresentFatalError( __FILE__, __LINE__, errorTitle, errorMessageText );	\
}

//-----------------------------------------------------------------------------------------------
#define UNKNOWN_FATAL_ERROR																					\
{																											\
	PresentFatalError( __FILE__, __LINE__, "Unknown Fatal Error!", "An unknown fatal error has occurred." );\
}

//-----------------------------------------------------------------------------------------------
#define RECOVERABLE_ERROR( errorTitle, errorMessageText )						\
{																				\
	PresentRecoverableError( __FILE__, __LINE__, errorTitle, errorMessageText );\
}

//-----------------------------------------------------------------------------------------------
#define UNKNOWN_RECOVERABLE_ERROR																										 \
{																																		 \
	PresentRecoverableError( __FILE__, __LINE__, "Unknown Recoverable Error!", "An unknown error has occurred, but it is recoverable." );\
}
#pragma endregion



#pragma region Assertions

#ifndef ENABLE_ASSERTIONS

	#define FATAL_ASSERTION( condition, errorTitle, errorMessageText ) { (void)( condition ); }

	#define UNKNOWN_FATAL_ASSERTION( condition ) { (void)( condition ); }

	#define RECOVERABLE_ASSERTION( condition, errorTitle, errorMessageText ) { (void)( condition ); }

	#define UNKNOWN_RECOVERABLE_ASSERTION( condition ) { (void)( condition ); }

#else
	//-----------------------------------------------------------------------------------------------
	#define FATAL_ASSERTION( condition, errorTitle, errorMessageText )								\
	{																								\
		if( !( condition ) )																		\
		{																							\
			const char* conditionText = #condition;													\
			PresentFatalError( __FILE__, __LINE__, errorTitle, errorMessageText, conditionText );	\
		}																							\
	}

	//-----------------------------------------------------------------------------------------------
	#define UNKNOWN_FATAL_ASSERTION( condition )									\
	{																				\
		if( !( condition ) )														\
		{																			\
			const char* conditionText = #condition;									\
			PresentFatalError( __FILE__, __LINE__, "Unknown Fatal Error!",			\
				"An unknown fatal error has occurred.", conditionText );			\
		}																			\
	}

	//-----------------------------------------------------------------------------------------------
	#define RECOVERABLE_ASSERTION( condition, errorTitle, errorMessageText )							\
	{																									\
		if( !( condition ) )																			\
		{																								\
			const char* conditionText = #condition;														\
			PresentRecoverableError( __FILE__, __LINE__, errorTitle, errorMessageText, conditionText );	\
		}																								\
	}

	//-----------------------------------------------------------------------------------------------
	#define UNKNOWN_RECOVERABLE_ASSERTION( condition )														 \
	{																										 \
		if( !( condition ) )																				 \
		{																									 \
			const char* conditionText = #condition;															 \
			PresentRecoverableError( __FILE__, __LINE__, "Unknown Recoverable Error!",						 \
									"An unknown error has occurred, but it is recoverable.", conditionText );\
		}																									 \
	}
#endif

#pragma endregion

#endif //INCLUDED_ASSERTION_ERROR_HPP
