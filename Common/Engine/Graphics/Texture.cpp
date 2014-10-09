#include "../EngineMacros.hpp"
#include "../FileIOInterface.hpp"
#include "Renderer.hpp"
#include "stb_image.h"
#include "nv_dds.h"
#include "Texture.hpp"
#include "OpenGLRenderer.hpp"

//-----------------------------------------------------------------------------------------------
STATIC std::map< std::string, Texture* > Texture::s_activeTextureRegistry;
STATIC std::map< std::wstring, Texture* > Texture::s_activeTextureRegistryUnicode;
STATIC const unsigned int Texture::DEFAULT_LEVEL_OF_DETAIL = 0;

//-----------------------------------------------------------------------------------------------
Texture::Texture( unsigned int xWidth, unsigned int yHeight, const Color& color )
	: m_openGLTextureID( 0 )
	, m_imageWidth( xWidth )
	, m_imageHeight( yHeight )
	, m_fileLocation( "" )
{
	static const unsigned int NUMBER_OF_COLOR_COMPONENTS = 4;

	unsigned char redComponentAsChar   = static_cast< unsigned char >( color.r * 255.f );
	unsigned char greenComponentAsChar = static_cast< unsigned char >( color.g * 255.f );
	unsigned char blueComponentAsChar  = static_cast< unsigned char >( color.b * 255.f );
	unsigned char alphaComponentAsChar = static_cast< unsigned char >( color.a * 255.f );

	unsigned int totalNumberOfColorComponents = xWidth * yHeight * NUMBER_OF_COLOR_COMPONENTS;
	unsigned char* textureData = new unsigned char[ totalNumberOfColorComponents ];

	for( unsigned int i = 0; i < totalNumberOfColorComponents; i += NUMBER_OF_COLOR_COMPONENTS )
	{
		textureData[ i     ] = redComponentAsChar;
		textureData[ i + 1 ] = greenComponentAsChar;
		textureData[ i + 2 ] = blueComponentAsChar;
		textureData[ i + 3 ] = alphaComponentAsChar;
	}

	Renderer* renderer = Renderer::GetRenderer();

	static const unsigned int BYTE_ALIGNED = 1;
	renderer->SetTextureInputImageAlignment( BYTE_ALIGNED );

	renderer->GenerateTextureIDs( 1, &m_openGLTextureID );
	renderer->BindTexture( Renderer::TEXTURES_2D, this );

	renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::CLAMP_TO_EDGE );
	renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::LINEAR_INTERPOLATION );

	renderer->CreateTextureFrom2DImage( Renderer::TEXTURES_2D, 
		DEFAULT_LEVEL_OF_DETAIL,
		Renderer::RGBA,
		m_imageWidth,
		m_imageHeight,
		Renderer::RGBA,
		Renderer::UNSIGNED_BYTE,
		textureData );
}

//-----------------------------------------------------------------------------------------------
Texture::Texture( AttachmentType attachmentType, unsigned int framebufferXWidth, unsigned int framebufferYHeight )
	: m_imageWidth( framebufferXWidth )
	, m_imageHeight( framebufferYHeight )
	, m_fileLocation( "" )
{
	Renderer* renderer = Renderer::GetRenderer();

	renderer->GenerateTextureIDs( 1, &m_openGLTextureID );
	renderer->BindTexture( Renderer::TEXTURES_2D, this );

	static const unsigned int NO_MIPMAPS = 0;
	switch( attachmentType )
	{
	case ATTACHMENT_COLOR:
		renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::CLAMP_TO_EDGE );
		renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::LINEAR_INTERPOLATION );
		renderer->SetTextureMinificationMode( Renderer::TEXTURES_2D, Renderer::LINEAR_INTERPOLATION );

		renderer->CreateTextureFrom2DImage( Renderer::TEXTURES_2D, NO_MIPMAPS, Renderer::RGBA, 
											m_imageWidth, m_imageHeight, 
											Renderer::RGBA, Renderer::UNSIGNED_BYTE, NULL );
		break;


	case ATTACHMENT_DEPTH:
		renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::CLAMP_TO_EDGE );
		renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::NEAREST_NEIGHBOR );
		renderer->SetTextureMinificationMode( Renderer::TEXTURES_2D, Renderer::NEAREST_NEIGHBOR );

		renderer->CreateTextureFrom2DImage( Renderer::TEXTURES_2D, NO_MIPMAPS, Renderer::COMPONENTS_DEPTH, 
											m_imageWidth, m_imageHeight, 
											Renderer::COMPONENTS_DEPTH, Renderer::UNSIGNED_INTEGER, NULL );
		break;


	case ATTACHMENT_STENCIL:
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
Texture::Texture( FILE* textureFile, FilteringMethod filterMethod, WrappingMode wrapMode )
	: m_openGLTextureID( 0 )
	, m_imageWidth( 0 )
	, m_imageHeight( 0 )
{
	int numberOfColorComponents, requestedNumberOfColorComponents = 0;
	unsigned char* textureData = stbi_load_from_file( textureFile, &m_imageWidth, &m_imageHeight, 
											&numberOfColorComponents, requestedNumberOfColorComponents );
	// FIX: Handle error case where texture file does not load
	Renderer::ColorComponents bufferTextureComponentFormat;
	if( numberOfColorComponents == 4 )
		bufferTextureComponentFormat = Renderer::RGBA;
	else
		bufferTextureComponentFormat = Renderer::RGB;
	Renderer::ColorComponents cardTextureComponentFormat = bufferTextureComponentFormat;
	// FIX: This doesn't correctly handle exotic component formats

	Renderer* renderer = Renderer::GetRenderer();

	static const unsigned int BYTE_ALIGNED = 1;
	renderer->SetTextureInputImageAlignment( BYTE_ALIGNED );

	renderer->GenerateTextureIDs( 1, &m_openGLTextureID );
	renderer->BindTexture( Renderer::TEXTURES_2D, this );

	switch( wrapMode )
	{
	case clampToEdge:
		renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::CLAMP_TO_EDGE );
		break;
	case repeatOverGeometry:
		renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::REPEAT_OVER_GEOMETRY );
		break;
	default:
		return;
	}

	switch( filterMethod )
	{
	case nearestNeighbor:
		renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::NEAREST_NEIGHBOR );
		break;
	case linearInterpolation:
		renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::LINEAR_INTERPOLATION );
		break;
	default:
		return;
	}
	renderer->SetTextureMinificationMode( Renderer::TEXTURES_2D, Renderer::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE );

	renderer->CreateTextureFrom2DImage( Renderer::TEXTURES_2D, 
										DEFAULT_LEVEL_OF_DETAIL,
										cardTextureComponentFormat,
										m_imageWidth,
										m_imageHeight,
										bufferTextureComponentFormat,
										Renderer::UNSIGNED_BYTE,
										textureData );

	renderer->SetMaximumMipmapLevel( Renderer::TEXTURES_2D, 4 );
	renderer->SetMipmapQuality( Renderer::FASTEST );
	renderer->GenerateMipmaps( Renderer::TEXTURES_2D );

	stbi_image_free( textureData );
}

//-----------------------------------------------------------------------------------------------
Texture::Texture( nv_dds::CDDSImage* image, FilteringMethod filterMethod, WrappingMode wrapMode )
	: m_openGLTextureID( 0 )
	, m_imageWidth( image->get_width() )
	, m_imageHeight( image->get_height() )
{
	Renderer* renderer = Renderer::GetRenderer();

	renderer->GenerateTextureIDs( 1, &m_openGLTextureID );
	renderer->BindTexture( Renderer::TEXTURES_2D, this );

	switch( wrapMode )
	{
	case clampToEdge:
		renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::CLAMP_TO_EDGE );
		break;
	case repeatOverGeometry:
		renderer->SetTextureWrappingMode( Renderer::TEXTURES_2D, Renderer::REPEAT_OVER_GEOMETRY );
		break;
	default:
		return;
	}

	switch( filterMethod )
	{
	case nearestNeighbor:
		renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::NEAREST_NEIGHBOR );
		break;
	case linearInterpolation:
		renderer->SetTextureMagnificationMode( Renderer::TEXTURES_2D, Renderer::LINEAR_INTERPOLATION );
		break;
	default:
		return;
	}
	renderer->SetTextureMinificationMode( Renderer::TEXTURES_2D, Renderer::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE );

	renderer->SetMipmapQuality( Renderer::FASTEST );
	renderer->SetMaximumMipmapLevel( Renderer::TEXTURES_2D, image->get_num_mipmaps() );

	if( image->is_compressed() )
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, image->get_format(), 
			image->get_width(), image->get_height(), 0, image->get_size(), 
			*image);

		for (unsigned int i = 0; i < image->get_num_mipmaps(); i++)
		{
			nv_dds::CSurface mipmap = image->get_mipmap(i);

			glCompressedTexImage2D(GL_TEXTURE_2D, i+1, image->get_format(), 
				mipmap.get_width(), mipmap.get_height(), 0, mipmap.get_size(), 
				mipmap);
		}
	}
	else
	{	
		renderer->CreateTextureFrom2DImage( Renderer::TEXTURES_2D, 
			DEFAULT_LEVEL_OF_DETAIL,
			( Renderer::ColorComponents )image->get_components(),
			m_imageWidth,
			m_imageHeight,
			( Renderer::ColorComponents )image->get_format(),
			Renderer::UNSIGNED_BYTE,
			*image );

		for (unsigned int i = 0; i < image->get_num_mipmaps(); i++)
		{
			renderer->CreateTextureFrom2DImage( Renderer::TEXTURES_2D, 
				i+1,
				( Renderer::ColorComponents )image->get_components(),
				image->get_mipmap(i).get_width(),
				image->get_mipmap(i).get_height(),
				( Renderer::ColorComponents )image->get_format(),
				Renderer::UNSIGNED_BYTE,
				image->get_mipmap(i) );
		}
	}
}

//-----------------------------------------------------------------------------------------------
Texture::~Texture() { }

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateTextureOfSizeWithColor( unsigned int width, unsigned int height, const Color& color )
{
	return new Texture( width, height, color );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateColorTextureForFramebuffer( unsigned int windowXWidth, unsigned int windowYHeight )
{
	return new Texture( ATTACHMENT_COLOR, windowXWidth, windowYHeight );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateDepthTextureForFramebuffer( unsigned int windowXWidth, unsigned int windowYHeight )
{
	return new Texture( ATTACHMENT_DEPTH, windowXWidth, windowYHeight );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateOrGetTexture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode )
{
	std::map< std::string, Texture* >::iterator textureIterator = s_activeTextureRegistry.find( textureFileLocation );

	if( textureIterator == s_activeTextureRegistry.end() )
	{
		std::string textureFileExtension = GetFileExtensionForFilePath( textureFileLocation );
		Texture* texture;

		if( textureFileExtension.compare( "dds" ) == 0 )
		{
			nv_dds::CDDSImage image;
			image.load( textureFileLocation, false );

			texture = new Texture( &image, filterMethod, wrapMode );
		}
		else
		{
			FILE* textureFile;
			errno_t errorNumber = fopen_s( &textureFile, textureFileLocation.c_str(), "rb" );

			if( errorNumber != 0 )
			{
				exit( -16 );
			}

			texture = new Texture( textureFile, filterMethod, wrapMode );
		}

		s_activeTextureRegistry[ textureFileLocation ] = texture;
		return texture;
	}

	return textureIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateOrGetTexture( const std::wstring& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode )
{
	std::map< std::wstring, Texture* >::iterator textureIterator = s_activeTextureRegistryUnicode.find( textureFileLocation );

	if( textureIterator == s_activeTextureRegistryUnicode.end() )
	{
		std::wstring textureFileExtension = GetFileExtensionForFilePath( textureFileLocation );
		Texture* texture;

		if( textureFileExtension.compare( L"dds" ) == 0 )
		{
			nv_dds::CDDSImage image;
			image.load( textureFileLocation, false );

			texture = new Texture( &image, filterMethod, wrapMode );
		}
		else
		{
			FILE* textureFile;
			errno_t errorNumber = _wfopen_s( &textureFile, textureFileLocation.c_str(), L"rb" );

			if( errorNumber != 0 )
			{
				exit( -16 );
			}

			texture = new Texture( textureFile, filterMethod, wrapMode );
		}

		s_activeTextureRegistryUnicode[ textureFileLocation ] = texture;
		return texture;
	}

	return textureIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC void Texture::CleanUpTextureRepositories()
{
	Renderer* renderer = Renderer::GetRenderer();

	Texture* texture;
	for( std::map< std::string, Texture* >::iterator pair = s_activeTextureRegistry.begin();
													 pair != s_activeTextureRegistry.end();
													 ++pair )
	{
		texture = pair->second;
		renderer->DeleteTextureDataOnCard( texture );
		delete texture;
	}
	s_activeTextureRegistry.clear();

	for( std::map< std::wstring, Texture* >::iterator pair = s_activeTextureRegistryUnicode.begin();
		pair != s_activeTextureRegistryUnicode.end();
		++pair )
	{
		texture = pair->second;
		renderer->DeleteTextureDataOnCard( texture );
		delete texture;
	}
	s_activeTextureRegistryUnicode.clear();
}
