#pragma once
#ifndef INCLUDED_TCP_SOCKET_HPP
#define INCLUDED_TCP_SOCKET_HPP

#include "Socket.hpp"

//-----------------------------------------------------------------------------------------------
namespace Network
{
	//-----------------------------------------------------------------------------------------------
	class TCPSocket : public Socket
	{
	public:
		TCPSocket();
		~TCPSocket();

		int Initialize( const std::string& address, const std::string& portNumber );
		int Accept( const TCPSocket& masterSocket );
		int Bind();
		int Connect();
		int Listen();
		int ReceiveBuffer( char*& buffer, int bufferLength );
		int SendBuffer( char*& buffer, int bufferLength );
		int Cleanup();
	};



	//-----------------------------------------------------------------------------------------------
	inline TCPSocket::TCPSocket()
	{
	}

	//-----------------------------------------------------------------------------------------------
	inline TCPSocket::~TCPSocket()
	{
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::Initialize( const std::string& address, const std::string& portNumber )
	{
		int initializationResult = WSAStartup( MAKEWORD(2,2), &m_winsockData ); //MAKEWORD(2,2) is asking for version 2.2
		if ( initializationResult != 0 ) 
			return -1;

		//Attempt to build protocol info struct
		struct addrinfo *ptr = NULL, hints;

		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC; //autodetect the address type
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		//Attempt to create address struct
		int addressingResult = getaddrinfo( address.c_str(), portNumber.c_str(), &hints, &m_addressInfo );
		if(addressingResult != 0)
			return -2;

		//Create and initialize socket object from info received
		m_windowsSocket = socket( m_addressInfo->ai_family, m_addressInfo->ai_socktype, m_addressInfo->ai_protocol );
		if( m_windowsSocket == INVALID_SOCKET)
			return -3;

		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::Accept( const TCPSocket& masterSocket )
	{
		m_windowsSocket = accept( masterSocket.m_windowsSocket, nullptr, nullptr );
		if( m_windowsSocket < 0 )
			return -4;
		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::Bind()
	{
		return bind( m_windowsSocket, m_addressInfo->ai_addr, (int) m_addressInfo->ai_addrlen );
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::Connect()
	{	
		//In some cases, there can allegedly be multiple results in addressInfo and we could try another result on failure
		//At this point, that functionality isn't supported
		return connect( m_windowsSocket, m_addressInfo->ai_addr, (int) m_addressInfo->ai_addrlen );
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::Listen()
	{
		return listen( m_windowsSocket, SOMAXCONN ) == SOCKET_ERROR;
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::ReceiveBuffer( char*& buffer, int bufferLength )
	{
		return recv( m_windowsSocket, buffer, bufferLength, 0 );
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::SendBuffer( char*& buffer, int bufferLength )
	{
		return send( m_windowsSocket, buffer, bufferLength, 0 );
	}

	//-----------------------------------------------------------------------------------------------
	inline int TCPSocket::Cleanup()
	{
		freeaddrinfo( m_addressInfo );
		closesocket( m_windowsSocket );
		WSACleanup();
		return 0;
	}
}

#endif //INCLUDED_TCP_SOCKET_HPP
