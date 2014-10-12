#ifndef INCLUDED_ENGINE_MACROS_HPP
#define INCLUDED_ENGINE_MACROS_HPP

#define VARIABLE_IS_UNUSED(x) (void)(x);

//Keywords
#define ABSTRACT
#define SINGLETON
#define STATIC 
#define VIRTUAL

//Conditional Execution
//#define MATH_DEBUG
#ifdef MATH_DEBUG
#define ONLY_DURING_MATH_DEBUGGING( x ) x
#else
#define ONLY_DURING_MATH_DEBUGGING( x )
#endif

//#define DEBUG_PHYSICS
#ifdef DEBUG_PHYSICS
#define ONLY_DURING_PHYSICS_DEBUGGING( x ) x
#else
#define ONLY_DURING_PHYSICS_DEBUGGING( x )
#endif

//Multiplatform
#if   defined( _WIN32 )
	#define ARCHITECTURE_32BIT
	#define PLATFORM_WINDOWS
#elif defined( _WIN64 )
	#define ARCHITECTURE_64BIT
	#define PLATFORM_WINDOWS
#endif

#endif //INCLUDED_ENGINE_MACROS_HPP
