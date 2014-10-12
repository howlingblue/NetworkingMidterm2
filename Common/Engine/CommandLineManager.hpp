#pragma once
#ifndef INCLUDED_COMMAND_LINE_MANAGER_HPP
#define INCLUDED_COMMAND_LINE_MANAGER_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "AssertionError.hpp"
#include "CommandLineDelegate.hpp"
#include "EngineMacros.hpp"

//-----------------------------------------------------------------------------------------------
namespace CommandLine
{
	typedef std::vector< CommandLine::Option > OptionList;

	//-----------------------------------------------------------------------------------------------
	//This is a utility class used by the manager to iterate through the command line.
	struct CharacterIterator
	{
		CharacterIterator( const char* stringToIterate )
			: currentPosition( 0 )
			, string( stringToIterate )
		{ }

		char GetCurrentCharacter() const
		{
			return string[ currentPosition ];
		}

		char GetNextCharacter()
		{
			++currentPosition;
			return string[ currentPosition ];
		}

		unsigned int currentPosition;
		const char* string;
	};



	//-------------------------------------------------------------------------------------------
	SINGLETON class Manager
	{
		static const char STRING_TERMINATOR = '\0';
	public:
		Manager();

		//Lifecycle
		static void Create();
		static void Destroy();

		//Observer Modification
		static void AddObserver( CommandLine::Delegate& observer );
		static void RemoveObserver( CommandLine::Delegate& observer );

		//Primary Function
		static void ReportCommandError( const std::string& errorDetails );
		static void RunCommandLine( const char* commandLineString );



	private:
		//Utility Functions
		static bool IsOptionStartingCharacter( char character );
		static bool IsQuoteCharacter( char character );

		//Low-level Processes
		static void ExtractQuoteDelimitedToken( std::string& out_token, CharacterIterator& cmdLineIterator );
		static void ExtractWhitespaceDelimitedToken( std::string& out_token, CharacterIterator& cmdLineIterator );
		static void SkipWhiteSpaceBetweenTokens( CharacterIterator& cmdLineIterator );

		//Mid-level Processes
		static void SplitCommandLineIntoTokens( const char* commandLine, std::vector< std::string >& out_tokens );
		static void MergeTokensIntoOptions( const std::vector< std::string >& tokens, OptionList& out_options );
		
		

		//Data
		static Manager* s_activeCommandLineManager;

		std::vector< CommandLine::Delegate > m_observers;
		bool m_errorHasOccurred;
	};



	//-------------------------------------------------------------------------------------------
	inline Manager::Manager()
		: m_errorHasOccurred( false )
	{
		m_observers.reserve( 16 );
	}



	#pragma region Lifecycle
	//-------------------------------------------------------------------------------------------
	STATIC inline void Manager::Create()
	{
		FATAL_ASSERTION( s_activeCommandLineManager != nullptr, "Manager Error!", "Cannot create multiple Command Line Managers!" );

		s_activeCommandLineManager = new CommandLine::Manager();
	}

	//-------------------------------------------------------------------------------------------
	STATIC inline void Manager::Destroy()
	{
		RECOVERABLE_ASSERTION( s_activeCommandLineManager == nullptr, "Manager Error!", "Cannot destroy a Command Line Manager because none exists!" );

		delete s_activeCommandLineManager;
		s_activeCommandLineManager = nullptr;
	}
	#pragma endregion



	#pragma region Observer Modification
	//-------------------------------------------------------------------------------------------
	STATIC inline void Manager::AddObserver( CommandLine::Delegate& observer )
	{
		FATAL_ASSERTION( s_activeCommandLineManager == nullptr, "Command Line Error!", 
						"Cannot use command line functions without an active Command Line Manager." );

		s_activeCommandLineManager->m_observers.push_back( observer );
		//WARNING: No protection against inserting multiple observers from the same object...
	}
	//-------------------------------------------------------------------------------------------
	STATIC inline void Manager::RemoveObserver( CommandLine::Delegate& observer )
	{
		FATAL_ASSERTION( s_activeCommandLineManager == nullptr, "Command Line Error!", 
						"Cannot use command line functions without an active Command Line Manager." );

		std::vector< CommandLine::Delegate >& observerList = s_activeCommandLineManager->m_observers;
		unsigned int numberOfObservers = observerList.size();
		
		for( unsigned int i = 0; i < numberOfObservers; ++i )
		{
			CommandLine::Delegate& activeObserver = observerList[ i ];
			
			if( activeObserver == observer )
			{
				observerList.erase( observerList.begin() + i );
				--i;
			}
		}
	}
	#pragma endregion



	#pragma region Command Line Utility Functions
	//-----------------------------------------------------------------------------------------------
	STATIC inline bool Manager::IsOptionStartingCharacter( char character )
	{
		switch( character )
		{
		case '-':
		case '\\':
			return true;

		default:
			break;
		}
		return false;
	}

	//-----------------------------------------------------------------------------------------------
	STATIC inline bool Manager::IsQuoteCharacter( char character )
	{
		switch( character )
		{
		case '\"':
		case '\'':
			return true;

		default:
			break;
		}
		return false;
	}
	#pragma endregion
}

#endif //INCLUDED_COMMAND_LINE_MANAGER_HPP
