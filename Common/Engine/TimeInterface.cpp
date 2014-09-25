#include "TimeInterface.hpp"

#include <cassert>

//----------------------------------------------------------------------------------------------------
#ifdef _WIN32
#pragma region Windows Time Functions
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//----------------------------------------------------------------------------------------------------
static double g_secondsPerCount = 0.0;

//----------------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	assert( g_secondsPerCount != 0.0 );

	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter( &performanceCount );

	double timeSeconds = static_cast< double >( performanceCount.QuadPart ) * g_secondsPerCount;
	return timeSeconds;
}

//----------------------------------------------------------------------------------------------------
void InitializeTimer()
{
	if( g_secondsPerCount != 0.0 )
		return;

	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency( &countsPerSecond );
	g_secondsPerCount = 1.0 / static_cast< double >( countsPerSecond.QuadPart );
}
#pragma endregion
#endif //_WIN32
