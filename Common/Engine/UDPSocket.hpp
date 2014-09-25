#pragma once
#ifndef INCLUDED_UDP_SOCKET_HPP
#define INCLUDED_UDP_SOCKET_HPP

#include <string>
#include "Socket.hpp"

//-----------------------------------------------------------------------------------------------
namespace Network
{
	//-----------------------------------------------------------------------------------------------
	class UDPSocket
	{
	public:
		UDPSocket();
		~UDPSocket();

		int Initialize();
		int Bind( const std::string& address, const std::string& portNumber );
		int ReceiveBuffer( char* buffer, int bufferLength, std::string& out_receivedIPAddress, unsigned short& out_receivedPortNumber );
		int SendBuffer( char* buffer, int bufferLength, const std::string& receiverIPAddress, unsigned short receiverPortNumber );
		int Cleanup();

		bool IsInitialized() { return m_isInitialized; }

		unsigned long GetNumberOfBytesInNetworkQueue();
		void GetRemoteAddress( std::string& out_remoteAddress );
		void GetRemotePort( std::string& out_remotePort );
		unsigned short GetRemotePort();

		void SetRemoteAddress( const std::string& address );
		void SetRemotePort( const std::string& portNumber ) { SetSockaddrPort( m_remoteAddress, (unsigned short)strtoul( portNumber.c_str(), 0, 0 ) ); }
		void SetRemotePort( unsigned short portNumber ) { SetSockaddrPort( m_remoteAddress, portNumber ); }
		int SetFunctionsToNonbindingMode();

	private:
		unsigned short GetSockaddrPort( sockaddr_in* addressStruct ) { return ntohs( addressStruct->sin_port ); }
		void GetSockaddrAddressAsString( sockaddr_in* socketAddress, char* out_addressBuffer, size_t sizeOfAddressBuffer );
		void SetSockaddrAddress( sockaddr_in* addressStruct, unsigned long newAddress ) { addressStruct->sin_addr.s_addr = newAddress; }
		void SetSockaddrPort( sockaddr_in* addressStruct, unsigned short portNumber ) { addressStruct->sin_port = htons( portNumber ); }

		sockaddr_in* m_socketAddress;
		sockaddr_in* m_remoteAddress;
		SOCKET m_winSocketID;
		WSADATA m_winsockData;
		bool m_isInitialized;
	};



	//-----------------------------------------------------------------------------------------------
	inline UDPSocket::UDPSocket()
		: m_socketAddress( nullptr )
		, m_remoteAddress( nullptr )
		, m_winSocketID( 0 )
		, m_isInitialized( false )
	{
	}

	//-----------------------------------------------------------------------------------------------
	inline UDPSocket::~UDPSocket()
	{
		Cleanup();
	}

	//-----------------------------------------------------------------------------------------------
	inline int UDPSocket::Initialize()
	{
		if ( WSAStartup( MAKEWORD(2,2), &m_winsockData ) != 0 )
		{
			printf( "Failed to initialize socket system. Error Code: %d", WSAGetLastError() );
			return -1;
		}

		m_socketAddress = new sockaddr_in();
		m_socketAddress->sin_family = AF_INET;
		m_remoteAddress = new sockaddr_in();
		m_remoteAddress->sin_family = AF_INET;

		//create socket
		if ( ( m_winSocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == SOCKET_ERROR )
		{
			printf( "Failed to initialize socket. windows Error Code: %d", WSAGetLastError() );
			return -2;
		}

		m_isInitialized = true;
		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	inline int UDPSocket::Bind( const std::string& address, const std::string& portNumber )
	{
		m_socketAddress->sin_family = AF_INET;
		SetSockaddrPort( m_socketAddress, (unsigned short)strtoul( portNumber.c_str(), 0, 0 ) );

		if( address.compare( "0.0.0.0" ) == 0 )
			m_socketAddress->sin_addr.s_addr = INADDR_ANY;
		else
			m_socketAddress->sin_addr.s_addr = inet_addr( address.c_str() );

		return bind( m_winSocketID, (struct sockaddr *) m_socketAddress, sizeof( sockaddr_in ) );
	}

	//-----------------------------------------------------------------------------------------------
	inline int UDPSocket::ReceiveBuffer( char* buffer, int bufferLength, std::string& out_receivedIPAddress, unsigned short& out_receivedPortNumber )
	{
		int sizeOfRemoteAddress = sizeof( sockaddr_in );
		int returnValue = recvfrom( m_winSocketID, buffer, bufferLength, 0, (sockaddr*)m_remoteAddress, &sizeOfRemoteAddress );

		static char addressBuffer[32];
		GetSockaddrAddressAsString( m_remoteAddress, addressBuffer, 32 );
		out_receivedIPAddress = addressBuffer;
		out_receivedPortNumber = GetSockaddrPort( m_remoteAddress );
		return returnValue;
	}

	//-----------------------------------------------------------------------------------------------
	inline int UDPSocket::SendBuffer( char* buffer, int bufferLength, const std::string& receiverIPAddress, unsigned short receiverPortNumber )
	{
		m_remoteAddress->sin_family = AF_INET;
		SetSockaddrAddress( m_remoteAddress, inet_addr( receiverIPAddress.c_str() ) );
		SetSockaddrPort( m_remoteAddress, receiverPortNumber );

		return sendto( m_winSocketID, buffer, bufferLength, 0, (sockaddr*)m_remoteAddress, sizeof( sockaddr_in ) );
	}

	//-----------------------------------------------------------------------------------------------
	inline int UDPSocket::Cleanup()
	{
		if( !IsInitialized() )
			return 0;

		delete m_socketAddress;
		delete m_remoteAddress;
		closesocket( m_winSocketID );
		WSACleanup();
		m_isInitialized = false;
		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	inline unsigned long UDPSocket::GetNumberOfBytesInNetworkQueue()
	{
		unsigned long numberOfBytesInQueue;

		ioctlsocket( m_winSocketID, FIONREAD, &numberOfBytesInQueue );
		return numberOfBytesInQueue;
	}

	//-----------------------------------------------------------------------------------------------
	inline void UDPSocket::GetSockaddrAddressAsString( sockaddr_in* socketAddress, char* out_addressBuffer, size_t sizeOfAddressBuffer )
	{
		inet_ntop( socketAddress->sin_family, (void*)&socketAddress->sin_addr, out_addressBuffer, sizeOfAddressBuffer );
	}

	//-----------------------------------------------------------------------------------------------
	inline void UDPSocket::SetRemoteAddress( const std::string& address )
	{
		if( address.compare( "0.0.0.0" ) == 0 )
			m_socketAddress->sin_addr.s_addr = INADDR_ANY;
		else
			m_socketAddress->sin_addr.s_addr = inet_addr( address.c_str() );
	}

	//-----------------------------------------------------------------------------------------------
	inline int UDPSocket::SetFunctionsToNonbindingMode()
	{
		static unsigned long LONG_TRUE = 1;

		return ioctlsocket( m_winSocketID, FIONBIO, &LONG_TRUE );
	}
}

#endif //INCLUDED_UDP_SOCKET_HPP
