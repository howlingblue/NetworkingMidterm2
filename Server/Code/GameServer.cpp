#include "GameServer.hpp"

#include "../../Common/Engine/EngineCommon.hpp"
#include "../../Common/Engine/EngineMath.hpp"
#include "../../Common/Engine/TimeInterface.hpp"

STATIC const float GameServer::SECONDS_BEFORE_CLIENT_TIMES_OUT = 5.f;
STATIC const float GameServer::SECONDS_BEFORE_GUARANTEED_PACKET_RESENT = 1.f;
STATIC const float GameServer::SECONDS_SINCE_LAST_CLIENT_PRINTOUT = 5.f;

//-----------------------------------------------------------------------------------------------
void GameServer::Initialize( const std::string& portNumber )
{
	m_serverSocket.Initialize();

	int bindingResult = m_serverSocket.Bind( "0.0.0.0", portNumber );
	if( bindingResult < 0 )
	{
		int errorCode = WSAGetLastError();
		printf( "Unable to bind server socket for listening. Error Code: %i.\n", errorCode );
		exit( -5 );
	}

	m_serverSocket.SetFunctionsToNonbindingMode();
}

//-----------------------------------------------------------------------------------------------
void GameServer::Update( float deltaSeconds )
{
	ProcessNetworkQueue();
	UpdateGameState();
	BroadcastGameStateToClients();

	//Print all connected Clients
	static float secondsSinceClientsLastPrinted = 0.f;
	if( secondsSinceClientsLastPrinted > SECONDS_SINCE_LAST_CLIENT_PRINTOUT )
	{
		PrintConnectedClients();
		secondsSinceClientsLastPrinted = 0.f;
	}
	secondsSinceClientsLastPrinted += deltaSeconds;
}



#pragma region Server Helper Functions
//-----------------------------------------------------------------------------------------------
ClientInfo* GameServer::AddNewClient( const std::string& ipAddress, unsigned short portNumber )
{
	m_clientList.push_back( new ClientInfo() );
	ClientInfo* newClient = m_clientList.back();

	newClient->id = m_nextClientID;
	++m_nextClientID;

	newClient->ipAddress = ipAddress;
	newClient->portNumber = portNumber;
	newClient->currentPacketNumber = 1;
	newClient->secondsSinceLastReceivedPacket = 0.f;
	return newClient;
}

//-----------------------------------------------------------------------------------------------
void GameServer::BroadcastGameStateToClients()
{
}

//-----------------------------------------------------------------------------------------------
ClientInfo* GameServer::FindClientByAddress( const std::string& ipAddress, unsigned short portNumber )
{
	ClientInfo* foundClient = nullptr;
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& client = m_clientList[ i ];
		if( ( client->ipAddress.compare( ipAddress ) == 0 ) && client->portNumber == portNumber )
		{
			foundClient = client;
			break;
		}
	}
	return foundClient;
}

//-----------------------------------------------------------------------------------------------
ClientInfo* GameServer::FindClientByID( unsigned short clientID )
{
	ClientInfo* foundClient = nullptr;
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& client = m_clientList[ i ];
		if( client->id == clientID )
		{
			foundClient = client;
			break;
		}
	}
	return foundClient;
}

//-----------------------------------------------------------------------------------------------
void GameServer::PrintConnectedClients() const
{
	if( m_clientList.size() == 0 )
	{
		printf( "No clients currently connected.\n\n" );
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::ProcessNetworkQueue()
{
	std::string receivedIPAddress;
	unsigned short receivedPort;
	MainPacketType receivedPacket;
	ClientInfo* receivedClient = nullptr;

	int numberOfBytesInNetworkQueue = m_serverSocket.GetNumberOfBytesInNetworkQueue();
	while( numberOfBytesInNetworkQueue > 0 )
	{
		int receiveResult = m_serverSocket.ReceiveBuffer( ( char* )&receivedPacket, sizeof( MainPacketType ), receivedIPAddress, receivedPort );

		receivedClient = FindClientByAddress( receivedIPAddress, receivedPort );
		if( receivedClient == nullptr )
		{
			if( receivedPacket.type != TYPE_Join )
			{
				printf( "WARNING: Received non-join packet from an unknown client at %s:%i.n", receivedIPAddress.c_str(), receivedPort );
				continue;
			}

			receivedClient = AddNewClient( receivedIPAddress, receivedPort );
			ResetClient( receivedClient );
			printf( "Received join packet from %s:%i. Added as client number %i.\n", receivedIPAddress.c_str(), receivedPort, receivedClient->id );
			continue;
		}
		
		printf( "Received packet from %s:%i.\n", receivedIPAddress.c_str(), receivedPort );

		numberOfBytesInNetworkQueue = m_serverSocket.GetNumberOfBytesInNetworkQueue();
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::ReceiveUpdateFromClient( const MainPacketType& updatePacket, ClientInfo* client )
{
}

//-----------------------------------------------------------------------------------------------
void GameServer::RemoveAcknowledgedPacketFromClientQueue( const MainPacketType& ackPacket )
{
}

//-----------------------------------------------------------------------------------------------
void GameServer::ResendUnacknowledgedPacketsToClient( ClientInfo* client )
{
}

//-----------------------------------------------------------------------------------------------
void GameServer::ResetClient( ClientInfo* client )
{
	if( m_itPlayerID == 0 ) //it should only be zero if this is our first ever player
	{
		m_itPlayerID = client->id;
	}

	client->xPosition = GetRandomFloatBetweenZeroandOne() * 600.f;
	if( client->id == m_itPlayerID )
		client->yPosition = 0;
	else
		client->yPosition = GetRandomFloatBetweenZeroandOne() * 600.f;
	client->xVelocity = 0.f;
	client->yVelocity = 0.f;
	client->orientationDegrees = 0.f;

	MainPacketType resetPacket;
	resetPacket.type = TYPE_Reset;
	resetPacket.clientID = client->id;
	resetPacket.number = client->currentPacketNumber;
	++client->currentPacketNumber;
	resetPacket.data.reset.itPlayerID = m_itPlayerID;
	resetPacket.data.reset.xPosition = client->xPosition;
	resetPacket.data.reset.yPosition = client->yPosition;
	resetPacket.data.reset.xVelocity = client->xVelocity;
	resetPacket.data.reset.yVelocity = client->yVelocity;
	resetPacket.data.reset.orientationDegrees = client->orientationDegrees;
	SendPacketToClient( resetPacket, client );
}

//-----------------------------------------------------------------------------------------------
void GameServer::SendPacketToClient( const MainPacketType& packet, ClientInfo* client )
{
	packet.timestamp = GetCurrentTimeSeconds();

	int sendResult = m_serverSocket.SendBuffer( ( char* )&packet, sizeof( MainPacketType ), client->ipAddress, client->portNumber );
	if( sendResult < 0 )
	{
		int errorCode = WSAGetLastError();
		printf( "Unable to send packet to client at %s:%i. Error Code:%i.\n", client->ipAddress.c_str(), client->portNumber, errorCode );
		exit( -42 );
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::UpdateGameState()
{

}
#pragma endregion
