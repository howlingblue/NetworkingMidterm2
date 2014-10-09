#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../DialogInterface.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"

//-----------------------------------------------------------------------------------------------
STATIC std::map< ShaderSet, ShaderProgram* > ShaderProgram::s_programRegistry;


//-----------------------------------------------------------------------------------------------
void BuildVisualStudioDebugMessage( std::string& out_debugMessage, const std::string& fileName, const std::string& errorMessage )
{
	out_debugMessage.assign( errorMessage );
	std::size_t zeroLocation = out_debugMessage.find( "0(" );
	while( zeroLocation != std::string::npos )
	{
		out_debugMessage.replace( zeroLocation, 1, fileName );
		zeroLocation = out_debugMessage.find( "0(" );
	}
}

//-----------------------------------------------------------------------------------------------
void ShaderProgram::LoadShaderFromFile( std::string& out_shaderData, const char* shaderFileName )
{
	char* shaderData = nullptr;
	FILE* shaderFile = nullptr;
	int errorResult = fopen_s( &shaderFile, shaderFileName, "rb" );
	if( errorResult != 0 )
		exit( -5 );

	fseek( shaderFile, 0, SEEK_END );
	long fileSize = ftell( shaderFile );
	rewind( shaderFile );

	shaderData = new char[ fileSize ];
	fread( shaderData, sizeof( char ), fileSize, shaderFile );
	fclose( shaderFile );

	out_shaderData.assign( shaderData, fileSize );
	delete[] shaderData;
}

//-----------------------------------------------------------------------------------------------
int ShaderProgram::CompileGeometryShaderOrDie( Renderer* renderer, const std::string& geometryShaderFileLocation )
{
	std::string geometryShaderData, rawErrorText;
	LoadShaderFromFile( geometryShaderData, geometryShaderFileLocation.c_str() );

	int geometryShaderID = renderer->CompileShader( Renderer::GEOMETRY_SHADER, geometryShaderData.c_str(), rawErrorText );
	if( geometryShaderID == -1 )
	{
		std::string visualStudioDebugMessage;
		BuildVisualStudioDebugMessage( visualStudioDebugMessage, geometryShaderFileLocation, rawErrorText );
		OutputDebugStringA( visualStudioDebugMessage.c_str() );

		std::string systemDialogMessage;
		systemDialogMessage.append( "FATAL ERROR:\n\tGeometry Shader Compilation Error!\n\n");
		systemDialogMessage.append( visualStudioDebugMessage );
		systemDialogMessage.append( "\nPlease submit an error report with information on how you caused this error." );
		PopUpSystemDialog( "FATAL ERROR", systemDialogMessage );
		exit( -12 );
	}
	return geometryShaderID;
}

//-----------------------------------------------------------------------------------------------
int ShaderProgram::CompilePixelShaderOrDie( Renderer* renderer, const std::string& pixelShaderFileLocation )
{
	std::string pixelFragmentShaderData, rawErrorText;
	LoadShaderFromFile( pixelFragmentShaderData, pixelShaderFileLocation.c_str() );

	int pixelShaderID = renderer->CompileShader( Renderer::PIXEL_FRAGMENT_SHADER, pixelFragmentShaderData.c_str(), rawErrorText );

	if( pixelShaderID == -1 )
	{
		std::string visualStudioDebugMessage;
		BuildVisualStudioDebugMessage( visualStudioDebugMessage, pixelShaderFileLocation, rawErrorText );
		OutputDebugStringA( visualStudioDebugMessage.c_str() );

		std::string systemDialogMessage;
		systemDialogMessage.append( "FATAL ERROR:\n\tPixel Shader Compilation Error!\n\n");
		systemDialogMessage.append( visualStudioDebugMessage );
		systemDialogMessage.append( "\nPlease submit an error report with information on how you caused this error." );
		PopUpSystemDialog( "FATAL ERROR", systemDialogMessage );
		exit( -12 );
	}
	return pixelShaderID;
}

//-----------------------------------------------------------------------------------------------
int ShaderProgram::CompileVertexShaderOrDie( Renderer* renderer, const std::string& vertexShaderFileLocation )
{
	std::string vertexShaderData, rawErrorText;
	LoadShaderFromFile( vertexShaderData, vertexShaderFileLocation.c_str() );

	int vertexShaderID = renderer->CompileShader( Renderer::VERTEX_SHADER, vertexShaderData.c_str(), rawErrorText );
	if( vertexShaderID == -1 )
	{
		std::string visualStudioDebugMessage;
		BuildVisualStudioDebugMessage( visualStudioDebugMessage, vertexShaderFileLocation, rawErrorText );
		OutputDebugStringA( visualStudioDebugMessage.c_str() );

		std::string systemDialogMessage;
		systemDialogMessage.append( "FATAL ERROR:\n\tVertex Shader Compilation Error!\n\n");
		systemDialogMessage.append( visualStudioDebugMessage );
		systemDialogMessage.append( "\nPlease submit an error report with information on how you caused this error." );
		PopUpSystemDialog( "GameName: FATAL ERROR", systemDialogMessage );
		exit( -10 );
	}
	return vertexShaderID;
}

//-----------------------------------------------------------------------------------------------
int ShaderProgram::LinkShadersIntoProgramOrDie( Renderer* renderer, const std::string& vertexShaderFileLocation, const std::string& pixelShaderFileLocation )
{
	std::string rawErrorText;
	int programID = renderer->CreateProgramFromShaders( m_vertexShaderID, m_geometryShaderID, m_pixelShaderID, rawErrorText );

	if( programID == -1 )
	{
		OutputDebugStringA( rawErrorText.c_str() );

		std::string systemDialogMessage;
		systemDialogMessage.append( "FATAL ERROR:\n\tVShader Program Linking Error!\n\n");
		systemDialogMessage.append( "SHADERS LINKING:" );
		systemDialogMessage.append( "\nVertex:\t" + vertexShaderFileLocation );
		systemDialogMessage.append( "\nPixel:\t" + pixelShaderFileLocation );
		systemDialogMessage.append( "\n\nRAW ERROR MESSAGE:\n" );
		systemDialogMessage.append( rawErrorText );
		systemDialogMessage.append( "\nPlease submit an error report with information on how you caused this error." );
		PopUpSystemDialog( "GameName: FATAL ERROR", systemDialogMessage );
		exit( -13 );
	}
	return programID;
}

//-----------------------------------------------------------------------------------------------
int ShaderProgram::LinkShadersIntoProgramOrDie( Renderer* renderer, const std::string& vertexShaderFileLocation, 
												const std::string& geometryShaderFileLocation, const std::string& pixelShaderFileLocation )
{
	std::string rawErrorText;
	int programID = renderer->CreateProgramFromShaders( m_vertexShaderID, m_geometryShaderID, m_pixelShaderID, rawErrorText );

	if( programID == -1 )
	{
		OutputDebugStringA( rawErrorText.c_str() );

		std::string systemDialogMessage;
		systemDialogMessage.append( "FATAL ERROR:\n\tVShader Program Linking Error!\n\n");
		systemDialogMessage.append( "SHADERS LINKING:" );
		systemDialogMessage.append( "\nVertex:\t" + vertexShaderFileLocation );
		systemDialogMessage.append( "\nGeometry:\t" + geometryShaderFileLocation );
		systemDialogMessage.append( "\nPixel:\t" + pixelShaderFileLocation );
		systemDialogMessage.append( "\n\nRAW ERROR MESSAGE:\n" );
		systemDialogMessage.append( rawErrorText );
		systemDialogMessage.append( "\nPlease submit an error report with information on how you caused this error." );
		PopUpSystemDialog( "GameName: FATAL ERROR", systemDialogMessage );
		exit( -13 );
	}
	return programID;
}

//-----------------------------------------------------------------------------------------------
void ShaderProgram::RegisterAttributeVariables( Renderer* renderer )
{
	int numberOfAttributes = renderer->GetNumberOfAttributesInProgram( this );

	std::string attributeName;
	for( int attributeIndex = 0; attributeIndex < numberOfAttributes; ++attributeIndex )
	{
		renderer->GetShaderAttributeName( this, attributeIndex, attributeName );
		m_attributeRegistry[ attributeName ] = renderer->GetAttributeLocation( this, attributeName );
	}
}

//-----------------------------------------------------------------------------------------------
void ShaderProgram::RegisterUniformVariables( Renderer* renderer )
{
	int numberOfUniforms = renderer->GetNumberOfUniformsInProgram( this );

	std::string uniformName;
	for( int uniformIndex = 0; uniformIndex < numberOfUniforms; ++uniformIndex )
	{
		renderer->GetShaderUniformName( this, uniformIndex, uniformName );
		m_uniformRegistry[ uniformName ] = renderer->GetUniformVariableLocation( this, uniformName );
	}
}

//-----------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram( const std::string& vertexShaderFileLocation, const std::string& pixelShaderFileLocation )
	: m_shaderProgramID( 0 )
	, m_vertexShaderID( -1 )
	, m_geometryShaderID( -1 )
	, m_pixelShaderID( -1 )
{
	Renderer* renderer = Renderer::GetRenderer();

	std::string vertexShaderData, pixelFragmentShaderData, rawErrorText;

	m_vertexShaderID = CompileVertexShaderOrDie( renderer, vertexShaderFileLocation );
	m_pixelShaderID = CompilePixelShaderOrDie( renderer, pixelShaderFileLocation );

	m_shaderProgramID = LinkShadersIntoProgramOrDie( renderer, vertexShaderFileLocation, pixelShaderFileLocation );

	//Detach shaders so that they will be deleted automatically when the program is deleted
	renderer->DetachShaderFromProgram( m_vertexShaderID, m_shaderProgramID );
	renderer->DetachShaderFromProgram( m_pixelShaderID, m_shaderProgramID );

	RegisterAttributeVariables( renderer );
	RegisterUniformVariables( renderer );

	s_programRegistry[ ShaderSet( vertexShaderFileLocation, pixelShaderFileLocation ) ] = this;
}

//-----------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram( const std::string& vertexShaderFileLocation, const std::string& geometryShaderFileLocation, const std::string& pixelShaderFileLocation )
	: m_shaderProgramID( 0 )
	, m_vertexShaderID( -1 )
	, m_geometryShaderID( -1 )
	, m_pixelShaderID( -1 )
{
	Renderer* renderer = Renderer::GetRenderer();

	std::string vertexShaderData, geometryShaderData, pixelFragmentShaderData, rawErrorText;

	m_vertexShaderID = CompileVertexShaderOrDie( renderer, vertexShaderFileLocation );
	m_geometryShaderID = CompileGeometryShaderOrDie( renderer, geometryShaderFileLocation );
	m_pixelShaderID = CompilePixelShaderOrDie( renderer, pixelShaderFileLocation );

	m_shaderProgramID = LinkShadersIntoProgramOrDie( renderer, vertexShaderFileLocation, geometryShaderFileLocation, pixelShaderFileLocation );

	//Detach shaders so that they will be deleted automatically when the program is deleted
	renderer->DetachShaderFromProgram( m_vertexShaderID, m_shaderProgramID );
	renderer->DetachShaderFromProgram( m_geometryShaderID, m_shaderProgramID );
	renderer->DetachShaderFromProgram( m_pixelShaderID, m_shaderProgramID );

	RegisterAttributeVariables( renderer );
	RegisterUniformVariables( renderer );

	s_programRegistry[ ShaderSet( vertexShaderFileLocation, geometryShaderFileLocation, pixelShaderFileLocation ) ] = this;
}

//-----------------------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram() { }

//-----------------------------------------------------------------------------------------------
STATIC ShaderProgram* ShaderProgram::CreateOrGetShaderProgram( const std::string& vertexShaderFileLocation, const std::string& pixelShaderFileLocation )
{
	std::map< ShaderSet, ShaderProgram* >::iterator programIterator = s_programRegistry.find( ShaderSet( vertexShaderFileLocation, pixelShaderFileLocation ) );

	if( programIterator == s_programRegistry.end() )
	{
		ShaderProgram* program = new ShaderProgram( vertexShaderFileLocation, pixelShaderFileLocation );
		return program;
	}

	return programIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC ShaderProgram* ShaderProgram::CreateOrGetShaderProgram( const std::string& vertexShaderFileLocation, const std::string& geometryShaderFileLocation, const std::string& pixelShaderFileLocation )
{
	std::map< ShaderSet, ShaderProgram* >::iterator programIterator = s_programRegistry.find( ShaderSet( vertexShaderFileLocation, geometryShaderFileLocation, pixelShaderFileLocation ) );

	if( programIterator == s_programRegistry.end() )
	{
		ShaderProgram* program = new ShaderProgram( vertexShaderFileLocation, geometryShaderFileLocation, pixelShaderFileLocation );
		return program;
	}

	return programIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC void ShaderProgram::CleanUpProgramRepository()
{
	Renderer* renderer = Renderer::GetRenderer();

	ShaderProgram* program;
	for( std::map< ShaderSet, ShaderProgram* >::iterator pair = s_programRegistry.begin();
		pair != s_programRegistry.end();
		++pair )
	{
		program = pair->second;
		renderer->DeleteProgramDataOnCard( program );
		delete program;
	}
	s_programRegistry.clear();
}

