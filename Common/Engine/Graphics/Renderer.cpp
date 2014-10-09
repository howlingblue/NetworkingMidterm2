#include "Renderer.hpp"
#include "OpenGLRenderer.hpp"
//#include "../BuildPreferences.hpp"
#include "../Components/MaterialComponent.hpp"

//-----------------------------------------------------------------------------------------------
STATIC Renderer* Renderer::s_renderer = nullptr;

//-----------------------------------------------------------------------------------------------
VertexColorTextureData2D* Renderer::BuildGlyphArrayFromString( unsigned int& out_totalGlyphVertices, const std::string& textString, const BitmapFont& font, 
																float cellHeight, const FloatVector2& textStartLowerLeftCorner, const Color& textColor )
{
	static const unsigned int VERTICES_PER_GLYPH = 6; // four vertices for the glyph, two for sewing strips together.
	out_totalGlyphVertices = VERTICES_PER_GLYPH * textString.length() - 2; //The last two strip sewing vertices are unneeded.
	VertexColorTextureData2D* glyphVertexArray = new VertexColorTextureData2D[ out_totalGlyphVertices ];

	float minY = textStartLowerLeftCorner.y;
	float maxY = minY + cellHeight;
	float startX = textStartLowerLeftCorner.x;
	float endX = startX;

	for( unsigned int i = 0; i < textString.length(); ++i )
	{
		const Glyph& glyph = font.GetGlyphForCharacter( textString[ i ] );

		startX = endX + glyph.m_advanceBeforeDrawA * cellHeight;
		endX = startX + glyph.m_fontWidthB * cellHeight;

		glyphVertexArray[ VERTICES_PER_GLYPH * i + 0 ] = VertexColorTextureData2D( FloatVector2( startX, minY ), textColor, FloatVector2( glyph.m_textureUVMins.x, glyph.m_textureUVMaxs.y ) );
		glyphVertexArray[ VERTICES_PER_GLYPH * i + 1 ] = VertexColorTextureData2D( FloatVector2( endX,   minY ), textColor, glyph.m_textureUVMaxs );
		glyphVertexArray[ VERTICES_PER_GLYPH * i + 2 ] = VertexColorTextureData2D( FloatVector2( startX, maxY ), textColor, glyph.m_textureUVMins );
		glyphVertexArray[ VERTICES_PER_GLYPH * i + 3 ] = VertexColorTextureData2D( FloatVector2( endX,   maxY ), textColor, FloatVector2( glyph.m_textureUVMaxs.x, glyph.m_textureUVMins.y ) );

		endX += glyph.m_advanceAfterDrawC * cellHeight;

		//Create four degenerate triangles to sew the triangle strips together.
		//On the last loop, we can't look ahead, so just don't do it at all (and save four vertices in the bargain.)
		if( i + 1 != textString.length() )
		{
			glyphVertexArray[ VERTICES_PER_GLYPH * i + 4 ] = glyphVertexArray[ VERTICES_PER_GLYPH * i + 3 ];

			const Glyph& nextGlyph = font.GetGlyphForCharacter( textString[ i + 1 ] );
			glyphVertexArray[ VERTICES_PER_GLYPH * i + 5 ] = VertexColorTextureData2D( FloatVector2( endX + nextGlyph.m_advanceBeforeDrawA * cellHeight,   minY ), textColor, FloatVector2( 0.f, 0.f ) );
		}
	}

	return glyphVertexArray;
}

//-----------------------------------------------------------------------------------------------
STATIC void Renderer::CreateRenderer()
{
	//#ifdef USE_OPENGL_RENDERING_INTERFACE
		s_renderer = new OpenGLRenderer();
		s_renderer->Initialize();

		if( s_renderer == nullptr )
			exit( -1 );
	//#endif
}

//-----------------------------------------------------------------------------------------------
void Renderer::UpdateSkeletonOnMaterial( const Float4x4Matrix& /*objectStartingTransform*/, const std::vector< Bone >& /*skeleton*/, MaterialComponent* /*material*/ ) const
{
// 	Float4x4Matrix vertexLocalToSkinnedWorldTransform;
// 	unsigned int numberOfBonesInSkeleton = skeleton.size();
// 	for( unsigned int i = 0; i < numberOfBonesInSkeleton; ++i )
// 	{
// 		vertexLocalToSkinnedWorldTransform = objectStartingTransform * skeleton[ i ].inverseRestTransform * skeleton[ i ].transformPointer->GetMatrix();
// 
// 		std::string boneUniformName( "u_boneTransformationMatrices[" + std::to_string( long long( i ) ) + "]" );
// 		material->SetMatrixUniform( boneUniformName, vertexLocalToSkinnedWorldTransform );
// 	}
}

//-----------------------------------------------------------------------------------------------
void Renderer::UpdateLightsOnMaterial( MaterialComponent* material ) const
{
	unsigned int numberOfLights = m_lights.size();
	for( unsigned int i = 0; i < numberOfLights; ++i )
	{
		std::string lightUniformName( "u_lights[" + std::to_string( long long( i ) ) + "]" );
		material->SetLightUniform( lightUniformName, m_lights[ i ] );
	}

	Light defaultLight;
	for( unsigned int i = numberOfLights; i < MAX_LIGHTS_IN_SHADER; ++i )
	{
		std::string lightUniformName( "u_lights[" + std::to_string( long long( i ) ) + "]" );
		material->SetLightUniform( lightUniformName, defaultLight );
	}
}

//-----------------------------------------------------------------------------------------------
float Renderer::CalculateTextWidth( const std::string& textString, const BitmapFont& font, float textHeight )
{
	return CalculateTextWidthFrom( textString, font, textHeight, 0, textString.length() );
}

//-----------------------------------------------------------------------------------------------
float Renderer::CalculateTextWidthFrom( const std::string& textString, const BitmapFont& font, float textHeight, unsigned int startIndex, unsigned int endIndex )
{
	float textWidth = 0.f;

	if( startIndex > textString.length() || endIndex > textString.length() )
		return textWidth;

	for( unsigned int i = startIndex; i < endIndex; ++i )
	{
		textWidth += font.GetWidthOfCharacter( textString[ i ], textHeight );
	}
	return textWidth;
}

//-----------------------------------------------------------------------------------------------
void RenderTextGlyphs( const Renderer* renderer, const VertexColorTextureData2D* vertexData, unsigned int numberOfVertices )
{
	static const int NUMBER_OF_VERTEX_COORDINATES = 2;
	static const int SIZE_OF_ARRAY_STRUCTURE = sizeof( VertexColorTextureData2D );
	renderer->SetPointerToVertexArray( NUMBER_OF_VERTEX_COORDINATES, Renderer::FLOAT_TYPE, SIZE_OF_ARRAY_STRUCTURE, &vertexData[ 0 ] );

	static const int NUMBER_OF_COLOR_COORDINATES = 4;
	renderer->SetPointerToColorArray( NUMBER_OF_COLOR_COORDINATES, Renderer::FLOAT_TYPE, SIZE_OF_ARRAY_STRUCTURE, &vertexData[ 0 ].red );

	static const int NUMBER_OF_TEXTURE_COORDINATES = 2;
	renderer->SetPointerToTextureCoordinateArray( NUMBER_OF_TEXTURE_COORDINATES, Renderer::FLOAT_TYPE, SIZE_OF_ARRAY_STRUCTURE, &vertexData[ 0 ].u );

	static const int VERTEX_ARRAY_START = 0;
	renderer->RenderVertexArray( Renderer::TRIANGLE_STRIP, 0, numberOfVertices );
}

//-----------------------------------------------------------------------------------------------
void RenderTextBackground( const Renderer* renderer, const VertexColorData2D* vertexData, unsigned int numberOfVertices )
{
	static const int SIZE_OF_QUAD_ARRAY_STRUCTURE = sizeof( VertexColorData2D );
	static const int NUMBER_OF_VERTEX_COORDINATES = 2;
	renderer->SetPointerToVertexArray( NUMBER_OF_VERTEX_COORDINATES, Renderer::FLOAT_TYPE, SIZE_OF_QUAD_ARRAY_STRUCTURE, &vertexData[ 0 ] );

	static const int NUMBER_OF_COLOR_COORDINATES = 4;
	renderer->SetPointerToColorArray( NUMBER_OF_COLOR_COORDINATES, Renderer::FLOAT_TYPE, SIZE_OF_QUAD_ARRAY_STRUCTURE, &vertexData[ 0 ].color.r );
	renderer->RenderVertexArray( Renderer::TRIANGLE_STRIP, 0, numberOfVertices );
}

//-----------------------------------------------------------------------------------------------
void Renderer::Render2DText( const std::string& textString, const BitmapFont& font, float cellHeight, const FloatVector2& textStartLowerLeftCorner,
							const Color& textColor, const Color& backgroundColor, const Color& textShadowColor )
{
	unsigned int totalGlyphVertices = 0;
	VertexColorTextureData2D* textQuadVertices = BuildGlyphArrayFromString( totalGlyphVertices, textString, font, cellHeight, textStartLowerLeftCorner, textShadowColor );


	static const unsigned int NUMBER_OF_BACKGROUND_QUAD_VERTICES = 4;
	float widthOfGlyphedText = s_renderer->CalculateTextWidth( textString, font, cellHeight );
	VertexColorData2D backgroundQuad[ NUMBER_OF_BACKGROUND_QUAD_VERTICES ] = 
	{
		VertexColorData2D( FloatVector2( textStartLowerLeftCorner.x,						textStartLowerLeftCorner.y ),				backgroundColor ),
		VertexColorData2D( FloatVector2( textStartLowerLeftCorner.x + widthOfGlyphedText,   textStartLowerLeftCorner.y ),				backgroundColor ),
		VertexColorData2D( FloatVector2( textStartLowerLeftCorner.x,						textStartLowerLeftCorner.y + cellHeight ),	backgroundColor ),
		VertexColorData2D( FloatVector2( textStartLowerLeftCorner.x + widthOfGlyphedText,   textStartLowerLeftCorner.y + cellHeight ),	backgroundColor )
	};
	s_renderer->PushMatrix();

	s_renderer->EnableArrayType( Renderer::VERTEX_ARRAYS );
	s_renderer->EnableArrayType( Renderer::COLOR_ARRAYS );



	const ShaderProgram* activeShader = GetActiveShaderProgram();
	//unsigned int shaderMapID = s_renderer->GetUniformVariableLocation( activeShader, "u_diffuseMap" );
	//s_renderer->SetUniformVariable( shaderMapID, 0 );

	RenderTextBackground( s_renderer, backgroundQuad, NUMBER_OF_BACKGROUND_QUAD_VERTICES );


	s_renderer->EnableArrayType( Renderer::TEXTURE_COORD_ARRAYS );

	//s_renderer->BindTexture( Renderer::TEXTURES_2D, font.GetTextureSheet( 0 ) );

	float shadowOffset = .05f * cellHeight;
	s_renderer->TranslateWorld( FloatVector3( shadowOffset, -shadowOffset, 0.f ) );
	RenderTextGlyphs( s_renderer, textQuadVertices, totalGlyphVertices );
	for( unsigned int i = 0; i < totalGlyphVertices; ++i )
	{
		textQuadVertices[ i ].SetColor( textColor );
	}
	s_renderer->TranslateWorld( FloatVector3( -shadowOffset, shadowOffset, 0.f ) );
	RenderTextGlyphs( s_renderer, textQuadVertices, totalGlyphVertices );

	s_renderer->DisableArrayType( Renderer::TEXTURE_COORD_ARRAYS );
	s_renderer->DisableArrayType( Renderer::COLOR_ARRAYS );
	s_renderer->DisableArrayType( Renderer::VERTEX_ARRAYS );

	s_renderer->PopMatrix();

	delete[] textQuadVertices;
}

//-----------------------------------------------------------------------------------------------
MaterialComponent* Renderer::CreateOrGetNewMaterialComponent( const std::wstring& materialName )
{
	return &m_materials[ materialName ];
}

//-----------------------------------------------------------------------------------------------
void Renderer::ApplyMaterialComponent( const MaterialComponent* material )
{
	UseShaderProgram( material->program );

	SetUniformVariable( material->modelMatrixUniformLocation, GetModelMatrix() );
	SetUniformVariable( material->viewMatrixUniformLocation, GetViewMatrix() );
	SetUniformVariable( material->projectionMatrixUniformLocation, GetProjectionMatrix() );

	for( unsigned int i = 0; i < material->infoForTextures.size(); ++i )
	{
		const MaterialComponent::TextureInfo& texInfo = material->infoForTextures[ i ];
		SetActiveTextureUnit( texInfo.textureUnitID );
		BindTexture( Renderer::TEXTURES_2D, texInfo.texture );
		SetUniformVariable( texInfo.samplerUniformID, texInfo.textureUnitID );
	}

	SetLineWidth( material->lineWidth );
}

//-----------------------------------------------------------------------------------------------
void Renderer::RemoveMaterialComponent( const MaterialComponent* material )
{
	for( unsigned int i = 0; i < material->infoForTextures.size(); ++i )
	{
		const MaterialComponent::TextureInfo& texInfo = material->infoForTextures[ i ];
		SetActiveTextureUnit( texInfo.textureUnitID );
		DisableFeature( Renderer::TEXTURES_2D );
	}

	SetLineWidth( 1 );
}
