#include "MaterialComponent.hpp"
#include "../Graphics/Renderer.hpp"

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetFloatUniform( const std::string& uniformName, float value )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();
	renderer->SetUniformVariable( uniformID, value );
}

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetFloatUniform( const std::string& uniformName, const FloatVector3& vector )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();
	renderer->SetUniformVariable( uniformID, vector );
}

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetFloatUniform( const std::string& uniformName, const FloatVector4& vector )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();
	renderer->SetUniformVariable( uniformID, vector );
}

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetIntegerUniform( const std::string& uniformName, int value )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();
	renderer->SetUniformVariable( uniformID, value );
}

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetLightUniform( const std::string& uniformName, const Light& light )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".position" );
	renderer->SetUniformVariable( uniformID, light.m_position );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".directionInWorld" );
	renderer->SetUniformVariable( uniformID, light.m_direction );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".colorAndBrightness" );
	renderer->SetUniformVariable( uniformID, light.m_colorAndBrightness );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".outerRadiusOfZeroIntensity" );
	renderer->SetUniformVariable( uniformID, light.m_outerRadiusOfZeroIntensity );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".inverseSizeOfDistanceAttenuationZone" );
	renderer->SetUniformVariable( uniformID, light.m_inverseSizeOfDistanceAttenuationZone );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".outerApertureAngleAsDotProduct" );
	renderer->SetUniformVariable( uniformID, light.m_outerApertureAngleAsDotProduct );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".inverseSizeOfApertureAttenuationZone" );
	renderer->SetUniformVariable( uniformID, light.m_inverseSizeOfApertureAttenuationZone );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".percentOfLightIsAmbientZeroToOne" );
	renderer->SetUniformVariable( uniformID, light.m_percentOfLightIsAmbientZeroToOne );

	uniformID = program->GetUniformVariableIDFromName( uniformName + ".positionWasGiven" );
	renderer->SetUniformVariable( uniformID, light.m_positionWasGiven );
}

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetMatrixUniform( const std::string& uniformName, const Float4x4Matrix& matrix )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();
	renderer->SetUniformVariable( uniformID, matrix );
}

//-----------------------------------------------------------------------------------------------
void MaterialComponent::SetTextureUniform( const std::string& uniformName, int textureUnitID, const std::string& textureFileLocation, 
	Texture::FilteringMethod filteringMethod, Texture::WrappingMode wrappingMode )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	Renderer* renderer = Renderer::GetRenderer();
	TextureInfo texInfo;

	texInfo.textureUnitID = textureUnitID;
	renderer->SetActiveTextureUnit( textureUnitID );

	//FIX: Multiple declarations of same texture WILL take on the same characteristics!
	texInfo.texture = Texture::CreateOrGetTexture( textureFileLocation, filteringMethod, wrappingMode );
	renderer->SetUniformVariable( uniformID, textureUnitID );

	texInfo.samplerUniformID = uniformID;
	infoForTextures.push_back( texInfo );
}
