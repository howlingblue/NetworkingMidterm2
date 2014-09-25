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
	return nullptr;
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

	MainPacketType testPacket;
	testPacket.clientID = 0;
	testPacket.type = TYPE_Test;
	testPacket.number = 0;
	testPacket.timestamp = GetCurrentTimeSeconds();

	int numberOfBytesInNetworkQueue = m_serverSocket.GetNumberOfBytesInNetworkQueue();
	while( numberOfBytesInNetworkQueue > 0 )
	{
		int receiveResult = m_serverSocket.ReceiveBuffer( ( char* )&receivedPacket, sizeof( MainPacketType ), receivedIPAddress, receivedPort );

		printf( "Received packet from %s:%i. Responding with test packet...\n", receivedIPAddress.c_str(), receivedPort );
		m_serverSocket.SendBuffer( (char*)&testPacket, sizeof( MainPacketType ), receivedIPAddress, receivedPort ); 

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
