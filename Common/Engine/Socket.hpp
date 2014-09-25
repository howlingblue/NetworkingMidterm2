#pragma once
#ifndef INCLUDED_SOCKET_HPP
#define INCLUDED_SOCKET_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )

#define ABSTRACT

//-----------------------------------------------------------------------------------------------
namespace Network
{
	//-----------------------------------------------------------------------------------------------
	enum AddressType
	{
		TYPE_Unknown = 0,
		TYPE_IPv4 = 4,
		TYPE_IPv6 = 6
	};

	//-----------------------------------------------------------------------------------------------
	enum Protocol
	{
		PROTOCOL_TCP = 0,
		PROTOCOL_UDP = 1
	};

	//-----------------------------------------------------------------------------------------------
	ABSTRACT class Socket
	{
	public:
		Socket();

		virtual int Initialize( const std::string& address, const std::string& portNumber ) = 0;
		virtual int Bind() = 0;
		virtual int Cleanup() = 0;

		bool IsInitialized() const { return m_isInitialized; }

	protected:
		addrinfo* m_addressInfo;
		SOCKET m_windowsSocket;
		WSADATA m_winsockData;
		bool m_isInitialized;
	};

	//-----------------------------------------------------------------------------------------------
	inline Socket::Socket()
		: m_isInitialized( false )
		, m_addressInfo( nullptr )
		, m_windowsSocket( INVALID_SOCKET )
	{
	}
}

#endif //INCLUDED_SOCKET_HPP
