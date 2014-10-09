#include "LaserBeam.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "../Engine/Components/MaterialComponent.hpp"
#include "../Engine/Graphics/Renderer.hpp"
#include "../Engine/Graphics/VertexDataContainers.hpp"
#include "../Engine/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float LaserBeam::DAMAGING_LENGTH_SECONDS = 0.2f;
STATIC const float LaserBeam::LIFETIME_SECONDS = 2.5f;

//-----------------------------------------------------------------------------------------------
LaserBeam::LaserBeam( const Entity* firer )
	: m_firer( firer )
	, m_fireAngleAsVector( ConvertAngleToUnitCirclePosition( ConvertDegreesToRadians( firer->GetCurrentOrientation() ) ) )
	, m_secondsSinceFired( 0.f )
{
	m_clientPosition = firer->GetCurrentPosition();
	m_serverPosition = m_clientPosition;

	Renderer* renderer = Renderer::GetRenderer();
	m_laserMaterial = renderer->CreateOrGetNewMaterialComponent( L"LaserMaterial" );
	m_laserMaterial->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.vertex.330.glsl", "Data/Shaders/BasicNoTexture.fragment.330.glsl" ) );
	m_laserMaterial->SetModelMatrixUniform( "u_modelMatrix" );
	m_laserMaterial->SetViewMatrixUniform( "u_viewMatrix" );
	m_laserMaterial->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
void LaserBeam::Render() const
{
	std::vector< VertexColorData > m_laserVertexArray;
	m_laserVertexArray.push_back( VertexColorData( m_clientPosition.x, m_clientPosition.y, 0.f, 1.f, 0.f, 0.3f, ( LIFETIME_SECONDS - 1.f - m_secondsSinceFired ) / LIFETIME_SECONDS ) );
	m_laserVertexArray.push_back( VertexColorData( m_clientPosition.x + 600.f * m_fireAngleAsVector.x, m_clientPosition.y + 600.f * m_fireAngleAsVector.y, 0.f, 0.f, 0.f, 1.f, ( LIFETIME_SECONDS - m_secondsSinceFired ) / LIFETIME_SECONDS ) );

	static const int SIZE_OF_ARRAY_STRUCTURE = sizeof( VertexColorData );
	static const int NUMBER_OF_VERTEX_COORDINATES = 3;
	static const int NUMBER_OF_COLOR_COORDINATES = 4;
	static const int VERTEX_ARRAY_START = 0;

	Renderer* renderer = Renderer::GetRenderer();
	renderer->PushMatrix();

	renderer->BindBufferObject( Renderer::ARRAY_BUFFER, 0 ); //Not using buffers

	renderer->BindVertexArraysToAttributeLocation( Renderer::LOCATION_Vertex );
	renderer->BindVertexArraysToAttributeLocation( Renderer::LOCATION_Color );
	renderer->ApplyMaterialComponent( m_laserMaterial );

	renderer->SetPointerToGenericArray( Renderer::LOCATION_Vertex, NUMBER_OF_VERTEX_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &m_laserVertexArray[0].x );
	renderer->SetPointerToGenericArray( Renderer::LOCATION_Color, NUMBER_OF_COLOR_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &m_laserVertexArray[0].red );
	renderer->RenderVertexArray( Renderer::LINES, VERTEX_ARRAY_START, m_laserVertexArray.size() );

	renderer->RemoveMaterialComponent( m_laserMaterial );
	renderer->UnbindVertexArraysFromAttributeLocation( Renderer::LOCATION_Color );
	renderer->UnbindVertexArraysFromAttributeLocation( Renderer::LOCATION_Vertex );

	renderer->PopMatrix();
}

//-----------------------------------------------------------------------------------------------
void LaserBeam::Update( float deltaSeconds )
{
	m_secondsSinceFired += deltaSeconds;
}
