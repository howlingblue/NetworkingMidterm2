#pragma once
#ifndef INCLUDED_VPACKET_HPP
#define INCLUDED_VPACKET_HPP

#include "PacketTypes.hpp"

//-----------------------------------------------------------------------------------------------
struct VPacket
{
	typedef unsigned char Type;
	static const Type TYPE_2DPosition = 1;
	static const Type TYPE_2DPositionColor = 2;
	
	//Members
	Type packetType;
	union PacketData
	{
		Position2DPacket position2D;
		ColorPosition2DPacket position2DColor;
	} packetData;
};

#endif //INCLUDED_VPACKET_HPP
