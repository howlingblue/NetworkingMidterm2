#ifndef INCLUDED_GLYPH_HPP
#define INCLUDED_GLYPH_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "../Math/FloatVector2.hpp"

//-----------------------------------------------------------------------------------------------
struct Glyph
{
	unsigned int m_textureSheetID;
	FloatVector2 m_textureUVMins;
	FloatVector2 m_textureUVMaxs;
	float		 m_advanceBeforeDrawA;
	float		 m_fontWidthB;
	float		 m_advanceAfterDrawC;

public:
	Glyph()
		: m_textureSheetID( 0 )
		, m_textureUVMins()
		, m_textureUVMaxs()
		, m_advanceBeforeDrawA( 0.f )
		, m_fontWidthB( 0.f )
		, m_advanceAfterDrawC( 0.f )
	{ }

	Glyph( unsigned int textureSheetID, const FloatVector2& textureUVMins, const FloatVector2& textureUVMaxs,
		   float advanceBeforeDrawA, float fontWidthB, float advanceAfterDrawC )
		   : m_textureSheetID( textureSheetID )
		   , m_textureUVMins( textureUVMins )
		   , m_textureUVMaxs( textureUVMaxs )
		   , m_advanceBeforeDrawA( advanceBeforeDrawA )
		   , m_fontWidthB( fontWidthB )
		   , m_advanceAfterDrawC( advanceAfterDrawC )
	{ }
};

#endif //INCLUDED_GLYPH_HPP
