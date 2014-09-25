#pragma once
#ifndef INCLUDED_PACKET_TYPES_HPP
#define INCLUDED_PACKET_TYPES_HPP

//-----------------------------------------------------------------------------------------------
struct Position2DPacket
{
	float x;
	float y;
};

//-----------------------------------------------------------------------------------------------
struct ColorPosition2DPacket
{
	unsigned char id;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	float x;
	float y;
};

//-----------------------------------------------------------------------------------------------
struct GuaranteedPacket
{
	unsigned char id;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned int counter;
	mutable double timestamp;
	float x;
	float y;
};

//-----------------------------------------------------------------------------------------------
struct AcknowledgePacket
{
	unsigned char id;
	unsigned int counter;
	double timestamp;
};

#endif //INCLUDED_PACKET_TYPES_HPP
