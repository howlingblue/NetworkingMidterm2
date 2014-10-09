#ifndef INCLUDED_RENDERER_HPP
#define INCLUDED_RENDERER_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <assert.h>
#include <stack>
#include "../Components/MaterialComponent.hpp"
#include "../Font/BitmapFont.hpp"
#include "../Math/EngineMath.hpp"
#include "../Math/FloatVector2.hpp"
#include "../Math/FloatVector3.hpp"
#include "../Math/FloatVector4.hpp"
#include "../Math/IntVector2.hpp"
#include "../Math/Float4x4Matrix.hpp"
#include "../Camera.hpp"
#include "../Color.hpp"
#include "Bone.hpp"
#include "Framebuffer.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "VertexDataContainers.hpp"

//-----------------------------------------------------------------------------------------------
ABSTRACT STATIC class Renderer
{
	static Renderer* s_renderer;


	virtual void Initialize() = 0;

protected:
	const ShaderProgram* s_activeShaderProgram;
	std::stack< Float4x4Matrix > m_matrixStack;
	Float4x4Matrix m_projectionMatrix;
	Float4x4Matrix m_viewMatrix;
	std::map< std::wstring, MaterialComponent > m_materials;

	static const unsigned int MAX_LIGHTS_IN_SHADER = 16;
	std::vector< Light > m_lights;

	static const unsigned int MAX_BONES_IN_SHADER = 250;

	Renderer()
		: m_projectionMatrix( F4X4_IDENTITY_MATRIX )
		, m_viewMatrix( F4X4_IDENTITY_MATRIX )
	{
		m_matrixStack.push( F4X4_IDENTITY_MATRIX );
	}

public:
	static enum DefaultAttributeLocation
	{
		LOCATION_Vertex = 0,
		LOCATION_Color = 1,
		LOCATION_TextureUV = 2,
		LOCATION_Normal = 3,
		LOCATION_Tangent = 4,
		LOCATION_Bitangent = 5,

		LOCATION_Custom1 = 6,
		LOCATION_Custom2 = 7,

		LOCATION_BoneIndex0 = 8,
		LOCATION_BoneWeight0 = 9,

		LOCATION_BoneIndex1 = 10,
		LOCATION_BoneWeight1 = 11,

		LOCATION_BoneIndex2 = 12,
		LOCATION_BoneWeight2 = 13,

		LOCATION_BoneIndex3 = 14,
		LOCATION_BoneWeight3 = 15
		//BE CAREFUL! Issues have occurred assigning locations > 15; they seem to be auto-assigned lower by OpenGL.
	};

	typedef unsigned short ArrayType;
	static const ArrayType COLOR_ARRAYS;
	static const ArrayType TEXTURE_COORD_ARRAYS;
	static const ArrayType VERTEX_ARRAYS;

	typedef unsigned short BufferType;
	static const BufferType ARRAY_BUFFER;
	static const BufferType INDEX_BUFFER;

	typedef unsigned short ColorComponents;
	static const ColorComponents COMPONENTS_DEPTH;
	static const ColorComponents COMPONENTS_STENCIL;
	static const ColorComponents RGB;
	static const ColorComponents RGBA;

	typedef unsigned short ColorBlendingMode;
	static const ColorBlendingMode NO_COLOR;
	static const ColorBlendingMode ONE_MINUS_DESTINATION_COLOR;
	static const ColorBlendingMode ONE_MINUS_SOURCE_ALPHA;
	static const ColorBlendingMode SOURCE_ALPHA;

	typedef unsigned short CoordinateType;
	static const CoordinateType SHORT_TYPE;
	static const CoordinateType INTEGER_TYPE;
	static const CoordinateType FLOAT_TYPE;
	static const CoordinateType DOUBLE_TYPE;
	static const CoordinateType UNSIGNED_BYTE;
	static const CoordinateType UNSIGNED_INTEGER;
	static const CoordinateType UNSIGNED_SHORT_TYPE;

	typedef unsigned short Feature;
	static const Feature COLOR_BLENDING;
	static const Feature DEPTH_TESTING;
	static const Feature FACE_CULLING;
	static const Feature SHAPE_RESTART_INDEXING;
	static const Feature TEXTURES_2D;

	typedef unsigned int QualityLevel;
	static const QualityLevel FASTEST;

	typedef unsigned short Shader;
	static const Shader GEOMETRY_SHADER;
	static const Shader PIXEL_FRAGMENT_SHADER;
	static const Shader VERTEX_SHADER;

	typedef unsigned short Shape;
	static const Shape POINTS;
	static const Shape LINES;
	static const Shape LINE_LOOP;
	static const Shape LINE_STRIP;
	static const Shape TRIANGLES;
	static const Shape TRIANGLE_STRIP;
	static const Shape QUADS;

	typedef unsigned short TextureFilteringMethod;
	static const TextureFilteringMethod NEAREST_NEIGHBOR;
	static const TextureFilteringMethod LINEAR_INTERPOLATION;
	static const TextureFilteringMethod NEAREST_MIPMAP_NEAREST_TEXTURE;
	static const TextureFilteringMethod NEAREST_MIPMAP_INTERPOLATE_TEXTURES;
	static const TextureFilteringMethod INTERPOLATE_MIPMAPS_NEAREST_TEXTURE;
	static const TextureFilteringMethod INTERPOLATE_MIPMAPS_INTERPOLATE_TEXTURES;

	typedef unsigned short TextureWrapMode;
	static const TextureWrapMode CLAMP_TO_EDGE;
	static const TextureWrapMode REPEAT_OVER_GEOMETRY;

	//Instantiation
	static void CreateRenderer();
	static Renderer* GetRenderer();

	//FIX: do something else with this
	VertexColorTextureData2D* BuildGlyphArrayFromString( unsigned int& out_totalGlyphVertices, const std::string& textString, const BitmapFont& font, 
														 float cellHeight, const FloatVector2& textStartLowerLeftCorner, const Color& textColor );

	//Feature Enabling
	virtual void EnableArrayType( ArrayType type ) const = 0;
	virtual void DisableArrayType( ArrayType type ) const = 0;
	virtual void BindVertexArraysToAttributeLocation( unsigned int location ) const = 0;
	virtual void UnbindVertexArraysFromAttributeLocation( unsigned int location ) const = 0;

	virtual void EnableFeature( Feature feature ) const = 0;
	virtual void DisableFeature( Feature feature ) const = 0;

	//Color and Depth Buffers
	virtual void ClearColorBuffer() const = 0;
	virtual void ClearDepthBuffer() const = 0;
	virtual void SetColorBufferClearValue( float red, float green, float blue, float alpha ) = 0;
	virtual void SetDepthBufferClearValue( float depthBetweenZeroAndOne ) = 0;
	virtual void DisableDepthBufferWriting() const = 0;
	virtual void EnableDepthBufferWriting() const = 0;

	//Frame Buffers
	virtual void AttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot ) = 0;
	virtual void AttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer ) = 0;
	virtual void AttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer ) = 0;
	virtual bool CheckIfFramebufferIsReady( const Framebuffer& framebuffer ) = 0;
	virtual void ClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot ) = 0;
	virtual void ClearFramebufferDepthOutput( Framebuffer& framebuffer ) = 0;
	virtual void ClearFramebufferStencilOutput( Framebuffer& framebuffer ) = 0;
	virtual Framebuffer CreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting ) = 0;
	virtual void DeleteFramebuffer( Framebuffer& framebuffer ) = 0;
	virtual void UseDefaultFramebuffer() = 0;
	virtual void UseFrameBuffer( const Framebuffer& framebuffer ) = 0;

	//Matrix Operations
	Float4x4Matrix& GetModelMatrix() { return m_matrixStack.top(); }
	const Float4x4Matrix& GetModelMatrix() const { return m_matrixStack.top(); }
	const Float4x4Matrix& GetViewMatrix() const { return m_viewMatrix; }
	const Float4x4Matrix& GetProjectionMatrix() const { return m_projectionMatrix; }
	void PopMatrix();
	void PushMatrix();
	void RotateWorldAboutAxisDegrees( const FloatVector3& axis, float angleDegrees );
	void SetOrthographicProjection( double leftXCoord, double rightXCoord, double bottomYCoord, double topYCoord, double nearClippingPlaneDistance, double farClippingPlaneDistance );
	void SetPerpectiveProjection( double horizontalFOVDegrees, double aspectRatio, double nearClippingPlaneDistance, double farClippingPlaneDistance );
	void SetTopOfStackToIdentity();
	void SetTopOfStackToMatrix( const Float4x4Matrix& matrix );
	void SetViewMatrixFromCamera( const Camera& camera );
	void SetViewMatrixToIdentity() { m_viewMatrix = F4X4_IDENTITY_MATRIX; }
	void TranslateWorld( const FloatVector3& translationDirection );

	//Draw modification
	virtual void SetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor ) const = 0;
	virtual void SetColor( float red, float green, float blue, float alpha ) const = 0;
	virtual void SetLineWidth( float widthPixels ) const = 0;
	virtual void SetPointSize( float pointSize ) const = 0;

	//Materials
	MaterialComponent* CreateOrGetNewMaterialComponent( const std::wstring& materialName );
	void ApplyMaterialComponent( const MaterialComponent* material );
	void RemoveMaterialComponent( const MaterialComponent* material );
	
	//Bones
	void UpdateSkeletonOnMaterial( const Float4x4Matrix& objectStartingTransform, const std::vector< Bone >& skeleton, MaterialComponent* material ) const;

	//Lights
	void AddLight( const Light& light ) { m_lights.push_back( light ); }
	void UpdateLightsOnMaterial( MaterialComponent* material ) const;

	//Mipmaps
	virtual void GenerateMipmaps( Feature textureType ) = 0;
	virtual void SetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel ) = 0;
	virtual void SetMipmapQuality( QualityLevel qualityLevel ) = 0;

	//Shaders
	virtual int CompileShader( Shader shaderType, const char* sourceString, std::string& out_errors ) = 0;
	virtual int CreateProgramFromShaders( int vertexShaderID, int geometryShaderID, int pixelFragmentShaderID, std::string& out_errors ) = 0;
	virtual void DeleteProgramDataOnCard( ShaderProgram* program ) = 0;
	virtual void DeleteShaderDataOnCard( int shaderID ) = 0;
	virtual void DetachShaderFromProgram( int shaderID, int programID ) = 0;
	const ShaderProgram* GetActiveShaderProgram() const { return s_activeShaderProgram; }
	virtual int GetAttributeLocation( const ShaderProgram* program, const std::string& attributeName ) = 0;
	virtual int GetNumberOfAttributesInProgram( const ShaderProgram* program ) const = 0;
	virtual int GetNumberOfUniformsInProgram( const ShaderProgram* program ) const = 0;
	virtual void GetShaderAttributeName( const ShaderProgram* program, unsigned int attributeIndex, std::string& out_attributeName ) = 0;
	virtual void GetShaderUniformName( const ShaderProgram* program, unsigned int uniformIndex, std::string& out_uniformName ) = 0;
	virtual int GetUniformVariableLocation( const ShaderProgram* program, const std::string& constantName ) = 0;
	virtual void SetAttributeLocation( const ShaderProgram* program, const std::string& constantName, int locationID ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, int setting ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, float setting ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, const Color& color ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, const IntVector2& vector2 ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, const FloatVector2& vector2 ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, const FloatVector3& vector3 ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, const FloatVector4& vector4 ) = 0;
	virtual void SetUniformVariable( int programConstantLocation, const Float4x4Matrix& matrix ) = 0;
	virtual void UseShaderProgram( const ShaderProgram* program ) = 0;

	//Text Rendering
	float CalculateTextWidth( const std::string& textString, const BitmapFont& font, float textHeight );
	float CalculateTextWidthFrom( const std::string& textString, const BitmapFont& font, float textHeight, unsigned int startIndex, unsigned int endIndex );
	void Render2DText( const std::string& textString, const BitmapFont& font, float cellHeight, const FloatVector2& textStartLowerLeftCorner,
					 const Color& textColor = Color( 1.f, 1.f, 1.f, 1.f ), const Color& backgroundColor = Color( 0.f, 0.f, 0.f, 0.f ), const Color& textShadowColor = Color( 0.f, 0.f, 0.f, 0.f ) );

	//Textures
	virtual void BindTexture( Feature textureType, const Texture* texture ) const = 0;
	virtual void CreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
										   unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
										   CoordinateType pixelDataType, const void* imageData ) = 0;
	virtual void DeleteTextureDataOnCard( Texture* texture ) = 0;
	virtual void GenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs ) = 0;
	virtual void SetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight ) = 0;
	virtual void SetActiveTextureUnit( unsigned int textureUnitNumber ) = 0;
	virtual void SetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod ) = 0;
	virtual void SetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod ) = 0;
	virtual void SetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode ) = 0;

	//Vertex/Index Arrays
	virtual void RenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender ) const = 0;
	virtual void RenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray ) const = 0;
	virtual void SetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void SetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void SetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void SetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void SetShapeRestartIndex( unsigned int index ) = 0;

	//Vertex Buffer Objects
	virtual void BindBufferObject( BufferType bufferType, unsigned int bufferID ) = 0;
	virtual void DeleteBufferObject( unsigned int bufferID ) = 0;
	virtual void GenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs ) = 0;
	virtual void SendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer ) = 0;
};

STATIC inline Renderer* Renderer::GetRenderer() 
{ 
	if ( s_renderer == nullptr )
		exit( -1 );
	return s_renderer; 
}

#pragma region +++++Renderer Matrix Operations+++++
//-----------------------------------------------------------------------------------------------
inline void Renderer::PopMatrix()
{
	assert( !m_matrixStack.empty() );
	m_matrixStack.pop();
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::PushMatrix()
{
	m_matrixStack.push( m_matrixStack.top() );
}

//-----------------------------------------------------------------------------------------------
inline void GetRotationMatrixForAxisAndAngleDegrees( Float4x4Matrix& out_rotationMatrix, const FloatVector3& axis, float angleDegrees )
{
	float angleRadians = ConvertDegreesToRadians( angleDegrees );
	float cosineAngle = cos( angleRadians );
	float oneMinusCosineAngle = 1.f - cosineAngle;
	float sineAngle = sin( angleRadians );

	//Using the conversion from the Wikipedia rotation matrix page:
	//http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	out_rotationMatrix( 1, 1 ) = cosineAngle + axis.x * axis.x * oneMinusCosineAngle;
	out_rotationMatrix( 2, 1 ) = axis.x * axis.y * oneMinusCosineAngle - axis.z * sineAngle;
	out_rotationMatrix( 3, 1 ) = axis.x * axis.z * oneMinusCosineAngle + axis.y * sineAngle;
	out_rotationMatrix( 1, 2 ) = axis.y * axis.x * oneMinusCosineAngle + axis.z * sineAngle;
	out_rotationMatrix( 2, 2 ) = cosineAngle + axis.y * axis.y * oneMinusCosineAngle;
	out_rotationMatrix( 3, 2 ) = axis.y * axis.z * oneMinusCosineAngle - axis.x * sineAngle;
	out_rotationMatrix( 1, 3 ) = axis.z * axis.x * oneMinusCosineAngle - axis.y * sineAngle;
	out_rotationMatrix( 2, 3 ) = axis.z * axis.y * oneMinusCosineAngle + axis.x * sineAngle;
	out_rotationMatrix( 3, 3 ) = cosineAngle + axis.z * axis.z * oneMinusCosineAngle;
	out_rotationMatrix( 4, 4 ) = 1.f;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::RotateWorldAboutAxisDegrees( const FloatVector3& axis, float angleDegrees )
{
	Float4x4Matrix& currentMatrix = GetModelMatrix();

	Float4x4Matrix rotationMatrix = F4X4_IDENTITY_MATRIX;
	GetRotationMatrixForAxisAndAngleDegrees( rotationMatrix, axis, angleDegrees );

	currentMatrix = rotationMatrix * currentMatrix;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::SetOrthographicProjection( double leftXCoord, double rightXCoord, double bottomYCoord, double topYCoord, double nearClippingPlaneDistance, double farClippingPlaneDistance )
{
	float inverseScreenWidth = 1.f / static_cast< float >( rightXCoord - leftXCoord );
	float inverseScreenHeight = 1.f / static_cast< float >( topYCoord - bottomYCoord );
	float inverseFrustumDepth = 1.f / static_cast< float >( farClippingPlaneDistance - nearClippingPlaneDistance );

	Float4x4Matrix orthographicMatrix = F4X4_IDENTITY_MATRIX;
	orthographicMatrix( 1, 1 ) =  2.f * inverseScreenWidth;
	orthographicMatrix( 2, 2 ) =  2.f * inverseScreenHeight;
	orthographicMatrix( 3, 3 ) = -2.f * inverseFrustumDepth;
	orthographicMatrix( 4, 1 ) = -static_cast< float >( leftXCoord + rightXCoord ) * inverseScreenWidth;
	orthographicMatrix( 4, 2 ) = -static_cast< float >( topYCoord + bottomYCoord ) * inverseScreenHeight;
	orthographicMatrix( 4, 3 ) = -static_cast< float >( farClippingPlaneDistance + nearClippingPlaneDistance ) * inverseFrustumDepth;

	m_projectionMatrix = orthographicMatrix;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::SetPerpectiveProjection( double horizontalFOVDegrees, double aspectRatio, double nearClippingPlaneDistance, double farClippingPlaneDistance )
{
	float perspectiveScalingFactor = 1.f / tan( ConvertDegreesToRadians( static_cast< float >( horizontalFOVDegrees ) ) * 0.5f );
	float inverseFrustumDepth = 1.f / static_cast< float >( nearClippingPlaneDistance - farClippingPlaneDistance );

	Float4x4Matrix perspectiveMatrix = F4X4_IDENTITY_MATRIX;
	perspectiveMatrix( 1, 1 ) = perspectiveScalingFactor;
	perspectiveMatrix( 2, 2 ) = perspectiveScalingFactor * static_cast< float >( aspectRatio );
	perspectiveMatrix( 3, 3 ) = static_cast< float >( farClippingPlaneDistance + nearClippingPlaneDistance ) * inverseFrustumDepth;
	perspectiveMatrix( 3, 4 ) = -1.f;
	perspectiveMatrix( 4, 3 ) = static_cast< float >( 2.f * farClippingPlaneDistance * nearClippingPlaneDistance ) * inverseFrustumDepth;
	perspectiveMatrix( 4, 4 ) = 0.f;

	m_projectionMatrix = perspectiveMatrix;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::SetTopOfStackToIdentity()
{
	m_matrixStack.top() = F4X4_IDENTITY_MATRIX;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::SetTopOfStackToMatrix( const Float4x4Matrix& matrix )
{
	m_matrixStack.top() = matrix;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::SetViewMatrixFromCamera( const Camera& camera )
{
 	Float4x4Matrix rotationMatrix = F4X4_IDENTITY_MATRIX;
	const EulerAngles& cameraOrientation = camera.GetHeading();
	Float4x4Matrix xRotation, yRotation, zRotation, x2, z2; 
	GetRotationMatrixForAxisAndAngleDegrees( xRotation, FloatVector3( 1.f, 0.f, 0.f ), -cameraOrientation.rollDegreesAboutX );
	GetRotationMatrixForAxisAndAngleDegrees( yRotation, FloatVector3( 0.f, 1.f, 0.f ), -cameraOrientation.pitchDegreesAboutY );
	GetRotationMatrixForAxisAndAngleDegrees( zRotation, FloatVector3( 0.f, 0.f, 1.f ), -cameraOrientation.yawDegreesAboutZ );
	GetRotationMatrixForAxisAndAngleDegrees( x2, FloatVector3( 1.f, 0.f, 0.f ), -90.f );
	GetRotationMatrixForAxisAndAngleDegrees( z2, FloatVector3( 0.f, 0.f, 1.f ),  90.f );
	rotationMatrix = zRotation * yRotation * xRotation * z2 * x2;
 
	Float4x4Matrix translationMatrix = F4X4_IDENTITY_MATRIX;
	const FloatVector3& cameraPosition = camera.GetPosition();
	translationMatrix[ 12 ] = -cameraPosition.x;
	translationMatrix[ 13 ] = -cameraPosition.y;
	translationMatrix[ 14 ] = -cameraPosition.z;
	translationMatrix[ 15 ] = 1.f;

 	m_viewMatrix = translationMatrix * rotationMatrix;
}

//-----------------------------------------------------------------------------------------------
inline void Renderer::TranslateWorld( const FloatVector3& translationDirection )
{
	Float4x4Matrix& currentMatrix = GetModelMatrix();

	Float4x4Matrix translationMatrix = F4X4_IDENTITY_MATRIX;
	translationMatrix[ 12 ] = translationDirection.x;
	translationMatrix[ 13 ] = translationDirection.y;
	translationMatrix[ 14 ] = translationDirection.z;
	translationMatrix[ 15 ] = 1.f;

	currentMatrix = translationMatrix * currentMatrix;
}
#pragma endregion

#endif //INCLUDED_RENDERER_HPP
