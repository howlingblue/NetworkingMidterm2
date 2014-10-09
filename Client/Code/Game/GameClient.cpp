#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "GameClient.hpp"
#include "../../../Common/Engine/EngineCommon.hpp"
#include "../../../Common/Engine/Math/EngineMath.hpp"
#include "../../../Common/Engine/TimeInterface.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float		 GameClient::WORLD_WIDTH  = 32.0f;
STATIC const float		 GameClient::WORLD_HEIGHT = 18.0f;
STATIC const float		 GameClient::IT_PLAYER_SPEED_MULTIPLIER = 1.1f;
STATIC const float		 GameClient::MAX_SECONDS_BETWEEN_PACKET_SENDS = 1.f;
STATIC const float		 GameClient::OBJECT_CONTACT_DISTANCE = 10.f;
STATIC const float		 GameClient::SECONDS_TO_WAIT_BEFORE_RESENDS = 1.f;


#pragma region Input Functions
//-----------------------------------------------------------------------------------------------
void GameClient::HandleInput( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_controllers.size(); ++i )
	{
		m_controllers[ i ].Update( deltaSeconds );

		float leftStickMagnitude = m_controllers[ i ].GetStickMagnitude( Xbox::LEFT_STICK );
		if( leftStickMagnitude != 0.0f )
		{
			m_tankInputs[ i ].tankMovementMagnitude = leftStickMagnitude;
			m_tankInputs[ i ].tankMovementHeading = m_controllers[ i ].GetStickAngleDegrees( Xbox::LEFT_STICK );
		}
		else
			m_tankInputs[ i ].tankMovementMagnitude = 0.0f;


		float rightStickMagnitude = m_controllers[ i ].GetStickMagnitude( Xbox::RIGHT_STICK );
		if( rightStickMagnitude != 0.0f )
		{
			m_tankInputs[ i ].turretMovementMagnitude = rightStickMagnitude;
			m_tankInputs[ i ].turretMovementHeading = m_controllers[ i ].GetStickAngleDegrees( Xbox::RIGHT_STICK );
		}
		else
			m_tankInputs[ i ].turretMovementMagnitude = 0.0f;

		if( m_controllers[ i ].IsTriggerPressed( Xbox::RIGHT_TRIGGER ) )
			m_tankInputs[ i ].isShooting = true;
		else
			m_tankInputs[ i ].isShooting = false;
	}

	TankControlWrapper& firstWrapper = m_tankInputs[ 0 ];
	if ( m_keyboard->KeyIsPressedOrHeld( Keyboard::W ) || m_keyboard->KeyIsPressedOrHeld( Keyboard::D ) 
		|| m_keyboard->KeyIsPressedOrHeld( Keyboard::S ) || m_keyboard->KeyIsPressedOrHeld( Keyboard::A ) ) 
	{
		firstWrapper.tankMovementMagnitude = 1.0f;
		firstWrapper.tankMovementHeading = TransformKeyInputIntoAngle( m_keyboard->KeyIsPressedOrHeld( Keyboard::W ), 
																	   m_keyboard->KeyIsPressedOrHeld( Keyboard::D ),
																	   m_keyboard->KeyIsPressedOrHeld( Keyboard::S ),
																	   m_keyboard->KeyIsPressedOrHeld( Keyboard::A ) );
	}

	if ( m_keyboard->KeyIsPressedOrHeld( Keyboard::I ) || m_keyboard->KeyIsPressedOrHeld( Keyboard::L ) 
		|| m_keyboard->KeyIsPressedOrHeld( Keyboard::K ) || m_keyboard->KeyIsPressedOrHeld( Keyboard::J ) ) 
	{
		firstWrapper.turretMovementMagnitude = 1.0f;
		firstWrapper.turretMovementHeading = TransformKeyInputIntoAngle( m_keyboard->KeyIsPressedOrHeld( Keyboard::I ), 
																		 m_keyboard->KeyIsPressedOrHeld( Keyboard::L ),
																		 m_keyboard->KeyIsPressedOrHeld( Keyboard::K ),
																		 m_keyboard->KeyIsPressedOrHeld( Keyboard::J ) );
	}

	if( m_keyboard->KeyIsPressed( Keyboard::SPACEBAR ) )
		firstWrapper.isShooting = true;
	else
		firstWrapper.isShooting = false;
}

//-----------------------------------------------------------------------------------------------
float GameClient::TransformKeyInputIntoAngle( bool upKeyIsPressed, bool rightKeyIsPressed, bool downKeyIsPressed, bool leftKeyIsPressed )
{
	static int upDirectionValue = 2, rightDirectionValue = 2, downDirectionValue = 2, leftDirectionValue = 2;
	
	if( upKeyIsPressed ) 
		upDirectionValue = 1;
	else 
		upDirectionValue = 0;

	if( rightKeyIsPressed ) 
		rightDirectionValue = 1;
	else 
		rightDirectionValue = 0;

	if( downKeyIsPressed ) 
		downDirectionValue = -1;
	else 
		downDirectionValue = 0;

	if( leftKeyIsPressed ) 
		leftDirectionValue = -1;
	else 
		leftDirectionValue = 0;

	float desiredOrientationRadians = atan2f( static_cast<float>( upDirectionValue + downDirectionValue ), 
											  static_cast<float>( rightDirectionValue + leftDirectionValue ) );
	float desiredOrientationDegrees = ConvertRadiansToDegrees( desiredOrientationRadians );
	return desiredOrientationDegrees;
}
#pragma endregion


#pragma region Game Helper Functions
//-----------------------------------------------------------------------------------------------
void GameClient::AcknowledgePacket(  const MainPacketType& packet )
{
	MainPacketType ackPacket;
	ackPacket.type = TYPE_Ack;

	if( m_localEntity != nullptr )
		ackPacket.clientID = m_localEntity->GetID();
	else
		ackPacket.clientID = 0;
	ackPacket.number = 0;
	ackPacket.timestamp = packet.timestamp;

	ackPacket.data.acknowledged.type = packet.type;
	ackPacket.data.acknowledged.number = packet.number;
	SendPacketToServer( ackPacket );
}

//-----------------------------------------------------------------------------------------------
void GameClient::ClearResendingPacket()
{
	delete m_packetToResend;
	m_packetToResend = nullptr;
}

//-----------------------------------------------------------------------------------------------
void GameClient::HandleIncomingPacket( const MainPacketType& packet )
{
	switch( packet.type )
	{
	case TYPE_Ack:
		{
			HandleServerAcknowledgement( packet );
			ClearResendingPacket();
		}
		break;
	case TYPE_Nack:
		{
			HandleServerRefusal( packet );
			ClearResendingPacket();
		}
		break;
	case TYPE_GameUpdate:
		if( m_currentState == STATE_InGame )
			UpdateEntityFromPacket( packet );
		else
			printf( "WARNING: Received game update while not in-game!\n" );
		break;
	case TYPE_LobbyUpdate:
		if( m_currentState == STATE_InLobby )
			UpdateLobbyStatus( packet );
		break;
	case TYPE_GameReset:
		if( m_currentState == STATE_WaitingForGameStart || m_currentState == STATE_InGame )
			ResetGame( packet );
		ClearResendingPacket();
		break;
	case TYPE_Fire:
		m_currentWorld->HandleFireEventFromPlayer( m_currentWorld->FindPlayerWithID( packet.data.gunfire.instigatorID ) );
		if( packet.clientID == m_myClientID )
			ClearResendingPacket();
		break;
	case TYPE_Hit:
		{
			Entity* hitPlayer = m_currentWorld->FindPlayerWithID( packet.data.hit.targetID );
			Entity* instigatingPlayer = m_currentWorld->FindPlayerWithID( packet.data.hit.instigatorID );

			hitPlayer->SetHealth( hitPlayer->GetHealth() - packet.data.hit.damageDealt );

			if( hitPlayer->GetHealth() == 0 && instigatingPlayer != nullptr )
				instigatingPlayer->SetScore( instigatingPlayer->GetScore() + 1 );
		}
		break;
	case TYPE_Respawn:
		RespawnPlayer( packet );
		break;
	case TYPE_ReturnToLobby:
		{
			delete m_currentWorld;
			m_currentWorld = nullptr;
			m_localEntity = nullptr;
			m_myClientID = ID_None;

			m_currentState = STATE_InLobby;
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
void GameClient::HandleServerAcknowledgement( const MainPacketType& packet )
{
	if( m_packetToResend == nullptr )
		return;

	if( ( packet.data.acknowledged.type != m_packetToResend->type ) ||
		( packet.data.acknowledged.number != m_packetToResend->number ) )
	{
		//We aren't getting acknowledgment of what we are expecting
		return;
	}

	switch( m_packetToResend->type )
	{
	case TYPE_CreateRoom:
	case TYPE_JoinRoom:
		{
			if( m_packetToResend->data.joining.room == ROOM_Lobby )
				m_currentState = STATE_InLobby;
			else
				m_currentState = STATE_WaitingForGameStart;
		}
		ClearResendingPacket();
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
void GameClient::HandleServerRefusal( const MainPacketType& packet )
{
	if( m_packetToResend == nullptr )
		return;
	if( ( packet.data.acknowledged.type != m_packetToResend->type ) ||
		( packet.data.acknowledged.number != m_packetToResend->number ) )
	{
		//We aren't getting acknowledgment of what we are expecting
		return;
	}

	switch( m_packetToResend->type )
	{
	case TYPE_CreateRoom:
	case TYPE_JoinRoom:
		m_currentState = STATE_InLobby;
	default:
		break;
	}
	ClearResendingPacket();
}

//-----------------------------------------------------------------------------------------------
void GameClient::ProcessNetworkQueue()
{
	std::string receivedIPAddress;
	unsigned short receivedPort;
	MainPacketType receivedPacket;

	// Fill queue
	int numberOfBytesInNetworkQueue = m_outputSocket.GetNumberOfBytesInNetworkQueue();
	while( numberOfBytesInNetworkQueue > 0 )
	{
		int receiveResult = m_outputSocket.ReceiveBuffer( ( char* )&receivedPacket, sizeof( MainPacketType ), receivedIPAddress, receivedPort );
		if( receiveResult < 0 )
		{
			int errorCode = WSAGetLastError();
			if( errorCode != 10054)
			{
				printf( "Unable to receive packet.\n" );
				exit( -63 );
			}
		}
		else
		{
			//printf( "Received packet from %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
			m_packetQueue.insert( receivedPacket );
		}

		numberOfBytesInNetworkQueue = m_outputSocket.GetNumberOfBytesInNetworkQueue();
	}
}
//-----------------------------------------------------------------------------------------------
void GameClient::ProcessPacketQueue()
{
	std::set< MainPacketType, FinalPacketComparer >::iterator packet;
	for( packet = m_packetQueue.begin(); packet != m_packetQueue.end(); ++packet )
	{
		//Check for badly timed packets
		if( m_currentState == STATE_WaitingToJoinServer || m_currentState == STATE_InLobby )
		{
			if( packet->type != TYPE_Ack && packet->type != TYPE_LobbyUpdate )
			{
				printf( "WARNING: Received invalid packet from server while waiting for room entry!!\n" );
				continue;
			}
		}


		//Exclude old packets
		if( packet->IsGuaranteed() )
		{
			if( packet->number <= m_lastReceivedGuaranteedPacketNumber )
				continue;
		}
		else
		{
			if( packet->number <= m_lastReceivedPacketNumber )
				continue;
		}


		HandleIncomingPacket( *packet );
		

		//Acknowledge and update packet numbers
		if( packet->IsGuaranteed() )
		{
			AcknowledgePacket( *packet );
			m_lastReceivedPacketNumber = packet->number;

			if( m_lastReceivedGuaranteedPacketNumber > m_lastReceivedPacketNumber )
				m_lastReceivedPacketNumber = m_lastReceivedGuaranteedPacketNumber;
		}
		else
		{
			m_lastReceivedPacketNumber = packet->number;
		}
	}
	m_packetQueue.clear();
}

//-----------------------------------------------------------------------------------------------
void GameClient::ResetGame( const MainPacketType& resetPacket )
{
	if( m_currentWorld != nullptr )
		delete m_currentWorld;
	m_currentWorld = new World();

	m_localEntity = new Entity();
	m_currentWorld->AddNewPlayer( m_localEntity );
	m_localEntity->SetID( resetPacket.data.reset.id );
	m_myClientID = resetPacket.data.reset.id;

	m_localEntity->SetClientPosition( resetPacket.data.reset.xPosition, resetPacket.data.reset.yPosition );
	m_localEntity->SetServerPosition( resetPacket.data.reset.xPosition, resetPacket.data.reset.yPosition );
	m_localEntity->SetClientOrientation( resetPacket.data.reset.orientationDegrees );
	m_localEntity->SetServerOrientation( resetPacket.data.reset.orientationDegrees );
	m_localEntity->SetHealth( World::MAX_HEALTH );
	m_localEntity->SetScore( 0 );

	m_currentState = STATE_InGame;
}

//-----------------------------------------------------------------------------------------------
void GameClient::RespawnPlayer( const MainPacketType& respawnPacket )
{
	Entity* playerThatShouldBeDead = m_currentWorld->FindPlayerWithID( respawnPacket.clientID );
	if( playerThatShouldBeDead != nullptr )
		m_currentWorld->RemovePlayer( playerThatShouldBeDead );

	printf( "Respawning player ID:%i", respawnPacket.clientID );

	Entity* respawnedEntity = new Entity();
	m_currentWorld->AddNewPlayer( respawnedEntity );
	respawnedEntity->SetID( respawnPacket.clientID );
	respawnedEntity->SetClientPosition( respawnPacket.data.respawn.xPosition, respawnPacket.data.respawn.yPosition );
	respawnedEntity->SetClientVelocity( 0.f, 0.f );
	respawnedEntity->SetClientAcceleration( 0.f, 0.f );
	respawnedEntity->SetClientOrientation( respawnPacket.data.respawn.orientationDegrees );

	respawnedEntity->SetServerPosition( respawnPacket.data.respawn.xPosition, respawnPacket.data.respawn.yPosition );
	respawnedEntity->SetServerVelocity( 0.f, 0.f );
	respawnedEntity->SetServerAcceleration( 0.f, 0.f );
	respawnedEntity->SetServerOrientation( respawnPacket.data.respawn.orientationDegrees );

	respawnedEntity->SetHealth( 1 );
	respawnedEntity->SetScore( 0 );
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendEntityTouchedIt( Entity*, Entity* )
{
// 	MainPacketType touchPacket;
// 	touchPacket.type = TYPE_Touch;
// 	touchPacket.number = 0;
// 	touchPacket.clientID = itEntity->GetID();
// 	touchPacket.timestamp = GetCurrentTimeSeconds();
// 
// 	touchPacket.data.touch.instigatorID = touchingEntity->GetID();
// 	touchPacket.data.touch.receiverID = itEntity->GetID();
// 	SendPacketToServer( touchPacket );
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendJoinRequestToServer( RoomID roomToJoin )
{
	MainPacketType* joinPacket = new MainPacketType();
	joinPacket->type = TYPE_JoinRoom;
	joinPacket->clientID = 0;
	joinPacket->number = 0;

	joinPacket->data.joining.room = roomToJoin;
	SendPacketToServer( *joinPacket );
	m_packetToResend = joinPacket;
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendPacketToServer( MainPacketType& packet )
{
	packet.timestamp = GetCurrentTimeSeconds();

	int sendResult = m_outputSocket.SendBuffer( ( char* )&packet, sizeof( MainPacketType ), m_serverAddress.c_str(), m_serverPort );
	if( sendResult < 0 )
	{
		int errorCode = WSAGetLastError();
		printf( "Unable to send packet to server at %s:%i. Error Code:%i.\n", m_serverAddress, m_serverPort, errorCode );
		exit( -6 );
	}
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendRoomCreationRequestToServer( RoomID roomToCreate )
{
	MainPacketType* createPacket = new MainPacketType();
	createPacket->type = TYPE_CreateRoom;
	createPacket->clientID = 0;
	createPacket->number = 0;

	createPacket->data.creating.room = roomToCreate;

	SendPacketToServer( *createPacket );
	m_packetToResend = createPacket;
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendServerRoomRequestBasedOnStatus( RoomID room )
{
	if( m_packetToResend != nullptr )
		return;

	if( m_playersInRoom[ room - 1 ] == 0 ) //Remember: Rooms are 1 indexed!
		SendRoomCreationRequestToServer( room );
	else
		SendJoinRequestToServer( room );
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendUpdatedPositionsToServer( float deltaSeconds )
{
	FloatVector2 currentPlayerPosition;
	MainPacketType updatePacket;
	updatePacket.type = TYPE_GameUpdate;
	updatePacket.number = 0;
	if( m_localEntity != nullptr )
		updatePacket.clientID = m_localEntity->GetID();
	else
		return;
	updatePacket.timestamp = GetCurrentTimeSeconds();

	if( m_tankInputs[ 0 ].tankMovementMagnitude > 0.f )
	{
		float orientationRadians = ConvertDegreesToRadians( m_tankInputs[ 0 ].tankMovementHeading );
		FloatVector2 deltaVelocity( cos( orientationRadians ), -sin( orientationRadians ) );
		deltaVelocity *= m_tankInputs[ 0 ].tankMovementMagnitude * 10.f;

		if( m_localEntity->IsIt() )
			deltaVelocity *= .9f; //It player is slower than the rest.

		FloatVector2 currentPosition = m_localEntity->GetCurrentPosition();
		currentPosition.x += deltaVelocity.x;
		currentPosition.y += deltaVelocity.y;
		updatePacket.data.updatedGame.xPosition = currentPosition.x;
		updatePacket.data.updatedGame.yPosition = currentPosition.y;
		updatePacket.data.updatedGame.xVelocity = deltaVelocity.x;
		updatePacket.data.updatedGame.yVelocity = deltaVelocity.y;
		updatePacket.data.updatedGame.orientationDegrees = m_tankInputs[ 0 ].tankMovementHeading;

		SendPacketToServer( updatePacket );
		m_secondsSinceLastSentUpdate = 0.f;
	}
	else if( m_secondsSinceLastSentUpdate > MAX_SECONDS_BETWEEN_PACKET_SENDS )
	{
		FloatVector2 currentPosition = m_localEntity->GetCurrentPosition();
		updatePacket.data.updatedGame.xPosition = currentPosition.x;
		updatePacket.data.updatedGame.yPosition = currentPosition.y;
		FloatVector2 currentVelocity = m_localEntity->GetCurrentVelocity();
		updatePacket.data.updatedGame.xVelocity = currentVelocity.x;
		updatePacket.data.updatedGame.yVelocity = currentVelocity.y;

		float currentOrientation = m_localEntity->GetCurrentOrientation();
		updatePacket.data.updatedGame.orientationDegrees = currentOrientation;

		SendPacketToServer( updatePacket );
		m_secondsSinceLastSentUpdate = 0.f;
	}

	if( m_tankInputs[ 0 ].isShooting )
	{
		MainPacketType* firePacket = new MainPacketType();
		firePacket->type = TYPE_Fire;
		firePacket->number = 0;
		firePacket->clientID = m_myClientID;
		firePacket->timestamp = GetCurrentTimeSeconds();
		firePacket->data.gunfire.instigatorID = m_myClientID;

		SendPacketToServer( *firePacket );
		m_packetToResend = firePacket;
		m_secondsSinceLastSentUpdate = 0.f;
	}

	m_secondsSinceLastSentUpdate += deltaSeconds;
}

//-----------------------------------------------------------------------------------------------
void GameClient::UpdateEntityFromPacket( const MainPacketType& packet )
{
	Entity* updatingEntity = m_currentWorld->FindPlayerWithID( packet.clientID );

	if( updatingEntity == nullptr )
	{
		Entity* newEntity = new Entity();
		m_currentWorld->AddNewPlayer( newEntity );
		newEntity->SetID( packet.clientID );
		newEntity->SetClientPosition( packet.data.updatedGame.xPosition, packet.data.updatedGame.yPosition );
		newEntity->SetClientVelocity( packet.data.updatedGame.xVelocity, packet.data.updatedGame.yVelocity );
		newEntity->SetClientAcceleration( packet.data.updatedGame.xAcceleration, packet.data.updatedGame.yAcceleration );
		newEntity->SetClientOrientation( packet.data.updatedGame.orientationDegrees );

		updatingEntity = newEntity;
		printf( "Adding new player: ID:%i", packet.clientID );
	}

	updatingEntity->SetServerPosition( packet.data.updatedGame.xPosition, packet.data.updatedGame.yPosition );
	updatingEntity->SetServerVelocity( packet.data.updatedGame.xVelocity, packet.data.updatedGame.yVelocity );
	updatingEntity->SetServerAcceleration( packet.data.updatedGame.xAcceleration, packet.data.updatedGame.yAcceleration );
	updatingEntity->SetServerOrientation( packet.data.updatedGame.orientationDegrees );

	updatingEntity->SetHealth( packet.data.updatedGame.health );
	updatingEntity->SetScore( packet.data.updatedGame.score );
}
#pragma endregion


#pragma region Public Functions
//-----------------------------------------------------------------------------------------------
GameClient::GameClient( unsigned int screenWidth, unsigned int screenHeight )
	: m_screenSize( FloatVector2( (float) screenWidth, (float) screenHeight ) )
	, m_tankInputs( 1 )
	, m_currentState( STATE_WaitingToJoinServer )
	, m_currentWorld( nullptr )
	, m_lastReceivedGuaranteedPacketNumber( 0 )
	, m_lastReceivedPacketNumber( 0 )
	, m_keyboard( new Keyboard() )
	, m_packetToResend( nullptr )
{
	m_controllers.push_back( Xbox::Controller::ONE );
}

//-----------------------------------------------------------------------------------------------
bool GameClient::HandleKeyDownEvent( unsigned char key )
{
	m_keyboard->SetKeyDown( key );
	return true;
}

//-----------------------------------------------------------------------------------------------
bool GameClient::HandleKeyUpEvent( unsigned char key )
{
	m_keyboard->SetKeyUp( key );
	return true;
}

//-----------------------------------------------------------------------------------------------
void GameClient::Start( const std::string& clientPort, const std::string& serverAddress, const std::string& serverPort )
{
	m_outputSocket.Initialize();

	int bindingResult = m_outputSocket.Bind( "0.0.0.0", clientPort );
	if( bindingResult < 0 )
	{
		int errorCode = WSAGetLastError();
		printf( "Unable to bind game socket for listening. Error Code: %i.\n", errorCode );
		exit( -5 );
	}

	m_outputSocket.SetRemoteAddress( serverAddress );
	m_outputSocket.SetRemotePort( serverPort );
	m_outputSocket.SetFunctionsToNonbindingMode();

	m_serverAddress = serverAddress;
	m_serverPort = ( unsigned short )strtoul( serverPort.c_str(), 0, 0 );
}

//-----------------------------------------------------------------------------------------------
void GameClient::Render() const
{
 	glPushMatrix();
	glOrtho( 0.f, m_screenSize.x, m_screenSize.y, 0.f, 0.f, 1.f );

	if( m_currentWorld != nullptr )
		m_currentWorld->Render();

 	glPopMatrix();
}

//-----------------------------------------------------------------------------------------------
void GameClient::Update( double timeSpentLastFrameSeconds )
{
	float deltaSeconds = static_cast< float >( timeSpentLastFrameSeconds );

	ProcessNetworkQueue();

	switch( m_currentState )
	{
	case STATE_WaitingToJoinServer:
		{
			printf( "Sending join packet to server @%s:%i.\n", m_serverAddress.c_str(), m_serverPort );
			if( m_packetToResend == nullptr )
				SendJoinRequestToServer( ROOM_Lobby );

			ProcessPacketQueue();
		}
		break;
	case STATE_InLobby:
		{
			ProcessPacketQueue();

			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_1 ) )
				SendServerRoomRequestBasedOnStatus( 1 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_2 ) )
				SendServerRoomRequestBasedOnStatus( 2 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_3 ) )
				SendServerRoomRequestBasedOnStatus( 3 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_4 ) )
				SendServerRoomRequestBasedOnStatus( 4 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_5 ) )
				SendServerRoomRequestBasedOnStatus( 5 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_6 ) )
				SendServerRoomRequestBasedOnStatus( 6 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_7 ) )
				SendServerRoomRequestBasedOnStatus( 7 );
			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_8 ) )
				SendServerRoomRequestBasedOnStatus( 8 );
			
			static float secondsSinceLastResentPacket = 0.f;
			if( secondsSinceLastResentPacket > MAX_SECONDS_BETWEEN_PACKET_SENDS )
			{
				MainPacketType keepAlivePacket;
				keepAlivePacket.clientID = m_myClientID;
				keepAlivePacket.type = TYPE_KeepAlive;
				keepAlivePacket.number = 0;
				SendPacketToServer( keepAlivePacket );

				secondsSinceLastResentPacket = 0.f;
			}
			secondsSinceLastResentPacket += deltaSeconds;
		}
		break;
	case STATE_WaitingForGameStart:
		{
			printf( "Waiting for the game to start...\n" );

			ProcessPacketQueue();
		}
		break;
	case STATE_InGame:
		{
			ProcessPacketQueue();
			HandleInput( deltaSeconds );
			SendUpdatedPositionsToServer( deltaSeconds );

			if( m_currentWorld != nullptr )
				m_currentWorld->Update( deltaSeconds );

			//check for touches
// 			bool localPlayerTouchedFlag = m_currentWorld->PlayerIsTouchingObjective( m_localEntity );
// 			if( localPlayerTouchedFlag )
// 			{
// 				SendEntityTouchedIt( m_localEntity, m_localEntity );
// 				m_currentState = STATE_InLobby;
// 			}

			ClearResendingPacket();
		}
		break;
	default:
		break;
	}

	//Resend "Guaranteed" client packets in situations when we need them.
	static const float SECONDS_BEFORE_PACKETS_RESENT = 1.5f;
	if( m_packetToResend != nullptr )
	{
		if( m_secondsSinceLastResentPacket > SECONDS_BEFORE_PACKETS_RESENT )
		{
			SendPacketToServer( *m_packetToResend );
			m_secondsSinceLastResentPacket = 0.f;
		}
		m_secondsSinceLastResentPacket += deltaSeconds;
	}
	else
		m_secondsSinceLastResentPacket = 0.f;

	m_keyboard->Update();
}

//-----------------------------------------------------------------------------------------------
void GameClient::UpdateLobbyStatus( const MainPacketType& packet )
{
	printf( "IN LOBBY:\n" );

	for( unsigned int i = 0; i < MAX_NUMBER_OF_ROOMS; ++i )
	{
		m_playersInRoom[ i ] = packet.data.updatedLobby.playersInRoomNumber[ i ];

		printf( "\t Room %i: ", i +	1 );
		if( m_playersInRoom[ i ] == 0 )
			printf( "EMPTY - Press '%i' to create this room.\n", i + 1 );
		else
			printf( "%i players - Press '%i' to create this room.\n", m_playersInRoom[ i ], i + 1 );
	}
}
#pragma endregion
