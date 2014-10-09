#pragma once
#ifndef INCLUDED_MATERIAL_COMPONENT_HPP
#define INCLUDED_MATERIAL_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>
#include "../Graphics/Tendon.hpp"
#include "../Graphics/Light.hpp"
#include "../Graphics/ShaderProgram.hpp"
#include "../Graphics/Texture.hpp"
#include "../Math/Float4x4Matrix.hpp"

//-----------------------------------------------------------------------------------------------
struct MaterialComponent
{
	static const unsigned short MAX_NUMBER_OF_TEXTURES = 4;
	typedef unsigned short TextureType;
	static const TextureType TEXTURE_DiffuseMap  = 0;
	static const TextureType TEXTURE_NormalMap   = 1;
	static const TextureType TEXTURE_SpecularMap = 2;
	static const TextureType TEXTURE_EmissiveMap = 3;

	struct TextureInfo
	{
		int textureUnitID;
		int samplerUniformID;
		const Texture* texture;
	};

	//Constructor
	MaterialComponent();

	int GetValidAttributeIDFromNameOrDie( const std::string& attributeName ) const;
	int GetValidUniformIDFromNameOrDie( const std::string& uniformName ) const;

	void SetFloatUniform( const std::string& uniformName, float value );
	void SetFloatUniform( const std::string& uniformName, const FloatVector3& vector );
	void SetFloatUniform( const std::string& uniformName, const FloatVector4& vector );
	void SetIntegerUniform( const std::string& uniformName, int value );
	void SetLightUniform( const std::string& uniformName, const Light& light );
	void SetMatrixUniform( const std::string& uniformName, const Float4x4Matrix& matrix );
	void SetModelMatrixUniform( const std::string& uniformName );
	void SetViewMatrixUniform( const std::string& uniformName );
	void SetProjectionMatrixUniform( const std::string& uniformName );
	void SetLineWidth( float newLineWidth ) { lineWidth = newLineWidth; }
	void SetShaderProgram( const ShaderProgram* shaderProgram ) { program = shaderProgram; }
	void SetTextureUniform( const std::string& uniformName, int textureUnitID, const Texture* texture );
	void SetTextureUniform( const std::string& uniformName, int textureUnitID, const std::string& textureFileLocation, 
							Texture::FilteringMethod filteringMethod, Texture::WrappingMode wrappingMode );

	//Data Members
	const ShaderProgram* program;
	int modelMatrixUniformLocation;
	int viewMatrixUniformLocation;
	int projectionMatrixUniformLocation;
	std::vector< TextureInfo > infoForTextures;
	float lineWidth;
};



//-----------------------------------------------------------------------------------------------
inline MaterialComponent::MaterialComponent()
	: program( nullptr )
	, modelMatrixUniformLocation( -1 )
	, viewMatrixUniformLocation( -1 )
	, projectionMatrixUniformLocation( -1 )
	, lineWidth( 1 )
{ }

//-----------------------------------------------------------------------------------------------
inline int MaterialComponent::GetValidAttributeIDFromNameOrDie( const std::string& attributeName ) const
{
	int attributeID = program->GetAttributeIDFromName( attributeName );
	assert( attributeID != -1 );
	return attributeID;
}

//-----------------------------------------------------------------------------------------------
inline int MaterialComponent::GetValidUniformIDFromNameOrDie( const std::string& uniformName ) const
{
	int uniformID = program->GetUniformVariableIDFromName( uniformName );
	assert( uniformID != -1 );
	return uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void MaterialComponent::SetModelMatrixUniform( const std::string& uniformName )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	modelMatrixUniformLocation = uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void MaterialComponent::SetViewMatrixUniform( const std::string& uniformName )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	viewMatrixUniformLocation = uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void MaterialComponent::SetProjectionMatrixUniform( const std::string& uniformName )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	projectionMatrixUniformLocation = uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void MaterialComponent::SetTextureUniform( const std::string& uniformName, int textureUnitID, const Texture* texture )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	TextureInfo texInfo;
	texInfo.textureUnitID = textureUnitID;
	texInfo.texture = texture;
	texInfo.samplerUniformID = uniformID;
	infoForTextures.push_back( texInfo );
}

#endif //INCLUDED_MATERIAL_COMPONENT_HPP
