#pragma once
#ifndef INCLUDED_MIDTERM_PACKET_HPP
#define INCLUDED_MIDTERM_PACKET_HPP

//-----------------------------------------------------------------------------------------------
typedef unsigned char PacketType;
static const PacketType TYPE_Test = 0;
static const PacketType TYPE_Acknowledgement = 20;
static const PacketType TYPE_Reset = 21;
static const PacketType TYPE_Touch = 22;
static const PacketType TYPE_Update = 23;
static const PacketType TYPE_Join = 24;

typedef unsigned char RoomID;
static const RoomID ROOM_Lobby = 254;
static const RoomID ROOM_None = 255;


#pragma region Packet Data Types
//-----------------------------------------------------------------------------------------------
struct AckPacket
{
	unsigned short clientID;
	unsigned int packetNumber;
};

//-----------------------------------------------------------------------------------------------
struct JoinPacket
{
	RoomID room;
};

//-----------------------------------------------------------------------------------------------
struct ResetPacket
{
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float orientationDegrees;

	float flagXPosition;
	float flagYPosition;
};

//-----------------------------------------------------------------------------------------------
struct TouchPacket
{
	unsigned short instigatorID;
	unsigned short receiverID;
};

//-----------------------------------------------------------------------------------------------
struct UpdatePacket
{
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float orientationDegrees;
};
#pragma endregion



//-----------------------------------------------------------------------------------------------
struct MidtermPacket
{
	PacketType type;
	unsigned short clientID;
	unsigned int number;
	mutable double timestamp;

	union PacketData
	{
		AckPacket acknowledged;
		JoinPacket joining;
		ResetPacket reset;
		TouchPacket touch;
		UpdatePacket updated;
	} data;

	bool IsGuaranteed() const;
};



//-----------------------------------------------------------------------------------------------
inline bool MidtermPacket::IsGuaranteed() const
{
	switch( type )
	{
	case TYPE_Reset:
	case TYPE_Touch:
		return true;

	case TYPE_Acknowledgement:
	case TYPE_Update:
	case TYPE_Join:
	default:
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
class PacketComparer
{
public:
	bool operator() (const MidtermPacket& lhs, const MidtermPacket& rhs) const
	{
		if( lhs.number < rhs.number )
			return true;
		return false;
	}
};

#endif //INCLUDED_MIDTERM_PACKET_HPP