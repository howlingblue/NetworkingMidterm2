#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "GameClient.hpp"
#include "../../../Common/Engine/EngineCommon.hpp"
#include "../../../Common/Engine/EngineMath.hpp"
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

		float leftStickMagnitude = m_controllers[ i ].getStickMagnitude( Xbox::LEFT_STICK );
		if( leftStickMagnitude != 0.0f )
		{
			m_tankInputs[ i ].tankMovementMagnitude = leftStickMagnitude;
			m_tankInputs[ i ].tankMovementHeading = ConvertRadiansToDegrees( m_controllers[ i ].getStickAngle( Xbox::LEFT_STICK ) );
		}
		else
			m_tankInputs[ i ].tankMovementMagnitude = 0.0f;


		float rightStickMagnitude = m_controllers[ i ].getStickMagnitude( Xbox::RIGHT_STICK );
		if( rightStickMagnitude != 0.0f )
		{
			m_tankInputs[ i ].turretMovementMagnitude = rightStickMagnitude;
			m_tankInputs[ i ].turretMovementHeading = ConvertRadiansToDegrees( m_controllers[ i ].getStickAngle( Xbox::RIGHT_STICK ) );
		}
		else
			m_tankInputs[ i ].turretMovementMagnitude = 0.0f;

		if( m_controllers[ i ].isTriggerPressed( Xbox::RIGHT_TRIGGER ) )
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
	ackPacket.type = TYPE_Acknowledgement;
	ackPacket.clientID = m_localEntity->GetID();
	ackPacket.number = 0;

	ackPacket.data.acknowledged.clientID = packet.clientID;
	ackPacket.data.acknowledged.packetNumber = packet.number;
	SendPacketToServer( ackPacket );
}

//-----------------------------------------------------------------------------------------------
void GameClient::HandleIncomingPacket( const MainPacketType& packet )
{
	switch( packet.type )
	{
	case TYPE_Reset:
		ResetGame( packet );
		m_currentState = STATE_InGame;
		break;
	case TYPE_Update:
		UpdateEntityFromPacket( packet );
		break;
	default:
		break;
	}
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
			printf( "Received packet from %s:%i.\n", receivedIPAddress.c_str(), receivedPort );
			m_packetQueue.insert( receivedPacket );
		}

		numberOfBytesInNetworkQueue = m_outputSocket.GetNumberOfBytesInNetworkQueue();
	}
}
//-----------------------------------------------------------------------------------------------
void GameClient::ProcessPacketQueue()
{
	std::set< MainPacketType, PacketComparer >::iterator packet;
	for( packet = m_packetQueue.begin(); packet != m_packetQueue.end(); ++packet )
	{
		if( m_currentState == STATE_WaitingToJoinServer|| m_currentState == STATE_InLobby )
		{
			if( packet->type != TYPE_Reset )
			{
				printf( "WARNING: Received non-reset packet from server when out of game!!\n" );
				continue;
			}
		}
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

	Entity* flagObjective = new Entity();
	m_currentWorld->SetObjective( flagObjective );
	flagObjective->SetClientPosition( resetPacket.data.reset.flagXPosition, resetPacket.data.reset.flagYPosition );
	flagObjective->SetServerPosition( resetPacket.data.reset.flagXPosition, resetPacket.data.reset.flagYPosition );
	flagObjective->SetItStatus( true );

	m_localEntity = new Entity();
	m_currentWorld->AddNewPlayer( m_localEntity );
	m_localEntity->SetID( resetPacket.clientID );

	m_localEntity->SetClientPosition( resetPacket.data.reset.xPosition, resetPacket.data.reset.yPosition );
	m_localEntity->SetServerPosition( resetPacket.data.reset.xPosition, resetPacket.data.reset.yPosition );
	m_localEntity->SetClientVelocity( resetPacket.data.reset.xVelocity, resetPacket.data.reset.yVelocity );
	m_localEntity->SetServerVelocity( resetPacket.data.reset.xVelocity, resetPacket.data.reset.yVelocity );
	m_localEntity->SetClientOrientation( resetPacket.data.reset.orientationDegrees );
	m_localEntity->SetServerOrientation( resetPacket.data.reset.orientationDegrees );
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendJoinRequestToServer( RoomID roomToJoin )
{
	MainPacketType joinPacket;
	joinPacket.type = TYPE_Join;
	joinPacket.clientID = 0;
	joinPacket.number = 0;

	joinPacket.data.joining.room = roomToJoin;
	SendPacketToServer( joinPacket );
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendPacketToServer( const MainPacketType& packet )
{
	packet.timestamp = GetCurrentTimeSeconds();

	int sendResult = m_outputSocket.SendBuffer( ( char* )&packet, sizeof( MidtermPacket ), m_serverAddress.c_str(), m_serverPort );
	if( sendResult < 0 )
	{
		int errorCode = WSAGetLastError();
		printf( "Unable to send packet to server at %s:%i. Error Code:%i.\n", m_serverAddress, m_serverPort, errorCode );
		exit( -6 );
	}
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendEntityTouchedIt( Entity* touchingEntity, Entity* itEntity )
{
	MainPacketType touchPacket;
	touchPacket.type = TYPE_Touch;
	touchPacket.number = 0;
	touchPacket.clientID = itEntity->GetID();
	touchPacket.timestamp = GetCurrentTimeSeconds();

	touchPacket.data.touch.instigatorID = touchingEntity->GetID();
	touchPacket.data.touch.receiverID = itEntity->GetID();
	SendPacketToServer( touchPacket );
}

//-----------------------------------------------------------------------------------------------
void GameClient::SendUpdatedPositionsToServer( float deltaSeconds )
{
	Vector2 currentPlayerPosition;
	MainPacketType updatePacket;
	updatePacket.type = TYPE_Update;
	updatePacket.number = 0;
	updatePacket.clientID = m_localEntity->GetID();
	updatePacket.timestamp = GetCurrentTimeSeconds();

	if( m_tankInputs[ 0 ].tankMovementMagnitude > 0.f )
	{
		float orientationRadians = ConvertDegreesToRadians( m_tankInputs[ 0 ].tankMovementHeading );
		Vector2 deltaVelocity( cos( orientationRadians ), -sin( orientationRadians ) );
		deltaVelocity *= m_tankInputs[ 0 ].tankMovementMagnitude * 10.f;

		if( m_localEntity->IsIt() )
			deltaVelocity *= .9f; //It player is slower than the rest.

		Vector2 currentPosition = m_localEntity->GetCurrentPosition();
		currentPosition.x += deltaVelocity.x;
		currentPosition.y += deltaVelocity.y;
		updatePacket.data.updated.xPosition = currentPosition.x;
		updatePacket.data.updated.yPosition = currentPosition.y;
		updatePacket.data.updated.xVelocity = deltaVelocity.x;
		updatePacket.data.updated.yVelocity = deltaVelocity.y;
		updatePacket.data.updated.orientationDegrees = m_tankInputs[ 0 ].tankMovementHeading;

		SendPacketToServer( updatePacket );
		m_secondsSinceLastSentUpdate = 0.f;
	}
	else if( m_secondsSinceLastSentUpdate > MAX_SECONDS_BETWEEN_PACKET_SENDS )
	{
		Vector2 currentPosition = m_localEntity->GetCurrentPosition();
		updatePacket.data.updated.xPosition = currentPosition.x;
		updatePacket.data.updated.yPosition = currentPosition.y;
		Vector2 currentVelocity = m_localEntity->GetCurrentVelocity();
		updatePacket.data.updated.xVelocity = currentVelocity.x;
		updatePacket.data.updated.yVelocity = currentVelocity.y;

		float currentOrientation = m_localEntity->GetCurrentOrientation();
		updatePacket.data.updated.orientationDegrees = currentOrientation;

		SendPacketToServer( updatePacket );
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
		newEntity->SetClientPosition( packet.data.updated.xPosition, packet.data.updated.yPosition );
		newEntity->SetClientVelocity( packet.data.updated.xVelocity, packet.data.updated.yVelocity );
		newEntity->SetClientOrientation( packet.data.updated.orientationDegrees );

		updatingEntity = newEntity;
		printf( "Adding new player: ID:%i", packet.clientID );
	}

	updatingEntity->SetServerPosition( packet.data.updated.xPosition, packet.data.updated.yPosition );
	updatingEntity->SetServerVelocity( packet.data.updated.xVelocity, packet.data.updated.yVelocity );
	updatingEntity->SetServerOrientation( packet.data.updated.orientationDegrees );
}
#pragma endregion


#pragma region Public Functions
//-----------------------------------------------------------------------------------------------
GameClient::GameClient( unsigned int screenWidth, unsigned int screenHeight )
	: m_screenSize( Vector2( (float) screenWidth, (float) screenHeight ) )
	, m_tankInputs( 1 )
	, m_currentState( STATE_WaitingToJoinServer )
	, m_currentWorld( nullptr )
	, m_lastReceivedGuaranteedPacketNumber( 0 )
	, m_lastReceivedPacketNumber( 0 )
	, m_keyboard( new Keyboard() )
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
			ProcessPacketQueue();

			if( m_currentState == STATE_WaitingToJoinServer )
			{
				printf( "Sending join packet to server @%s:%i.\n", m_serverAddress.c_str(), m_serverPort );
				SendJoinRequestToServer( ROOM_Lobby );
			}
		}
		break;
	case STATE_InLobby:
		{
			ProcessPacketQueue();

			static bool key1WasNotPreviouslyDown = true;
			static bool key2WasNotPreviouslyDown = true;
			static bool key3WasNotPreviouslyDown = true;
			static bool keyNWasNotPreviouslyDown = true;

			if( m_keyboard->KeyIsPressed( Keyboard::NUMBER_1 ) )
				;

			if( m_currentState == STATE_InLobby )
			{
				printf( "Sending touch packet to server @%s:%i.\n", m_serverAddress.c_str(), m_serverPort );
				//SendJoinRequestToServer();
			}
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
			bool localPlayerTouchedFlag = m_currentWorld->PlayerIsTouchingObjective( m_localEntity );
			if( localPlayerTouchedFlag )
			{
				SendEntityTouchedIt( m_localEntity, m_localEntity );
				m_currentState = STATE_InLobby;
			}
		}
		break;
	default:
		break;
	}
}
#pragma endregion
