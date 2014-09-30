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
	CreateNewWorld();
}

//-----------------------------------------------------------------------------------------------
void GameServer::Update( float deltaSeconds )
{
	ProcessNetworkQueue();
	UpdateGameState( deltaSeconds );
	BroadcastGameStateToClients();

	//Remove all clients that have timed out
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& client = m_clientList[ i ];
		client->secondsSinceLastReceivedPacket += deltaSeconds;
		if( client->secondsSinceLastReceivedPacket > SECONDS_BEFORE_CLIENT_TIMES_OUT )
		{
			printf( "Removed client %i @%s:%i for timing out.\n", client->id, client->ipAddress.c_str(), client->portNumber );
			m_clientList.erase( m_clientList.begin() + i );
			--i;
		}
	}

	//Resend all unacknowledged packets
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& client = m_clientList[ i ];

		std::set< MainPacketType, PacketComparer >::iterator unackedPacket;
		for( unackedPacket = client->unacknowledgedPackets.begin(); unackedPacket != client->unacknowledgedPackets.end(); ++unackedPacket )
		{
			SendPacketToClient( *unackedPacket, client );
		}
	}

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
	newClient->currentRoom = 0;
	return newClient;
}

//-----------------------------------------------------------------------------------------------
void GameServer::BroadcastGameStateToClients()
{
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& broadcastedClient = m_clientList[ i ];

		MainPacketType updatePacket;
		updatePacket.type = TYPE_Update;
		updatePacket.clientID = broadcastedClient->id;

		Vector2 currentPosition = broadcastedClient->ownedPlayer->GetCurrentPosition();
		updatePacket.data.updated.xPosition = currentPosition.x;
		updatePacket.data.updated.yPosition = currentPosition.y;

		Vector2 currentVelocity = broadcastedClient->ownedPlayer->GetCurrentVelocity();
		updatePacket.data.updated.xVelocity = currentVelocity.x;
		updatePacket.data.updated.yVelocity = currentVelocity.y;
		updatePacket.data.updated.orientationDegrees = broadcastedClient->ownedPlayer->GetCurrentOrientation();

		for( unsigned int j = 0; j < m_clientList.size(); ++j )
		{
			ClientInfo*& receivingClient = m_clientList[ j ];
			if( receivingClient->currentRoom != broadcastedClient->currentRoom )
				continue;

			updatePacket.number = receivingClient->currentPacketNumber;
			++receivingClient->currentPacketNumber;
			SendPacketToClient( updatePacket, receivingClient );
		}
	}
}

//-----------------------------------------------------------------------------------------------
unsigned char GameServer::CreateNewWorld()
{
	m_openRooms.push_back( new World() );
	World*& newWorld = m_openRooms.back();

	Vector2 objectivePosition( GetRandomFloatBetweenZeroandOne() * 600.f, 400.f );
	Entity* objectiveFlag = new Entity();
	objectiveFlag->SetClientPosition( objectivePosition.x, objectivePosition.y );
	objectiveFlag->SetServerPosition( objectivePosition.x, objectivePosition.y );
	newWorld->SetObjective( objectiveFlag );

	return m_openRooms.size() - 1;
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

	printf( "Connected Clients:\n\n" );
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		const ClientInfo* const& client = m_clientList[ i ];
		printf( "\t Client %i: @%s:%i, Last packet %f seconds ago, %i unacked packets\n", client->id, client->ipAddress.c_str(), 
												client->portNumber, client->secondsSinceLastReceivedPacket, client->unacknowledgedPackets.size() );
	}
	printf( "\n" );
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
		if( receiveResult < 0 )
		{
			int errorCode = WSAGetLastError();
			if( errorCode = 10035 )
				break; //The queue is empty

			printf( "Packet Receiving error! Error Code: %i", errorCode );
			exit( -14 );
		}

		receivedClient = FindClientByAddress( receivedIPAddress, receivedPort );
		if( receivedClient == nullptr )
		{
			if( receivedPacket.type != TYPE_Join )
			{
				printf( "WARNING: Received non-join packet from an unknown client at %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
				continue;
			}

			receivedClient = AddNewClient( receivedIPAddress, receivedPort );

			receivedClient->ownedPlayer = new Entity();
			m_openRooms[ 0 ]->AddNewPlayer( receivedClient->ownedPlayer );

			ResetClient( receivedClient );
			printf( "Received join packet from %s:%i. Added as client number %i.\n", receivedIPAddress.c_str(), receivedPort, receivedClient->id );
			continue;
		}
		
		printf( "Received packet from %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
		switch( receivedPacket.type )
		{
		case TYPE_Acknowledgement:
			RemoveAcknowledgedPacketFromClientQueue( receivedPacket );
			break;
		case TYPE_Update:
			ReceiveUpdateFromClient( receivedPacket, receivedClient );
			break;
		case TYPE_Touch:
			HandleTouchAndResetGame( receivedPacket );
			break;
		case TYPE_Join:
		default:
			printf( "WARNING: Received bad packet from %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
		}
		receivedClient->secondsSinceLastReceivedPacket = 0.f;
		numberOfBytesInNetworkQueue = m_serverSocket.GetNumberOfBytesInNetworkQueue();
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::HandleTouchAndResetGame( const MainPacketType& touchPacket )
{
	ClientInfo* itPlayer = FindClientByID( touchPacket.data.touch.receiverID );
	ClientInfo* touchingPlayer = FindClientByID( touchPacket.data.touch.instigatorID );

	printf( "Player %i touched the flag! Resetting Game...", touchingPlayer->id );

	World* roomWhereTouchOccurred = m_openRooms[ touchingPlayer->currentRoom ];

	Entity* newFlag = new Entity();
	Vector2 newFlagPosition( GetRandomFloatBetweenZeroandOne() * 600.f, 400.f );
	newFlag->SetClientPosition( newFlagPosition.x, newFlagPosition.y );
	newFlag->SetServerPosition( newFlagPosition.x, newFlagPosition.y );
	roomWhereTouchOccurred->SetObjective( newFlag );
	
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ResetClient( m_clientList[ i ] );
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::ReceiveUpdateFromClient( const MainPacketType& updatePacket, ClientInfo* client )
{
	client->ownedPlayer->SetClientPosition( updatePacket.data.updated.xPosition, updatePacket.data.updated.yPosition );
	client->ownedPlayer->SetClientVelocity( updatePacket.data.updated.xVelocity, updatePacket.data.updated.yVelocity );
	client->ownedPlayer->SetClientOrientation( updatePacket.data.updated.orientationDegrees );
}

//-----------------------------------------------------------------------------------------------
void GameServer::RemoveAcknowledgedPacketFromClientQueue( const MainPacketType& ackPacket )
{
	ClientInfo* acknowledgingClient = FindClientByID( ackPacket.clientID );

	std::set< MainPacketType, PacketComparer >::iterator unackedPacket;
	for( unackedPacket = acknowledgingClient->unacknowledgedPackets.begin(); 
		 unackedPacket != acknowledgingClient->unacknowledgedPackets.end(); 
		 ++unackedPacket )
	{
		if( unackedPacket->number == ackPacket.data.acknowledged.packetNumber )
		{
			printf( "Removing an acknowledged packet from client ID %i.\n", acknowledgingClient->id );
			acknowledgingClient->unacknowledgedPackets.erase( unackedPacket );
			return;
		}
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::ResendUnacknowledgedPacketsToClient( ClientInfo* client )
{
}

//-----------------------------------------------------------------------------------------------
void GameServer::ResetClient( ClientInfo* client )
{
	Vector2 startingPosition;
	startingPosition.x = GetRandomFloatBetweenZeroandOne() * 600.f;
	if( client->id == m_itPlayerID )
		startingPosition.y = 0;
	else
		startingPosition.y = GetRandomFloatBetweenZeroandOne() * 600.f;

	client->ownedPlayer->SetClientPosition( startingPosition.x, startingPosition.y );
	client->ownedPlayer->SetClientVelocity( 0.f, 0.f );
	client->ownedPlayer->SetClientOrientation( 0.f );

	MainPacketType resetPacket;
	resetPacket.type = TYPE_Reset;
	resetPacket.clientID = client->id;
	resetPacket.number = client->currentPacketNumber;
	++client->currentPacketNumber;

	Vector2 flagPosition = m_openRooms[ client->currentRoom ]->GetObjective()->GetCurrentPosition();
	resetPacket.data.reset.flagXPosition = flagPosition.x;
	resetPacket.data.reset.flagYPosition = flagPosition.y;

	resetPacket.data.reset.xPosition = startingPosition.x;
	resetPacket.data.reset.yPosition = startingPosition.y;
	resetPacket.data.reset.xVelocity = 0.f;
	resetPacket.data.reset.yVelocity = 0.f;
	resetPacket.data.reset.orientationDegrees = 0.f;
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

	if( packet.IsGuaranteed() )
	{
		client->unacknowledgedPackets.insert( packet );
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::UpdateGameState( float /*deltaSeconds*/ )
{
	for( unsigned int i = 0; i < m_openRooms.size(); ++i )
	{
		//m_openRooms[ i ].Update( deltaSeconds );
	}
// 	for( unsigned int i = 0; i < m_clientList.size(); ++i )
// 	{
// 		ClientInfo*& client = m_clientList[ i ];
// 
// 		client->xPosition += client->xVelocity * deltaSeconds;
// 		client->yPosition += client->yVelocity * deltaSeconds;
// 	}
}
#pragma endregion
