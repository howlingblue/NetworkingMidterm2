#pragma once
#ifndef INCLUDED_GAME_SERVER_HPP
#define INCLUDED_GAME_SERVER_HPP

//-----------------------------------------------------------------------------------------------
#include <set>
#include <vector>
#include "../../Common/Engine/UDPSocket.hpp"
#include "../../Common/Game/MidtermPacket.hpp"

typedef MidtermPacket MainPacketType;

//-----------------------------------------------------------------------------------------------
struct ClientInfo
{
	unsigned char id;
	std::string ipAddress;
	unsigned short portNumber;
};

//-----------------------------------------------------------------------------------------------
class GameServer
{
	static const float SECONDS_BEFORE_CLIENT_TIMES_OUT;
	static const float SECONDS_BEFORE_GUARANTEED_PACKET_RESENT;
	static const float SECONDS_SINCE_LAST_CLIENT_PRINTOUT;

public:
	GameServer() { }
	~GameServer() { }

	void Initialize( const std::string& portNumber );
	void Update( float deltaSeconds );

private:
	ClientInfo* AddNewClient( const std::string& ipAddress, unsigned short portNumber );
	void BroadcastGameStateToClients();
	void PrintConnectedClients() const;
	ClientInfo* FindClientByAddress( const std::string& ipAddress, unsigned short portNumber );
	ClientInfo* FindClientByID( unsigned short clientID );
	void ProcessNetworkQueue();
	void ReceiveUpdateFromClient( const MainPacketType& updatePacket, ClientInfo* client );
	void RemoveAcknowledgedPacketFromClientQueue( const MainPacketType& ackPacket );
	void ResendUnacknowledgedPacketsToClient( ClientInfo* client );
	void ResetClient( ClientInfo* client );
	void SendPacketToClient( const MainPacketType& packet, ClientInfo* client );
	void UpdateGameState();

	Network::UDPSocket m_serverSocket;
	unsigned int m_nextClientID;
	std::vector< ClientInfo* > m_clientList;
};

#endif //INCLUDED_GAME_SERVER_HPP
