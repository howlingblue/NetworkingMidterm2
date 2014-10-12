#include "CommandLineManager.hpp"

//-----------------------------------------------------------------------------------------------
namespace CommandLine
{
	STATIC Manager* Manager::s_activeCommandLineManager = nullptr;

	//-------------------------------------------------------------------------------------------
	STATIC void Manager::ReportCommandError( const std::string& errorDetails )
	{
		printf( "A command line error has occurred.\n\nDetails: " );
		printf( errorDetails.c_str() );
		RECOVERABLE_ERROR( "Command Prompt Error", "An error has occurred while processing the command prompt.\n\n Check the log for details, then choose whether to retry." );
	}

	//-------------------------------------------------------------------------------------------
	STATIC void Manager::RunCommandLine( const char* commandLineString )
	{
		FATAL_ASSERTION( s_activeCommandLineManager == nullptr, "Command Line Error!", 
			"Cannot use command line functions without an active Command Line Manager." );

		std::vector< std::string > commandLineTokens;
		SplitCommandLineIntoTokens( commandLineString, commandLineTokens );

		OptionList commandLineOptions;
		MergeTokensIntoOptions( commandLineTokens, commandLineOptions );

		//If we receive the help message, prep the queue to print only usage messages
		bool helpCommandActivated = false;
		for( unsigned int i = 0; i < commandLineOptions.size(); ++i )
		{
			const std::string& optionText = commandLineOptions[ i ].option;
			if( optionText == "help" || optionText == "h" || optionText == "?" )
			{
				CommandLine::Option helpOption = commandLineOptions[ i ];
				commandLineOptions.clear();
				commandLineOptions.push_back( helpOption );

				printf( "Here are the valid command line arguments for this Vingine game.\n" );
				printf( "%-7.7s %-15.15s %47.47s", "Short:", "Long:", "Command Arguments:\n");
				helpCommandActivated = true;
				break;
			}
		}

		//Otherwise let all observers look through the arguments and act accordingly.
		unsigned int numberOfObservers = s_activeCommandLineManager->m_observers.size();
		for( unsigned int i = 0; i < numberOfObservers; ++i )
		{
			const CommandLine::Delegate& observer = s_activeCommandLineManager->m_observers[ i ];
			observer.Invoke( commandLineOptions );
		}

		if( helpCommandActivated )
			FATAL_ERROR( "Command Prompt Error", "A help command was received from the command line.\nExamine command line usage in the log window, then press OK to quit." );
	}




	//-----------------------------------------------------------------------------------------------
	STATIC void Manager::ExtractQuoteDelimitedToken( std::string& out_token, CharacterIterator& cmdLineIterator )
	{
		char currentCharacter = cmdLineIterator.GetNextCharacter();

		while( !IsQuoteCharacter( currentCharacter ) && currentCharacter != STRING_TERMINATOR )
		{
			out_token.push_back( currentCharacter );

			currentCharacter = cmdLineIterator.GetNextCharacter();
		}

		if( currentCharacter == STRING_TERMINATOR )
		{
			std::string errorString( "Unable to find terminating quote character to finish command line token!\n" );
			errorString.append( "Current Token: \"" );
			errorString.append( out_token );
			errorString.append( "\"\n" );

			FATAL_ERROR( "Command Line Error", errorString );
		}

		currentCharacter = cmdLineIterator.GetNextCharacter();
	}

	//-----------------------------------------------------------------------------------------------
	STATIC void Manager::ExtractWhitespaceDelimitedToken( std::string& out_token, CharacterIterator& cmdLineIterator )
	{
		char currentCharacter = cmdLineIterator.GetCurrentCharacter();

		while( !isspace( currentCharacter ) && currentCharacter != STRING_TERMINATOR )
		{
			out_token.push_back( currentCharacter );

			currentCharacter = cmdLineIterator.GetNextCharacter();
		}
	}

	//-----------------------------------------------------------------------------------------------
	STATIC void Manager::SkipWhiteSpaceBetweenTokens( CharacterIterator& cmdLineIterator )
	{
		char currentCharacter = cmdLineIterator.GetCurrentCharacter();

		while( isspace( currentCharacter ) && currentCharacter != STRING_TERMINATOR )
		{
			currentCharacter = cmdLineIterator.GetNextCharacter();
		}
	}

	//-----------------------------------------------------------------------------------------------
	STATIC void Manager::SplitCommandLineIntoTokens( const char* commandLine, std::vector< std::string >& out_tokens )
	{
		CharacterIterator commandLineIterator( commandLine );

		char currentCharacter = commandLineIterator.GetCurrentCharacter();
		while( currentCharacter != STRING_TERMINATOR )
		{
			std::string currentToken;

			if( IsQuoteCharacter( currentCharacter ) )
			{
				ExtractQuoteDelimitedToken( currentToken, commandLineIterator );
			}
			else
			{
				ExtractWhitespaceDelimitedToken( currentToken, commandLineIterator );
			}

			out_tokens.push_back( currentToken );

			SkipWhiteSpaceBetweenTokens( commandLineIterator );
			currentCharacter = commandLineIterator.GetCurrentCharacter();
		}
	}

	//-----------------------------------------------------------------------------------------------
	STATIC void Manager::MergeTokensIntoOptions( const std::vector< std::string >& tokens, OptionList& out_options )
	{
		if( tokens.size() == 0 )
			return;

		std::vector< std::string >::const_iterator tokenIterator = tokens.begin();

		if( !IsOptionStartingCharacter( (*tokenIterator)[ 0 ] ) )
		{
			FATAL_ERROR( "Command Line Error", "Command line options cannot start with a non-option." );
		}

		for( ; tokenIterator != tokens.end(); )
		{
			CommandLine::Option newOption;

			newOption.option = *tokenIterator;
			while( IsOptionStartingCharacter( newOption.option[ 0 ] ) )
			{
				newOption.option.erase( newOption.option.begin() );
			}
			++tokenIterator;

			while( tokenIterator != tokens.end() && !IsOptionStartingCharacter( (*tokenIterator)[ 0 ] ) )
			{
				newOption.arguments.push_back( *tokenIterator );
				++tokenIterator;
			}

			out_options.push_back( newOption );
			if( tokenIterator == tokens.end() )
				break;
		}
	}
}