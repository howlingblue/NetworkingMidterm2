#pragma once
#ifndef INCLUDED_GAME_SERVER_HPP
#define INCLUDED_GAME_SERVER_HPP

//-----------------------------------------------------------------------------------------------
#include <set>
#include <vector>
#include "../../Common/Engine/UDPSocket.hpp"
#include "../../Common/Game/Entity.hpp"
#include "../../Common/Game/FinalPacket.hpp"
//#include "../../Common/Game/MidtermPacket.hpp"
#include "../../Common/Game/World.hpp"

typedef FinalPacket MainPacketType;

//-----------------------------------------------------------------------------------------------
struct ClientInfo
{
	unsigned char id;
	std::string ipAddress;
	unsigned short portNumber;

	unsigned int currentPacketNumber;
	std::set< MainPacketType, FinalPacketComparer > unacknowledgedPackets;
	float secondsSinceLastReceivedPacket;

	RoomID currentRoom;
	bool ownsCurrentRoom;
	Entity* ownedPlayer;

	ClientInfo()
		: id( 0 )
		, currentPacketNumber( 1 )
		, secondsSinceLastReceivedPacket( 0.f )
		, currentRoom( ROOM_None )
		, ownsCurrentRoom( false )
		, ownedPlayer( nullptr )
	{ }

	unsigned int GetNextPacketNumber()
	{
		unsigned int nextPacketNumber = currentPacketNumber;
		++currentPacketNumber;
		return nextPacketNumber;
	}
};

//-----------------------------------------------------------------------------------------------
class GameServer
{
	static const size_t MAXIMUM_NUMBER_OF_GAME_ROOMS = 8;
	static const float SECONDS_BEFORE_CLIENT_TIMES_OUT;
	static const float SECONDS_BEFORE_GUARANTEED_PACKET_RESENT;
	static const float SECONDS_SINCE_LAST_CLIENT_PRINTOUT;

public:
	GameServer();
	~GameServer() { }

	void Initialize( const std::string& portNumber );
	void Update( float deltaSeconds );

private:
	//Utilities
	ClientInfo* FindClientByAddress( const std::string& ipAddress, unsigned short portNumber );
	ClientInfo* FindClientByID( unsigned short clientID );
	World* GetRoomWithID( RoomID roomID ) { return m_openRooms[ roomID - 1 ]; } //Rooms start at 1

	//Packet Senders
	void AcknowledgePacketFromClient( const MainPacketType& packet, ClientInfo* client );
	void BroadcastGameStateToClients();
	ErrorCode CreateNewRoomForClient( RoomID room, ClientInfo* client );
	void HandleTouchAndResetGame( const MainPacketType& touchPacket );
	ErrorCode MoveClientToRoom( ClientInfo* client, RoomID room, bool ownsRoom );
	void RefusePacketFromClient( const MainPacketType& packet, ClientInfo* client, ErrorCode errorCode );
	void ResetClient( ClientInfo* client );

	ClientInfo* AddNewClient( const std::string& ipAddress, unsigned short portNumber );
	void CloseRoom( RoomID room );
	ErrorCode CreateNewWorldAtRoomID( RoomID id );
	void PrintConnectedClients() const;
	void ProcessNetworkQueue();
	void ReceiveUpdateFromClient( const MainPacketType& updatePacket, ClientInfo* client );
	void RemoveAcknowledgedPacketFromClientQueue( const MainPacketType& ackPacket );
	void ResendUnacknowledgedPacketsToClient( ClientInfo* client );
	void SendPacketToClient( MainPacketType& packet, ClientInfo* client );
	void UpdateGameState( float deltaSeconds );


	//Data Members
	Network::UDPSocket m_serverSocket;

	unsigned int m_nextClientID;
	std::vector< ClientInfo* > m_clientList;

	unsigned short m_itPlayerID;
	World* m_openRooms[ MAXIMUM_NUMBER_OF_GAME_ROOMS ];
};

inline GameServer::GameServer()
	: m_nextClientID( 1 )
	, m_itPlayerID( 0 )
{
	for( unsigned char i = 0; i < MAXIMUM_NUMBER_OF_GAME_ROOMS; ++i )
	{
		m_openRooms[ i ] = nullptr;
	}
}

#endif //INCLUDED_GAME_SERVER_HPP
