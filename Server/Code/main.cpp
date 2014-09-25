#include <iostream>

#include "../../Common/Engine/TimeInterface.hpp"
#include "GameServer.hpp"

static const double LOCKED_FRAME_RATE_SECONDS = 1.0 / 60.0;
static const unsigned int MESSAGE_BUFFER_LENGTH = 512;

//-----------------------------------------------------------------------------------------------
enum ConnectionMode
{
	MODE_None = 0,
	MODE_Server = 1,
	MODE_Client = 2,
	MODE_TestClient = 3,
	MODE_TestServer = 4
};

//-----------------------------------------------------------------------------------------------
double WaitUntilNextFrameThenGiveFrameTime()
{
	static double targetTime = 0.0;
	double timeNow = GetCurrentTimeSeconds();

	while( timeNow < targetTime )
	{
		timeNow = GetCurrentTimeSeconds();
	}
	targetTime = timeNow + LOCKED_FRAME_RATE_SECONDS;

	return LOCKED_FRAME_RATE_SECONDS;
}

//-----------------------------------------------------------------------------------------------
int HandleCommandLine( int argc, char** argv, std::string& out_portNumber )
{
	if( argc != 2 )
	{
		std::cout << "Incorrect number of arguments!" << std::endl;
		std::cout << "Usage: " << argv[0] << " [Port Number]" << std::endl;
		return -1;
	}

	//Address
	out_portNumber = argv[ 1 ];

	return 0;
}

//-----------------------------------------------------------------------------------------------
int main( int argc, char** argv )
{
	InitializeTimer();

	ConnectionMode connectionMode = MODE_None;
	Network::AddressType addressType = Network::TYPE_Unknown;
	std::string ipAddress = "127.0.0.1"; //localhost
	Network::Protocol netProtocol = Network::PROTOCOL_TCP;
	std::string portNumber = "22"; //telnet
	
	int commandLineResult = HandleCommandLine( argc, argv, portNumber );
	if( commandLineResult != 0 )
		return -1;

	GameServer server;
	printf( "Initializing game server on UDP port %s...\n\n", portNumber.c_str() );
	server.Initialize( portNumber );

	static double timeSpentLastFrameSeconds = 0.0;
	while( true )
	{
		server.Update( static_cast< float >( timeSpentLastFrameSeconds ) );

		timeSpentLastFrameSeconds = WaitUntilNextFrameThenGiveFrameTime();
	}

	return 0;
}