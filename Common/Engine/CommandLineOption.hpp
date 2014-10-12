#pragma once
#ifndef INCLUDED_COMMAND_LINE_OPTION_HPP
#define INCLUDED_COMMAND_LINE_OPTION_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

//-----------------------------------------------------------------------------------------------
namespace CommandLine
{
	//-------------------------------------------------------------------------------------------
	struct Option
	{
		unsigned int GetNumberOfArguments() const { return arguments.size(); }

		//Data Members
		std::string option;
		std::vector< std::string > arguments;
	};
}

#endif //INCLUDED_COMMAND_LINE_OPTION_HPP