#pragma once
#ifndef INCLUDED_PLATFORM_SPECIFIC_HEADERS_HPP
#define INCLUDED_PLATFORM_SPECIFIC_HEADERS_HPP

#include "EngineMacros.hpp"

#if defined( PLATFORM_WINDOWS )
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <process.h>
#elif defined ( PLATFORM_UNIX )
#endif

#endif //INCLUDED_PLATFORM_SPECIFIC_HEADERS_HPP
