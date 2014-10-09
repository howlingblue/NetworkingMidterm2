#include "World.hpp"

#include "../Engine/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float World::OBJECTIVE_TOUCH_DISTANCE = 10.f;

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
	if( m_objective != nullptr )
		m_objective->Render();

	for( unsigned int i = 0; i < m_activeLasers.size(); ++i )
	{
		m_activeLasers[ i ]->Render();
	}

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		m_players[ i ]->Render();
	}
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
