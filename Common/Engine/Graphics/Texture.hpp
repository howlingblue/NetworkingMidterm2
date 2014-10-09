#ifndef INCLUDED_TEXTURE_HPP
#define INCLUDED_TEXTURE_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <map>
#include <string>
#include "../Math/FloatVector2.hpp"
#include "../Color.hpp"

namespace nv_dds
{
class CDDSImage;
}

//-----------------------------------------------------------------------------------------------
class Texture
{
public:
	static enum AttachmentType
	{
		ATTACHMENT_COLOR = 0,
		ATTACHMENT_DEPTH = 1,
		ATTACHMENT_STENCIL = 2
	};
	static enum FilteringMethod
	{
		nearestNeighbor		= 0,
		linearInterpolation = 1
	};

	static enum WrappingMode
	{
		clampToEdge			= 0,
		repeatOverGeometry  = 1
	};

private:
	static Texture* CreateTextureOfSizeWithColor( unsigned int width, unsigned int height, const Color& color );

	static std::map< std::string, Texture* > s_activeTextureRegistry;
	static std::map< std::wstring, Texture* > s_activeTextureRegistryUnicode;
	static const unsigned int DEFAULT_LEVEL_OF_DETAIL;

	std::string m_fileLocation;
	int m_imageWidth, m_imageHeight;
	unsigned int m_openGLTextureID;

public:
	Texture( unsigned int xWidth, unsigned int yHeight, const Color& color );
	Texture( AttachmentType attachmentType, unsigned int framebufferXWidth, unsigned int framebufferYHeight );
	Texture( FILE* textureFile, FilteringMethod filterMethod, WrappingMode wrapMode );
	Texture( nv_dds::CDDSImage* image, FilteringMethod filterMethod, WrappingMode wrapMode );

	~Texture();

	unsigned int GetID() const { return m_openGLTextureID; }
	
	static Texture* CreateOrGetTexture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode );
	static Texture* CreateOrGetTexture( const std::wstring& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode );

	static Texture* CreateColorTextureForFramebuffer( unsigned int windowXWidth, unsigned int windowYHeight );
	static Texture* CreateDepthTextureForFramebuffer( unsigned int windowXWidth, unsigned int windowYHeight );

	static Texture* CreateDefaultDiffuseTexture( unsigned int xWidth, unsigned int yHeight );
	static Texture* CreateDefaultNormalTexture( unsigned int xWidth, unsigned int yHeight );
	static Texture* CreateDefaultSpecularTexture( unsigned int xWidth, unsigned int yHeight );
	static Texture* CreateDefaultEmissiveTexture( unsigned int xWidth, unsigned int yHeight );
	static Texture* CreateDefaultParallaxTexture( unsigned int xWidth, unsigned int yHeight );

	static void CleanUpTextureRepositories();
};



//-----------------------------------------------------------------------------------------------
STATIC inline Texture* Texture::CreateDefaultDiffuseTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color WHITE = Color( 1.f, 1.f, 1.f, 1.f );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, WHITE );
}

//-----------------------------------------------------------------------------------------------
STATIC inline Texture* Texture::CreateDefaultNormalTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color Z_UP = Color( 0.5f, 0.5f, 1.f, 1.f );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, Z_UP );
}

//-----------------------------------------------------------------------------------------------
STATIC inline Texture* Texture::CreateDefaultSpecularTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color WHITE = Color( 1.f, 1.f, 1.f, 1.f );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, WHITE );
}

//-----------------------------------------------------------------------------------------------
STATIC inline Texture* Texture::CreateDefaultEmissiveTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color BLACK = Color( 0.f, 0.f, 0.f, 1.f );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, BLACK );
}

//-----------------------------------------------------------------------------------------------
STATIC inline Texture* Texture::CreateDefaultParallaxTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color BLACK = Color( 0.f, 0.f, 0.f, 1.f );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, BLACK );
}

#endif //INCLUDED_TEXTURE_HPP
