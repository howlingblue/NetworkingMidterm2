#include "../XML/pugixml.hpp"
#include "../StringConversion.hpp"
#include "BitmapFont.hpp"

//-----------------------------------------------------------------------------------------------
void BitmapFont::LoadFontMetadataFromXML( const std::string& fontMetadataXMLfileLocation )
{
	pugi::xml_document fontXMLmetadata;

	pugi::xml_parse_result xmlParseResult = fontXMLmetadata.load_file( fontMetadataXMLfileLocation.c_str() );
	if( xmlParseResult != true )
	{
		exit( -14 );
	}

	pugi::xml_node rootNode = fontXMLmetadata.child( "FontDefinition" );

	pugi::xml_node fontInfoNode = rootNode.child( "FontInfo" );
	m_fontName = fontInfoNode.attribute( "name" ).value();
	m_numberOfTextureSheets = ConvertStringToUnsignedInt( fontInfoNode.attribute( "numTextureSheets" ).value() );

	for (pugi::xml_node glyphData = rootNode.child( "Glyph" ); glyphData != false; glyphData = glyphData.next_sibling( "Glyph" ) )
	{
		Glyph newGlyph( ConvertStringToUnsignedInt( glyphData.attribute( "sheet" ).value() ),
						ConvertStringToFloatVector2OrDie( glyphData.attribute( "texCoordMins" ).value() ),
						ConvertStringToFloatVector2OrDie( glyphData.attribute( "texCoordMaxs" ).value() ),
						ConvertStringToFloat( glyphData.attribute( "ttfA" ).value() ),
						ConvertStringToFloat( glyphData.attribute( "ttfB" ).value() ),
						ConvertStringToFloat( glyphData.attribute( "ttfC" ).value() ) );

		unsigned int ucsIndex = ConvertStringToUnsignedInt( glyphData.attribute( "ucsIndex" ).value() );
		m_glyphData[ ucsIndex ] = newGlyph;
	}
}

//-----------------------------------------------------------------------------------------------
BitmapFont::BitmapFont( const std::string& fontDefinitionXMLlocation, 
	const std::string* fontAtlasLocations, unsigned int numberOfAtlases )
{
	LoadFontMetadataFromXML( fontDefinitionXMLlocation );

	for( unsigned int textureID = 0; textureID < numberOfAtlases; ++textureID )
	{
		Texture* newAtlas = Texture::CreateOrGetTexture( fontAtlasLocations[ textureID ], Texture::linearInterpolation, Texture::clampToEdge );
		m_glyphAtlases.push_back( newAtlas );
	}
}

//-----------------------------------------------------------------------------------------------
float BitmapFont::GetWidthOfCharacter( char character, float fontHeight ) const
{
	const Glyph& characterGlyph = ( *m_glyphData.find( character ) ).second;

	float preAdvanceWidth = characterGlyph.m_advanceBeforeDrawA * fontHeight;
	float characterWidth = characterGlyph.m_fontWidthB * fontHeight;
	float postAdvanceWidth = characterGlyph.m_advanceAfterDrawC * fontHeight;

	return preAdvanceWidth + characterWidth + postAdvanceWidth;
}
