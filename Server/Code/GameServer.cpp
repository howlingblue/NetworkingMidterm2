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
			if( client->ownsCurrentRoom )
				CloseRoom( client->currentRoom );
			m_clientList.erase( m_clientList.begin() + i );
			--i;
		}
	}

	//Resend all unacknowledged packets
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& client = m_clientList[ i ];
		ResendUnacknowledgedPacketsToClient( client );
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
void GameServer::AcknowledgePacketFromClient( const MainPacketType& packet, ClientInfo* client )
{
	MainPacketType ackPacket;
	ackPacket.type = TYPE_Ack;
	ackPacket.clientID = client->id;
	ackPacket.number = client->GetNextPacketNumber();
	ackPacket.timestamp = GetCurrentTimeSeconds();

	ackPacket.data.acknowledged.type = packet.type;
	ackPacket.data.acknowledged.number = packet.number;

	SendPacketToClient( ackPacket, client );
}

//-----------------------------------------------------------------------------------------------
void GameServer::RefusePacketFromClient( const MainPacketType& packet, ClientInfo* client, ErrorCode errorCode )
{
	MainPacketType nackPacket;
	nackPacket.type = TYPE_Nack;
	nackPacket.clientID = client->id;
	nackPacket.number = client->GetNextPacketNumber();
	nackPacket.timestamp = GetCurrentTimeSeconds();

	nackPacket.data.refused.type = packet.type;
	nackPacket.data.refused.type = packet.number;
	nackPacket.data.refused.errorCode = errorCode;

	SendPacketToClient( nackPacket, client );
}

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
	newClient->currentRoom = ROOM_None;
	return newClient;
}

//-----------------------------------------------------------------------------------------------
void GameServer::BroadcastGameStateToClients()
{
	MainPacketType lobbyUpdatePacket;
	lobbyUpdatePacket.type = TYPE_LobbyUpdate;
	lobbyUpdatePacket.clientID = ID_None;

	for( unsigned int i = 0; i < MAXIMUM_NUMBER_OF_GAME_ROOMS; ++i )
	{
		if( m_openRooms[ i ] == nullptr )
			lobbyUpdatePacket.data.updatedLobby.playersInRoomNumber[ i ] = 0;
		else
			lobbyUpdatePacket.data.updatedLobby.playersInRoomNumber[ i ] = m_openRooms[ i ]->GetNumberOfPlayers();
	}

	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& broadcastedClient = m_clientList[ i ];
		if( broadcastedClient->currentRoom == ROOM_Lobby )
		{
			lobbyUpdatePacket.number = broadcastedClient->currentPacketNumber;
			++broadcastedClient->currentPacketNumber;
			SendPacketToClient( lobbyUpdatePacket, broadcastedClient );
			continue; 
		}

		MainPacketType updatePacket;
		updatePacket.type = TYPE_GameUpdate;
		updatePacket.clientID = broadcastedClient->id;

		Vector2 currentPosition = broadcastedClient->ownedPlayer->GetCurrentPosition();
		updatePacket.data.updatedGame.xPosition = currentPosition.x;
		updatePacket.data.updatedGame.yPosition = currentPosition.y;

		Vector2 currentVelocity = broadcastedClient->ownedPlayer->GetCurrentVelocity();
		updatePacket.data.updatedGame.xVelocity = currentVelocity.x;
		updatePacket.data.updatedGame.yVelocity = currentVelocity.y;

		Vector2 currentAcceleration = broadcastedClient->ownedPlayer->GetCurrentAcceleration();
		updatePacket.data.updatedGame.xAcceleration = currentAcceleration.x;
		updatePacket.data.updatedGame.yAcceleration = currentAcceleration.y;

		updatePacket.data.updatedGame.orientationDegrees = broadcastedClient->ownedPlayer->GetCurrentOrientation();

		updatePacket.data.updatedGame.health = broadcastedClient->ownedPlayer->GetHealth();
		updatePacket.data.updatedGame.score = broadcastedClient->ownedPlayer->GetScore();

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
void GameServer::CloseRoom( RoomID room )
{
	for( unsigned int i = 0; i < m_clientList.size(); ++i )
	{
		ClientInfo*& client = m_clientList[ i ];

		if( client->currentRoom != room )
			continue;

		MoveClientToRoom( client, ROOM_Lobby, false );
	}

	delete m_openRooms[ room ];
	m_openRooms[ room ] = nullptr;
}

//-----------------------------------------------------------------------------------------------
ErrorCode GameServer::CreateNewRoomForClient( RoomID room, ClientInfo* client )
{
	if( room == ROOM_Lobby || room > MAXIMUM_NUMBER_OF_GAME_ROOMS )
		return ERROR_BadRoomID;

	if( client->ownsCurrentRoom )
	{
		printf( "WARNING: Client with ID %i tried to create a room even though it's a room owner!\n", client->id );
		MoveClientToRoom( client, client->currentRoom, true );
		return ERROR_None;
	}

	printf( "Attempting to create room %i for client ID %i...\n", room, client->id );
	ErrorCode creationError = CreateNewWorldAtRoomID( room );
	if( creationError != ERROR_None )
		return creationError;

	MoveClientToRoom( client, room, true );
	return ERROR_None;
}

//-----------------------------------------------------------------------------------------------
ErrorCode GameServer::CreateNewWorldAtRoomID( RoomID id )
{
	if( GetRoomWithID( id ) != nullptr )
	{
		return ERROR_RoomFull;
	}

	World* newWorld = new World();
	m_openRooms[ id - 1 ] = newWorld; //remember, room ids start at 1!

// 	Vector2 objectivePosition( GetRandomFloatBetweenZeroandOne() * 600.f, 400.f );
// 	Entity* objectiveFlag = new Entity();
// 	objectiveFlag->SetClientPosition( objectivePosition.x, objectivePosition.y );
// 	objectiveFlag->SetServerPosition( objectivePosition.x, objectivePosition.y );
// 	newWorld->SetObjective( objectiveFlag );

	return ERROR_None;
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
ErrorCode GameServer::MoveClientToRoom( ClientInfo* client, RoomID room, bool ownsRoom )
{
	if( room > MAXIMUM_NUMBER_OF_GAME_ROOMS || room == ROOM_None )
	{
		printf( "WARNING: Client with ID %i is trying to join a room that doesn't exist!\n", client->id );
		return ERROR_BadRoomID;
	}
	else if( room != ROOM_Lobby && GetRoomWithID( room ) == nullptr )
	{
		printf( "WARNING: Client with ID %i tried to join an empty room!\n", client->id );
		return ERROR_RoomEmpty;
	}

	if( client->currentRoom != ROOM_None && client->currentRoom != ROOM_Lobby ) //If it's a room that contains a world
	{
		m_openRooms[ client->currentRoom - 1 ]->RemovePlayer( client->ownedPlayer );
		client->ownedPlayer = nullptr;
		client->currentRoom = ROOM_None;
		client->ownsCurrentRoom = false;
	}

	client->currentRoom = room;
	client->ownsCurrentRoom = ownsRoom;

	if( client->currentRoom > ROOM_Lobby )
	{
		client->ownedPlayer = new Entity();
		m_openRooms[ client->currentRoom - 1 ]->AddNewPlayer( client->ownedPlayer );
		client->id = m_openRooms[ client->currentRoom - 1 ]->GetNextPlayerID();
		ResetClient( client );
	}
	return ERROR_None;
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
			if( receivedPacket.type != TYPE_JoinRoom )
			{
				printf( "WARNING: Received non-join packet from an unknown client at %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
				continue;
			}
			if( receivedPacket.data.joining.room == ROOM_None )
			{
				printf( "WARNING: Received join packet to invalid room from client at %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
				continue;
			}

			receivedClient = AddNewClient( receivedIPAddress, receivedPort );
			ErrorCode moveError = MoveClientToRoom( receivedClient, receivedPacket.data.joining.room, false );
			if( moveError == ERROR_None )
			{
				printf( "Received join packet from %s:%i. Added as client.\n", receivedIPAddress.c_str(), receivedPort );
				AcknowledgePacketFromClient( receivedPacket, receivedClient );
			}
			else
			{
				printf( "Refused join request from %s:%i. Error Code: %i.\n", receivedIPAddress.c_str(), receivedPort, moveError );
				RefusePacketFromClient( receivedPacket, receivedClient, moveError );
			}
			continue;
		}
		
		printf( "Received packet from %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
		switch( receivedPacket.type )
		{
		case TYPE_Ack:
			{
				RemoveAcknowledgedPacketFromClientQueue( receivedPacket, FindClientByAddress( receivedIPAddress, receivedPort ) );
			}
			break;
		case TYPE_GameUpdate:
			ReceiveUpdateFromClient( receivedPacket, receivedClient );
			break;
		case TYPE_CreateRoom:
			{
				ErrorCode creationError = CreateNewRoomForClient( receivedPacket.data.creating.room, receivedClient );
				if( creationError == ERROR_None )
				{
					printf( "Client at %s:%i has created room %i.\n", receivedIPAddress.c_str(), receivedPort, receivedPacket.data.joining.room );
					AcknowledgePacketFromClient( receivedPacket, receivedClient );
				}
				else
				{
					printf( "Refused creation request from client at %s:%i. Error Code: %i.\n", receivedIPAddress.c_str(), receivedPort, creationError );
					RefusePacketFromClient( receivedPacket, receivedClient, creationError );
				}
			}
			break;
		case TYPE_JoinRoom:
			{
				ErrorCode moveError = MoveClientToRoom( receivedClient, receivedPacket.data.joining.room, false );
				if( moveError == ERROR_None )
				{
					printf( "Client at %s:%i has moved to room %i.\n", receivedIPAddress.c_str(), receivedPort, receivedPacket.data.joining.room );
					AcknowledgePacketFromClient( receivedPacket, receivedClient );
				}
				else
				{
					printf( "Refused join request from client at %s:%i. Error Code: %i.\n", receivedIPAddress.c_str(), receivedPort, moveError );
					RefusePacketFromClient( receivedPacket, receivedClient, moveError );
				}
			}
			break;
		case TYPE_KeepAlive:
			// Just keep that client alive, baby...
			break;
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
// 	ClientInfo* itPlayer = FindClientByID( touchPacket.data.touch.receiverID );
// 	ClientInfo* touchingPlayer = FindClientByID( touchPacket.data.touch.instigatorID );
// 
// 	printf( "Player %i touched the flag in room %i! Returning all players to lobby...\n", touchingPlayer->id, touchingPlayer->currentRoom );
// 
// 	RoomID roomNumberOfTouch = touchingPlayer->currentRoom;
// 	World* roomWhereTouchOccurred = m_openRooms[ roomNumberOfTouch ];
// 
// 	//Get the room ready for another game (kinda useless right now, but hey...)
// 	Entity* newFlag = new Entity();
// 	Vector2 newFlagPosition( GetRandomFloatBetweenZeroandOne() * 600.f, 400.f );
// 	newFlag->SetClientPosition( newFlagPosition.x, newFlagPosition.y );
// 	newFlag->SetServerPosition( newFlagPosition.x, newFlagPosition.y );
// 	roomWhereTouchOccurred->SetObjective( newFlag );
// 	
// 	CloseRoom( roomNumberOfTouch );
}

//-----------------------------------------------------------------------------------------------
void GameServer::ReceiveUpdateFromClient( const MainPacketType& updatePacket, ClientInfo* client )
{
	if( client->ownedPlayer == nullptr )
		return;

	client->ownedPlayer->SetClientPosition( updatePacket.data.updatedGame.xPosition, updatePacket.data.updatedGame.yPosition );
	client->ownedPlayer->SetClientVelocity( updatePacket.data.updatedGame.xVelocity, updatePacket.data.updatedGame.yVelocity );
	client->ownedPlayer->SetClientAcceleration( updatePacket.data.updatedGame.xAcceleration, updatePacket.data.updatedGame.yAcceleration );
	client->ownedPlayer->SetClientOrientation( updatePacket.data.updatedGame.orientationDegrees );
	//Client doesn't get to set health and score, since we can't trust them.
}

//-----------------------------------------------------------------------------------------------
void GameServer::RemoveAcknowledgedPacketFromClientQueue( const MainPacketType& ackPacket, ClientInfo* client )
{
	std::set< MainPacketType, FinalPacketComparer >::iterator unackedPacket;
	for( unackedPacket = client->unacknowledgedPackets.begin(); 
		 unackedPacket != client->unacknowledgedPackets.end(); 
		 ++unackedPacket )
	{
		if( unackedPacket->number == ackPacket.data.acknowledged.number )
		{
			printf( "Removing an acknowledged packet from client ID %i.\n", client->id );
			client->unacknowledgedPackets.erase( unackedPacket );
			return;
		}
	}
}

//-----------------------------------------------------------------------------------------------
void GameServer::ResendUnacknowledgedPacketsToClient( ClientInfo* client )
{
	std::set< MainPacketType, FinalPacketComparer >::iterator unackedPacket;
	for( unackedPacket = client->unacknowledgedPackets.begin(); unackedPacket != client->unacknowledgedPackets.end(); ++unackedPacket )
	{
		SendPacketToClient( const_cast< MainPacketType& >( *unackedPacket ), client );
	}
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
	client->ownedPlayer->SetClientAcceleration( 0.f, 0.f );
	client->ownedPlayer->SetClientOrientation( 0.f );
	client->ownedPlayer->SetHealth( World::MAX_HEALTH );
	client->ownedPlayer->SetScore( 0 );

	MainPacketType resetPacket;
	resetPacket.type = TYPE_GameReset;
	resetPacket.clientID = client->id;
	resetPacket.number = client->currentPacketNumber;
	++client->currentPacketNumber;

	resetPacket.data.reset.id = client->id;
	resetPacket.data.reset.xPosition = startingPosition.x;
	resetPacket.data.reset.yPosition = startingPosition.y;
	resetPacket.data.reset.orientationDegrees = 0.f;
	SendPacketToClient( resetPacket, client );
}

//-----------------------------------------------------------------------------------------------
void GameServer::SendPacketToClient( MainPacketType& packet, ClientInfo* client )
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
	for( unsigned int i = 0; i < MAXIMUM_NUMBER_OF_GAME_ROOMS; ++i )
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
