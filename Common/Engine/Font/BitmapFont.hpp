#ifndef INCLUDED_BITMAP_FONT_HPP
#define INCLUDED_BITMAP_FONT_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <map>
#include <vector>
#include "../Graphics/Texture.hpp"
#include "Glyph.hpp"

//-----------------------------------------------------------------------------------------------
class BitmapFont
{
public:
	enum Locale
	{
		NONE = 0,
		ENGLISH = 1
	};

private:
	std::string  m_fontName;
	Locale		 m_locale;
	unsigned int m_numberOfTextureSheets;
	float		 m_pixelHeight;

	std::vector< Texture* >	m_glyphAtlases;
	std::map< unsigned int, Glyph >	m_glyphData;

	void LoadFontMetadataFromXML( const std::string& fontMetadataXMLfileLocation );

public:
	BitmapFont()
		: m_fontName()
		, m_locale( NONE )
		, m_numberOfTextureSheets( 0 )
		, m_pixelHeight( 0.f )
	{ }

	BitmapFont( const std::string& fontDefinitionXMLlocation, 
				const std::string* fontAtlasLocations, unsigned int numberOfAtlases );

	const Glyph& GetGlyphForCharacter( char character ) const { return ( *m_glyphData.find( character ) ).second ; }
	const Texture* GetTextureSheet( unsigned int sheetNumber ) const { return m_glyphAtlases[ sheetNumber ]; }
	float GetWidthOfCharacter( char character, float fontHeight ) const;
};

#endif //INCLUDED_BITMAP_FONT_HPP
