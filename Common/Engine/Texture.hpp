#ifndef INCLUDED_TEXTURE_HPP
#define INCLUDED_TEXTURE_HPP

//-----------------------------------------------------------------------------------------------
#include <map>
#include <string>
#include "Vector2.hpp"

//-----------------------------------------------------------------------------------------------
class Texture
{
public:
	static enum FilteringMethod
	{
		nearestNeighbor		= 0x2600,
		linearInterpolation = 0x2601
	};

	static enum WrappingMode
	{
		clampToEdge			= 0x2900,
		repeatOverGeometry  = 0x2901
	};

private:
	static std::map< std::string, Texture* > s_activeTextureRegistry;
	static const unsigned int DEFAULT_LEVEL_OF_DETAIL, NO_IMAGE_BORDERS;

	std::string m_fileLocation;
	int m_imageWidth, m_imageHeight;
	unsigned int m_openGLTextureID;


	static Texture* GetTextureByName( const std::string& textureFileName, FilteringMethod filterMethod, WrappingMode wrapMode );

public:
	Texture::Texture()
		: m_openGLTextureID( 0 )
		, m_imageWidth( 0 )
		, m_imageHeight( 0 )
	{ }
	Texture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode );

	~Texture();

	unsigned int GetID() const { return m_openGLTextureID; }

	static Texture CreateOrGetTexture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode );
	static void CleanUpTextureRepository();
};

#endif //INCLUDED_TEXTURE_HPP
