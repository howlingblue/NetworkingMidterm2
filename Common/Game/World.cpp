#include "World.hpp"

#include "../Engine/Graphics/Renderer.hpp"
#include "../Engine/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float World::OBJECTIVE_TOUCH_DISTANCE = 10.f;

//-----------------------------------------------------------------------------------------------
World::World()
	: m_objective( nullptr )
	, m_nextPlayerID( 0 )
	, m_camera( 250.f, -75.f, 200.f )
{
	m_camera.SetRotation( 0.f, 45.f, 90.f );

	Renderer* renderer = Renderer::GetRenderer();
	m_floorMaterial = renderer->CreateOrGetNewMaterialComponent( L"FloorMaterial" );
	m_floorMaterial->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.vertex.330.glsl", "Data/Shaders/BasicNoTexture.fragment.330.glsl" ) );
	m_floorMaterial->SetModelMatrixUniform( "u_modelMatrix" );
	m_floorMaterial->SetViewMatrixUniform( "u_viewMatrix" );
	m_floorMaterial->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
//Returns player pointer if player was found; nullptr otherwise
Entity* World::FindPlayerWithID( unsigned short targetID )
{
	Entity* foundPlayer = nullptr;

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		Entity*& player = m_players[ i ];

		if( player->GetID() == targetID )
		{
			foundPlayer = player;
			break;
		}
	}
	return foundPlayer;
}

//-----------------------------------------------------------------------------------------------
//Returns true if a player was successfully removed; false otherwise
bool World::RemovePlayer( Entity* player )
{
	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		if( m_players[ i ] == player )
		{
			m_players.erase( m_players.begin() + i );
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
void World::CheckForLaserImpacts( std::vector< std::pair< const Entity*, const Entity* > >& out_impactsThisFrame )
{
	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		const FloatVector2& playerPosition = m_players[ i ]->GetCurrentPosition();

		for( unsigned int j = 0; j < m_activeLasers.size(); ++i )
		{
			const FloatVector2& laserAngleVector = m_activeLasers[ i ]->GetAngleVector();
			const FloatVector2& laserSource = m_activeLasers[ i ]->GetCurrentPosition();

			FloatVector2 vectorFromPlayerToLaserSource = laserSource - playerPosition;
			FloatVector2 parallelProjectionOfVectorWithLaser = DotProduct( vectorFromPlayerToLaserSource, laserAngleVector ) * laserAngleVector;
			float distanceFromPlayerToBeam = ( vectorFromPlayerToLaserSource - parallelProjectionOfVectorWithLaser ).CalculateNorm();

			static const float TANK_SIZE = 10.f;
			if( distanceFromPlayerToBeam < 10.f )
			{
				out_impactsThisFrame.push_back( std::pair< const Entity*, const Entity* >( m_players[ i ], m_activeLasers[ i ]->GetFirer() ) );
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
//Returns player pointer if player was found; nullptr otherwise
Entity* World::FindPlayerTouchingObjective()
{
	FloatVector2 objectivePosition = m_objective->GetCurrentPosition();

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		FloatVector2 vectorFromObjectiveToLocalPlayer = objectivePosition - m_players[ i ]->GetCurrentPosition();
		if( vectorFromObjectiveToLocalPlayer.CalculateNorm() < OBJECTIVE_TOUCH_DISTANCE )
		{
			return m_players[ i ];
		}
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
void World::HandleFireEventFromPlayer( const Entity* player )
{
	LaserBeam* newLaser = new LaserBeam( player );
	m_activeLasers.push_back( newLaser );
}

//-----------------------------------------------------------------------------------------------
bool World::PlayerIsTouchingObjective( Entity* player )
{
	if( m_objective == nullptr )
		return false;

	FloatVector2 objectivePosition = m_objective->GetCurrentPosition();

	FloatVector2 vectorFromObjectiveToPlayer = objectivePosition - player->GetCurrentPosition();
	if( vectorFromObjectiveToPlayer.CalculateNorm() < OBJECTIVE_TOUCH_DISTANCE )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
void World::RenderFloor() const
{
	std::vector< VertexColorData > m_floorVertexArray;
	m_floorVertexArray.push_back( VertexColorData(   0.f,   0.f, -5.f, 0.4f, 0.2f, 0.f, 1.f ) );
	m_floorVertexArray.push_back( VertexColorData(   0.f, 500.f, -5.f, 0.4f, 0.2f, 0.f, 1.f ) );
	m_floorVertexArray.push_back( VertexColorData( 500.f,   0.f, -5.f, 0.4f, 0.2f, 0.f, 1.f ) );
	m_floorVertexArray.push_back( VertexColorData( 500.f, 500.f, -5.f, 0.4f, 0.2f, 0.f, 1.f ) );

	static const int SIZE_OF_ARRAY_STRUCTURE = sizeof( VertexColorData );
	static const int NUMBER_OF_VERTEX_COORDINATES = 3;
	static const int NUMBER_OF_COLOR_COORDINATES = 4;
	static const int VERTEX_ARRAY_START = 0;

	Renderer* renderer = Renderer::GetRenderer();
	renderer->PushMatrix();

	renderer->BindBufferObject( Renderer::ARRAY_BUFFER, 0 ); //Not using buffers

	renderer->BindVertexArraysToAttributeLocation( Renderer::LOCATION_Vertex );
	renderer->BindVertexArraysToAttributeLocation( Renderer::LOCATION_Color );
	renderer->ApplyMaterialComponent( m_floorMaterial );

	renderer->SetPointerToGenericArray( Renderer::LOCATION_Vertex, NUMBER_OF_VERTEX_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &m_floorVertexArray[0].x );
	renderer->SetPointerToGenericArray( Renderer::LOCATION_Color, NUMBER_OF_COLOR_COORDINATES, Renderer::FLOAT_TYPE, false, SIZE_OF_ARRAY_STRUCTURE, &m_floorVertexArray[0].red );
	renderer->RenderVertexArray( Renderer::TRIANGLE_STRIP, VERTEX_ARRAY_START, m_floorVertexArray.size() );

	renderer->RemoveMaterialComponent( m_floorMaterial );
	renderer->UnbindVertexArraysFromAttributeLocation( Renderer::LOCATION_Color );
	renderer->UnbindVertexArraysFromAttributeLocation( Renderer::LOCATION_Vertex );

	renderer->PopMatrix();
}

//-----------------------------------------------------------------------------------------------
void World::UpdateLasers( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_activeLasers.size(); ++i )
	{
		m_activeLasers[ i ]->Update( deltaSeconds );

		if( m_activeLasers[ i ]->ReadyForCleanup() )
		{
			delete m_activeLasers[ i ];
			m_activeLasers.erase( m_activeLasers.begin() + i );
			--i;
		}
	}
}



//-----------------------------------------------------------------------------------------------
void World::Render() const
{
	Renderer* renderer = Renderer::GetRenderer();

	//renderer->EnableFeature( Renderer::COLOR_BLENDING );
	//renderer->SetAlphaBlendingFunction( Renderer::SOURCE_ALPHA, Renderer::ONE_MINUS_SOURCE_ALPHA );
	renderer->EnableFeature( Renderer::DEPTH_TESTING );

	renderer->ClearColorBuffer();
	renderer->ClearDepthBuffer();

	renderer->SetPerpectiveProjection( 90.f, 1.f, 0.1f, 1000.f );
	//renderer->DisableFeature( Renderer::DEPTH_TESTING );
	//renderer->DisableDepthBufferWriting();
	renderer->SetLineWidth( 2.f );

	m_camera.ViewWorldThrough();

	if( m_objective != nullptr )
		m_objective->Render();

	RenderFloor();

	for( unsigned int i = 0; i < m_activeLasers.size(); ++i )
	{
		m_activeLasers[ i ]->Render();
	}

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		m_players[ i ]->Render();
	}

	//renderer->EnableDepthBufferWriting();
	//renderer->EnableFeature( Renderer::DEPTH_TESTING );
}

//-----------------------------------------------------------------------------------------------
void World::Update( float deltaSeconds )
{
	if( m_objective != nullptr )
		m_objective->Update( deltaSeconds );

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		m_players[ i ]->Update( deltaSeconds );
	}

	UpdateLasers( deltaSeconds );
}
