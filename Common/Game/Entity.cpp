#include "Entity.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include <cmath>

#include "../Engine/Components/MaterialComponent.hpp"
#include "../Engine/Graphics/Renderer.hpp"
#include "../Engine/Graphics/VertexDataContainers.hpp"


//-----------------------------------------------------------------------------------------------
Entity::Entity()
	: m_playerID( 0 )
	, m_isIt( false )
	, m_clientOrientationDegrees( 0 )
	, m_serverOrientationDegrees( 0 )
	, m_color( (unsigned char)255, 0, 0, 255 )
{
	Renderer* renderer = Renderer::GetRenderer();
	m_tankMaterial = renderer->CreateOrGetNewMaterialComponent( L"TankMaterial" );
	m_tankMaterial->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.vertex.330.glsl", "Data/Shaders/BasicNoTexture.fragment.330.glsl" ) );
	m_tankMaterial->SetModelMatrixUniform( "u_modelMatrix" );
	m_tankMaterial->SetViewMatrixUniform( "u_viewMatrix" );
	m_tankMaterial->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
void Entity::Render() const
{
	float halfWidth = 5.f, halfHeight = 5.f, halfDepth = 5.f;

	std::vector< VertexColorData > tankVertexArray;
	tankVertexArray.push_back( VertexColorData( -halfWidth, -halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth, -halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData( -halfWidth,  halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth,  halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth,  halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth, -halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth, -halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData( -halfWidth, -halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData( -halfWidth, -halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData( -halfWidth,  halfHeight, -halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData( -halfWidth,  halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth,  halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData( -halfWidth, -halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );
	tankVertexArray.push_back( VertexColorData(  halfWidth, -halfHeight,  halfDepth, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, 1.f ) );

	halfHeight = 2.f;
	halfDepth = 2.f;
	std::vector< VertexColorData > turretVertexArray;
	turretVertexArray.push_back( VertexColorData(  0.f, -halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f, -halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData(  0.f,  halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f,  halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f,  halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f, -halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f, -halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData(  0.f, -halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData(  0.f, -halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData(  0.f,  halfHeight, -halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData(  0.f,  halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f,  halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData(  0.f, -halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );
	turretVertexArray.push_back( VertexColorData( 12.f, -halfHeight,  halfDepth, 1.f, 1.f, 1.f, 1.f ) );

	static const int SIZE_OF_ARRAY_STRUCTURE = sizeof( VertexColorData );
	static const int NUMBER_OF_VERTEX_COORDINATES = 3;
	static const int NUMBER_OF_COLOR_COORDINATES = 4;
	static const int VERTEX_ARRAY_START = 0;

	Renderer* renderer = Renderer::GetRenderer();

	renderer->PushMatrix();
	renderer->TranslateWorld( FloatVector3( m_clientPosition.x, m_clientPosition.y, 0.f ) );
	renderer->RotateWorldAboutAxisDegrees( FloatVector3( 0.f, 0.f, 1.f ), -m_clientOrientationDegrees );

	renderer->BindBufferObject( Renderer::ARRAY_BUFFER, 0 ); //Not using buffers

	renderer->BindVertexArraysToAttributeLocation( Renderer::LOCATION_Vertex );
	renderer->BindVertexArraysToAttributeLocation( Renderer::LOCATION_Color );
	renderer->ApplyMaterialComponent( m_tankMaterial );

	renderer->SetPointerToGenericArray( Renderer::LOCATION_Vertex, NUMBER_OF_VERTEX_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &tankVertexArray[0].x );
	renderer->SetPointerToGenericArray( Renderer::LOCATION_Color, NUMBER_OF_COLOR_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &tankVertexArray[0].red );
	renderer->RenderVertexArray( Renderer::TRIANGLE_STRIP, VERTEX_ARRAY_START, tankVertexArray.size() );

	renderer->SetPointerToGenericArray( Renderer::LOCATION_Vertex, NUMBER_OF_VERTEX_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &turretVertexArray[0].x );
	renderer->SetPointerToGenericArray( Renderer::LOCATION_Color, NUMBER_OF_COLOR_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &turretVertexArray[0].red );
	renderer->RenderVertexArray( Renderer::TRIANGLE_STRIP, VERTEX_ARRAY_START, turretVertexArray.size() );

	renderer->RemoveMaterialComponent( m_tankMaterial );
	renderer->UnbindVertexArraysFromAttributeLocation( Renderer::LOCATION_Color );
	renderer->UnbindVertexArraysFromAttributeLocation( Renderer::LOCATION_Vertex );

	renderer->PopMatrix();
}

//-----------------------------------------------------------------------------------------------
void Entity::Update( float /*deltaSeconds*/ )
{
	//Janky Dead Reckoning
	static const float GUESSTIMATED_LATENCY = .001f;
	static const float PERCENT_TO_INTERPOLATE_PER_FRAME = 0.2f;
	FloatVector2 goalPosition = m_serverPosition + ( m_serverVelocity * GUESSTIMATED_LATENCY );

	m_clientPosition += PERCENT_TO_INTERPOLATE_PER_FRAME * ( goalPosition - m_clientPosition );
	m_clientVelocity += PERCENT_TO_INTERPOLATE_PER_FRAME * ( m_serverVelocity - m_clientVelocity );

	//Interpolate rotation
	float angularDisplacementDegrees = m_serverOrientationDegrees - m_clientOrientationDegrees;
	float angularRotationThisFrame = PERCENT_TO_INTERPOLATE_PER_FRAME * angularDisplacementDegrees;

	if( abs( angularDisplacementDegrees ) < angularRotationThisFrame )
		return;

	while( angularDisplacementDegrees > 180.f )
		angularDisplacementDegrees -= 360.f;

	while( angularDisplacementDegrees < -180.f )
		angularDisplacementDegrees += 360.f;

	if( angularDisplacementDegrees < 0 )
		m_clientOrientationDegrees -= angularDisplacementDegrees;

	if( angularDisplacementDegrees > 0 )
		m_clientOrientationDegrees += angularDisplacementDegrees;

	while( m_clientOrientationDegrees >= 360.f )
	{
		m_clientOrientationDegrees -= 360.f;
	}

	while( m_clientOrientationDegrees < 0.f )
	{
		m_clientOrientationDegrees += 360.f;
	}

}