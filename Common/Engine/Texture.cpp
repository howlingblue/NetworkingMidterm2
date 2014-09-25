#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "EngineCommon.hpp"
#include "stb_image.h"
#include "Texture.hpp"

//-----------------------------------------------------------------------------------------------
STATIC std::map< std::string, Texture* > Texture::s_activeTextureRegistry;
STATIC const unsigned int Texture::DEFAULT_LEVEL_OF_DETAIL = 0;
STATIC const unsigned int Texture::NO_IMAGE_BORDERS = 0;

//-----------------------------------------------------------------------------------------------
Texture::Texture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode )
	: m_openGLTextureID( 0 )
	, m_imageWidth( 0 )
	, m_imageHeight( 0 )
	, m_fileLocation( textureFileLocation )
{
	int numberOfColorComponents, requestedNumberOfColorComponents = 0;
	unsigned char* textureData = stbi_load( textureFileLocation.c_str(), &m_imageWidth, &m_imageHeight, 
											&numberOfColorComponents, requestedNumberOfColorComponents );
	// FIX: Handle error case where texture file does not load
	GLenum bufferTextureComponentFormat;
	if( numberOfColorComponents == 4 )
		bufferTextureComponentFormat = GL_RGBA;
	else
		bufferTextureComponentFormat = GL_RGB;
	GLenum cardTextureComponentFormat = bufferTextureComponentFormat;
	// FIX: This doesn't correctly handle exotic component formats

	glEnable( GL_TEXTURE_2D );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glGenTextures( 1, (GLuint*) &m_openGLTextureID );

	glBindTexture( GL_TEXTURE_2D, m_openGLTextureID );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMethod );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMethod );

	glTexImage2D( GL_TEXTURE_2D,
				  DEFAULT_LEVEL_OF_DETAIL,
				  cardTextureComponentFormat,
				  m_imageWidth,
				  m_imageHeight,
				  NO_IMAGE_BORDERS,
				  bufferTextureComponentFormat,
				  GL_UNSIGNED_BYTE,
				  textureData);

	glDisable( GL_TEXTURE_2D );

	stbi_image_free( textureData );

	s_activeTextureRegistry[ textureFileLocation ] = this;
}

//-----------------------------------------------------------------------------------------------
Texture::~Texture()
{
// 	glDeleteTextures(1, (GLuint*) &m_openGLTextureID );
// 	delete s_activeTextureRegistry[ m_fileLocation ];
// 	s_activeTextureRegistry.erase( m_fileLocation );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::GetTextureByName( const std::string& textureFileName, FilteringMethod filterMethod, WrappingMode wrapMode )
{
	std::map< std::string, Texture* >::iterator textureIterator = s_activeTextureRegistry.find( textureFileName );

	if( textureIterator == s_activeTextureRegistry.end() )
	{
		Texture* texture = new Texture( textureFileName, filterMethod, wrapMode );
		return texture;
	}

	return textureIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC Texture Texture::CreateOrGetTexture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode )
{
	Texture* texture = Texture::GetTextureByName( textureFileLocation, filterMethod, wrapMode );

	return *texture;
}

//-----------------------------------------------------------------------------------------------
STATIC void Texture::CleanUpTextureRepository()
{
	Texture* texture;
	for( std::map< std::string, Texture* >::iterator pair = s_activeTextureRegistry.begin();
													 pair != s_activeTextureRegistry.end();
													 ++pair )
	{
		texture = pair->second;
		glDeleteTextures(1, (GLuint*) &texture->m_openGLTextureID );
		delete texture;
	}
	s_activeTextureRegistry.clear();
}
