#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "Game.hpp"
#include "../../../Common/Engine/EngineCommon.hpp"
#include "../../../Common/Engine/EngineMath.hpp"
#include "../../../Common/Engine/TimeInterface.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float		 Game::WORLD_WIDTH  = 32.0f;
STATIC const float		 Game::WORLD_HEIGHT = 18.0f;
STATIC const float		 Game::IT_PLAYER_SPEED_MULTIPLIER = 1.1f;
STATIC const float		 Game::MAX_SECONDS_BETWEEN_PACKET_SENDS = 1.f;
STATIC const float		 Game::OBJECT_CONTACT_DISTANCE = 10.f;
STATIC const float		 Game::SECONDS_TO_WAIT_BEFORE_RESENDS = 1.f;


#pragma region Input Functions
//-----------------------------------------------------------------------------------------------
void Game::HandleInput( float deltaSeconds )
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
	if ( m_isKeyDown[ 'W' ] || m_isKeyDown[ 'D' ] || m_isKeyDown[ 'S' ] || m_isKeyDown[ 'A' ] ) 
	{
		firstWrapper.tankMovementMagnitude = 1.0f;
		firstWrapper.tankMovementHeading = TransformKeyInputIntoAngle( m_isKeyDown[ 'W' ], m_isKeyDown[ 'D' ],
			m_isKeyDown[ 'S' ], m_isKeyDown[ 'A' ]);
	}

	if ( m_isKeyDown[ 'I' ] || m_isKeyDown[ 'L' ] || m_isKeyDown[ 'K' ] || m_isKeyDown[ 'J' ] ) 
	{
		firstWrapper.turretMovementMagnitude = 1.0f;
		firstWrapper.turretMovementHeading = TransformKeyInputIntoAngle( m_isKeyDown[ 'I' ], m_isKeyDown[ 'L' ],
			m_isKeyDown[ 'K' ], m_isKeyDown[ 'J' ]);
	}

	static bool spaceWasNotPreviouslyDown = true;
	if( m_isKeyDown[ VK_SPACE ] && spaceWasNotPreviouslyDown )
	{
		spaceWasNotPreviouslyDown = false;
		firstWrapper.isShooting = true;
	}
	else if( !m_isKeyDown[ VK_SPACE ] )
	{
		spaceWasNotPreviouslyDown = true;
	}

}

//-----------------------------------------------------------------------------------------------
float Game::TransformKeyInputIntoAngle( bool upKeyIsPressed, bool rightKeyIsPressed, bool downKeyIsPressed, bool leftKeyIsPressed )
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
void Game::AcknowledgePacket(  const MainPacketType& packet )
{
	MainPacketType ackPacket;
	ackPacket.type = TYPE_Acknowledgement;
	ackPacket.clientID = m_localPlayer->GetID();
	ackPacket.number = 0;

	ackPacket.data.acknowledged.clientID = packet.clientID;
	ackPacket.data.acknowledged.packetNumber = packet.number;
	SendPacketToServer( ackPacket );
}

//-----------------------------------------------------------------------------------------------
void Game::HandleIncomingPacket( const MainPacketType& packet )
{
	switch( packet.type )
	{
	case TYPE_Reset:
		ResetGame( packet );
		m_currentState = STATE_InGame;
		break;
	case TYPE_Update:
		UpdatePlayerFromPacket( packet );
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
void Game::ProcessNetworkQueue()
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
void Game::ProcessPacketQueue()
{
	std::set< MainPacketType, PacketComparer >::iterator packet;
	for( packet = m_packetQueue.begin(); packet != m_packetQueue.end(); ++packet )
	{
		if( m_currentState == STATE_WaitingForStart || m_currentState == STATE_WaitingForRestart )
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
void Game::ResetGame( const MainPacketType& resetPacket )
{
	if( m_currentWorld != nullptr )
		delete m_currentWorld;
	m_currentWorld = new World();

	m_itPlayerID = resetPacket.data.reset.itPlayerID;

	m_localPlayer = new Player();
	m_currentWorld->AddNewPlayer( m_localPlayer );
	m_localPlayer->SetID( resetPacket.clientID );

	m_localPlayer->SetClientPosition( resetPacket.data.reset.xPosition, resetPacket.data.reset.yPosition );
	m_localPlayer->SetServerPosition( resetPacket.data.reset.xPosition, resetPacket.data.reset.yPosition );
	m_localPlayer->SetClientVelocity( resetPacket.data.reset.xVelocity, resetPacket.data.reset.yVelocity );
	m_localPlayer->SetServerVelocity( resetPacket.data.reset.xVelocity, resetPacket.data.reset.yVelocity );
	m_localPlayer->SetClientOrientation( resetPacket.data.reset.orientationDegrees );
	m_localPlayer->SetServerOrientation( resetPacket.data.reset.orientationDegrees );

	if( m_localPlayer->GetID() == m_itPlayerID )
		m_localPlayer->SetItStatus( true );
	else
		m_localPlayer->SetItStatus( false );

}

//-----------------------------------------------------------------------------------------------
void Game::SendJoinRequestToServer()
{
	MainPacketType joinPacket;
	joinPacket.type = TYPE_Join;
	joinPacket.clientID = 0;
	joinPacket.number = 0;
	SendPacketToServer( joinPacket );
}

//-----------------------------------------------------------------------------------------------
void Game::SendPacketToServer( const MainPacketType& packet )
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
void Game::SendPlayerTouchedIt( Player* touchingPlayer, Player* itPlayer )
{
	MainPacketType touchPacket;
	touchPacket.type = TYPE_Touch;
	touchPacket.number = 0;
	touchPacket.clientID = itPlayer->GetID();
	touchPacket.timestamp = GetCurrentTimeSeconds();

	touchPacket.data.touch.instigatorID = touchingPlayer->GetID();
	touchPacket.data.touch.receiverID = itPlayer->GetID();
	SendPacketToServer( touchPacket );
}

//-----------------------------------------------------------------------------------------------
void Game::SendUpdatedPositionsToServer( float deltaSeconds )
{
	Vector2 currentPlayerPosition;
	MainPacketType updatePacket;
	updatePacket.type = TYPE_Update;
	updatePacket.number = 0;
	updatePacket.clientID = m_localPlayer->GetID();
	updatePacket.timestamp = GetCurrentTimeSeconds();

	if( m_tankInputs[ 0 ].tankMovementMagnitude > 0.f )
	{
		float orientationRadians = ConvertDegreesToRadians( m_tankInputs[ 0 ].tankMovementHeading );
		Vector2 deltaVelocity( cos( orientationRadians ), -sin( orientationRadians ) );
		deltaVelocity *= m_tankInputs[ 0 ].tankMovementMagnitude * 10.f;

		if( m_localPlayer->IsIt() )
			deltaVelocity *= .9f; //It player is slower than the rest.

		Vector2 currentPosition = m_localPlayer->GetCurrentPosition();
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
		Vector2 currentPosition = m_localPlayer->GetCurrentPosition();
		updatePacket.data.updated.xPosition = currentPosition.x;
		updatePacket.data.updated.yPosition = currentPosition.y;
		Vector2 currentVelocity = m_localPlayer->GetCurrentVelocity();
		updatePacket.data.updated.xVelocity = currentVelocity.x;
		updatePacket.data.updated.yVelocity = currentVelocity.y;

		float currentOrientation = m_localPlayer->GetCurrentOrientation();
		updatePacket.data.updated.orientationDegrees = currentOrientation;

		SendPacketToServer( updatePacket );
		m_secondsSinceLastSentUpdate = 0.f;
	}

	m_secondsSinceLastSentUpdate += deltaSeconds;
}

//-----------------------------------------------------------------------------------------------
void Game::UpdatePlayerFromPacket( const MainPacketType& packet )
{
	Player* updatingPlayer = m_currentWorld->FindPlayerWithID( packet.clientID );

	if( updatingPlayer == nullptr )
	{
		Player* newPlayer = new Player();
		m_currentWorld->AddNewPlayer( newPlayer );
		newPlayer->SetID( packet.clientID );
		newPlayer->SetClientPosition( packet.data.updated.xPosition, packet.data.updated.yPosition );
		newPlayer->SetClientVelocity( packet.data.updated.xVelocity, packet.data.updated.yVelocity );
		newPlayer->SetClientOrientation( packet.data.updated.orientationDegrees );

		if( newPlayer->GetID() == m_itPlayerID )
			newPlayer->SetItStatus( true );
		else
			newPlayer->SetItStatus( false );

		updatingPlayer = newPlayer;
		printf( "Adding new player: ID:%i", packet.clientID );
	}

	updatingPlayer->SetServerPosition( packet.data.updated.xPosition, packet.data.updated.yPosition );
	updatingPlayer->SetServerVelocity( packet.data.updated.xVelocity, packet.data.updated.yVelocity );
	updatingPlayer->SetServerOrientation( packet.data.updated.orientationDegrees );
}
#pragma endregion


#pragma region Public Functions
//-----------------------------------------------------------------------------------------------
Game::Game( unsigned int screenWidth, unsigned int screenHeight )
	: m_screenSize( Vector2( (float) screenWidth, (float) screenHeight ) )
	, m_isKeyDown( 256, false )
	, m_tankInputs( 1 )
	, m_currentState( STATE_WaitingForStart )
	, m_currentWorld( nullptr )
	, m_lastReceivedGuaranteedPacketNumber( 0 )
	, m_lastReceivedPacketNumber( 0 )
{
	m_controllers.push_back( Xbox::Controller::ONE );
}

//-----------------------------------------------------------------------------------------------
bool Game::HandleKeyDownEvent( unsigned char key )
{
	m_isKeyDown[ key ] = true;
	return true;
}

//-----------------------------------------------------------------------------------------------
bool Game::HandleKeyUpEvent( unsigned char key )
{
	m_isKeyDown[ key ] = false;
	return true;
}

//-----------------------------------------------------------------------------------------------
void Game::Start( const std::string& clientPort, const std::string& serverAddress, const std::string& serverPort )
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
void Game::Render() const
{
 	glPushMatrix();
	glOrtho( 0.f, m_screenSize.x, m_screenSize.y, 0.f, 0.f, 1.f );

	if( m_currentWorld != nullptr )
		m_currentWorld->Render();

 	glPopMatrix();
}

//-----------------------------------------------------------------------------------------------
void Game::Update( double timeSpentLastFrameSeconds )
{
	float deltaSeconds = static_cast< float >( timeSpentLastFrameSeconds );

	ProcessNetworkQueue();

	if( m_currentState == STATE_WaitingForStart )
	{
		ProcessPacketQueue();

		if( m_currentState == STATE_WaitingForStart )
		{
			printf( "Sending join packet to server @%s:%i.\n", m_serverAddress.c_str(), m_serverPort );
			SendJoinRequestToServer();
		}
	}
	else if( m_currentState == STATE_InGame )
	{
		ProcessPacketQueue();
		HandleInput( deltaSeconds );
		SendUpdatedPositionsToServer( deltaSeconds );

		if( m_currentWorld != nullptr )
			m_currentWorld->Update( deltaSeconds );

		//check for touches
		if( m_localPlayer->IsIt() )
		{
			Player* playerTouchingIt = m_currentWorld->FindPlayerTouchingIt();
			if( playerTouchingIt != nullptr )
			{
				SendPlayerTouchedIt( playerTouchingIt, m_localPlayer );
				m_currentState = STATE_WaitingForRestart;
			}
		}
	}
	else
	{
		ProcessPacketQueue();

		if( m_currentState == STATE_WaitingForRestart )
		{
			printf( "Sending touch packet to server @%s:%i.\n", m_serverAddress.c_str(), m_serverPort );
			//SendJoinRequestToServer();
		}
	}
}
#pragma endregion
