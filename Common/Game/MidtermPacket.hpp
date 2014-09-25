#pragma once
#ifndef INCLUDED_MIDTERM_PACKET_HPP
#define INCLUDED_MIDTERM_PACKET_HPP

//-----------------------------------------------------------------------------------------------
typedef unsigned char PacketType;
static const PacketType TYPE_Test = 0;


#pragma region Packet Data Types
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
	} data;

	bool IsGuaranteed() const;
};



//-----------------------------------------------------------------------------------------------
inline bool MidtermPacket::IsGuaranteed() const
{
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