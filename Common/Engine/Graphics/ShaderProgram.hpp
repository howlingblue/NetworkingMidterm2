#ifndef INCLUDED_SHADER_PROGRAM_HPP
#define INCLUDED_SHADER_PROGRAM_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <map>
#include <string>

class Renderer;

//-----------------------------------------------------------------------------------------------
//This class is used as an identifier in ShaderProgram's registry.
struct ShaderSet
{
	std::string vertexShaderFileLocation;
	std::string geometryShaderFileLocation;
	std::string pixelShaderFileLocation;

	ShaderSet() { }
	ShaderSet( const std::string& vertexShaderFileLoc, const std::string& pixelShaderFileLoc )
		: vertexShaderFileLocation( vertexShaderFileLoc )
		, pixelShaderFileLocation( pixelShaderFileLoc )
	{ }

	ShaderSet( const std::string& vertexShaderFileLoc, const std::string& geometryShaderFileLoc, const std::string& pixelShaderFileLoc )
		: vertexShaderFileLocation( vertexShaderFileLoc )
		, geometryShaderFileLocation( geometryShaderFileLoc )
		, pixelShaderFileLocation( pixelShaderFileLoc )
	{ }

	bool operator<( const ShaderSet& rhs ) const 
	{
		if( this->vertexShaderFileLocation < rhs.vertexShaderFileLocation )
		{
			return true;
		}
		else if ( this->vertexShaderFileLocation == rhs.vertexShaderFileLocation )
		{
			if( this->pixelShaderFileLocation < rhs.pixelShaderFileLocation )
			{
				return true;
			}
			else if( this->pixelShaderFileLocation == rhs.pixelShaderFileLocation )
			{
				if( this->geometryShaderFileLocation < rhs.geometryShaderFileLocation )
				{
					return true;
				}
			}
		}
		return false;
	}
};
//-----------------------------------------------------------------------------------------------
class ShaderProgram
{
	static std::map< ShaderSet, ShaderProgram* > s_programRegistry;

	int m_vertexShaderID, m_geometryShaderID, m_pixelShaderID;
	int m_shaderProgramID;
	std::map< std::string, int > m_attributeRegistry;
	std::map< std::string, int > m_uniformRegistry;

	void LoadShaderFromFile( std::string& out_shaderData, const char* shaderFileName );

	int CompileGeometryShaderOrDie( Renderer* renderer, const std::string& geometryShaderFileLocation );
	int CompilePixelShaderOrDie( Renderer* renderer, const std::string& pixelShaderFileLocation );
	int CompileVertexShaderOrDie( Renderer* renderer, const std::string& vertexShaderFileLocation );
	int LinkShadersIntoProgramOrDie( Renderer* renderer, const std::string& vertexShaderFileLocation, const std::string& pixelShaderFileLocation );
	int LinkShadersIntoProgramOrDie( Renderer* renderer, const std::string& vertexShaderFileLocation, const std::string& geometryShaderFileLocation, const std::string& pixelShaderFileLocation );

	void RegisterAttributeVariables( Renderer* renderer );
	void RegisterUniformVariables( Renderer* renderer );

public:
	ShaderProgram( const std::string& vertexShaderFileLocation, const std::string& pixelShaderFileLocation );
	ShaderProgram( const std::string& vertexShaderFileLocation, const std::string& geometryShaderFileLocation, const std::string& pixelShaderFileLocation );

	~ShaderProgram();

	int GetID() const { return m_shaderProgramID; }

	static ShaderProgram* CreateOrGetShaderProgram( const std::string& vertexShaderFileLocation, const std::string& pixelShaderFileLocation );
	static ShaderProgram* CreateOrGetShaderProgram( const std::string& vertexShaderFileLocation, const std::string& geometryShaderFileLocation, const std::string& pixelShaderFileLocation );
	static void CleanUpProgramRepository();

	int GetAttributeIDFromName( const std::string& attributeName ) const;
	int GetUniformVariableIDFromName( const std::string& uniformName ) const;
};

//-----------------------------------------------------------------------------------------------
inline int ShaderProgram::GetAttributeIDFromName( const std::string& attributeName ) const
{
	std::map< std::string, int >::const_iterator attribute = m_attributeRegistry.find( attributeName );
	//assert( attribute != m_attributeRegistry.end() );

	return attribute->second;
}

//-----------------------------------------------------------------------------------------------
inline int ShaderProgram::GetUniformVariableIDFromName( const std::string& uniformName ) const
{
	std::map< std::string, int >::const_iterator uniform = m_uniformRegistry.find( uniformName );
	//assert( uniform != m_uniformRegistry.end() );

	return uniform->second;
}

#endif //INCLUDED_SHADER_PROGRAM_HPP
