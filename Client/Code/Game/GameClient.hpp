#ifndef INCLUDED_GAME_CLIENT_HPP
#define INCLUDED_GAME_CLIENT_HPP

//-----------------------------------------------------------------------------------------------
#include <map>
#include <set>
#include <vector>
#include "../../../Common/Engine/Color.hpp"
#include "../../../Common/Engine/Texture.hpp"
#include "../../../Common/Engine/UDPSocket.hpp"
#include "../../../Common/Engine/Vector2.hpp"
#include "../../../Common/Engine/Xbox.hpp"
#include "../../../Common/Game/MidtermPacket.hpp"
#include "../../../Common/Game/Entity.hpp"
#include "../../../Common/Game/World.hpp"
#include "TankControlWrapper.h"

typedef MidtermPacket MainPacketType;

//-----------------------------------------------------------------------------------------------
class GameClient
{
	typedef unsigned char State;
	static const State STATE_WaitingForStart = 0;
	static const State STATE_InGame = 1;
	static const State STATE_WaitingForRestart = 2;
	static const State STATE_InLobby = 3;

	static const float		 WORLD_WIDTH;
	static const float		 WORLD_HEIGHT;
	static const float		 IT_PLAYER_SPEED_MULTIPLIER;
	static const float		 MAX_SECONDS_BETWEEN_PACKET_SENDS;
	static const float		 OBJECT_CONTACT_DISTANCE;
	static const float		 SECONDS_TO_WAIT_BEFORE_RESENDS;

	Vector2								m_screenSize;
	std::vector< bool >					m_isKeyDown;
	std::vector< Xbox::Controller >		m_controllers;
	std::vector< TankControlWrapper >	m_tankInputs;

	std::string				m_serverAddress;
	unsigned short			m_serverPort;
	Network::UDPSocket		m_outputSocket;
	std::set< MainPacketType, PacketComparer > m_packetQueue;
	unsigned int m_lastReceivedPacketNumber;
	unsigned int m_lastReceivedGuaranteedPacketNumber;

	unsigned short			m_itEntityID;
	State					m_currentState;
	World*					m_currentWorld;
	Entity*					m_localEntity;
	float					m_secondsSinceLastSentUpdate;

	//Input Functions
	void HandleInput( float deltaSeconds );
	float TransformKeyInputIntoAngle( bool upKeyIsPressed, bool rightKeyIsPressed, bool downKeyIsPressed, bool leftKeyIsPressed );

	//Game Helper Functions
	void AcknowledgePacket( const MainPacketType& packet );
	void HandleIncomingPacket( const MainPacketType& packet );
	void ProcessNetworkQueue();
	void ProcessPacketQueue();
	void ResetGame( const MainPacketType& resetPacket );
	void SendJoinRequestToServer();
	void SendPacketToServer( const MainPacketType& packet );
	void SendEntityTouchedIt( Entity* touchingEntity, Entity* itEntity );
	void SendUpdatedPositionsToServer( float deltaSeconds );
	void UpdateEntityFromPacket( const MainPacketType& packet );

public:
	GameClient( unsigned int screenWidth, unsigned int screenHeight );
	~GameClient() {};

	bool HandleKeyDownEvent( unsigned char key );
	bool HandleKeyUpEvent( unsigned char key );

	void Start( const std::string& clientPort, const std::string& serverAddress, const std::string& serverPort );
	void Render() const;
	void Update( double timeSpentLastFrameSeconds );
};

#endif //INCLUDED_GAME_CLIENT_HPP