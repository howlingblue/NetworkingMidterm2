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
//Returns player pointer if player was found; nullptr otherwise
Entity* World::FindPlayerTouchingObjective()
{
	Vector2 objectivePosition = m_objective->GetCurrentPosition();

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		Vector2 vectorFromObjectiveToLocalPlayer = objectivePosition - m_players[ i ]->GetCurrentPosition();
		if( vectorFromObjectiveToLocalPlayer.Length() < OBJECTIVE_TOUCH_DISTANCE )
		{
			return m_players[ i ];
		}
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
bool World::PlayerIsTouchingObjective( Entity* player )
{
	if( m_objective == nullptr )
		return false;

	Vector2 objectivePosition = m_objective->GetCurrentPosition();

	Vector2 vectorFromObjectiveToPlayer = objectivePosition - player->GetCurrentPosition();
	if( vectorFromObjectiveToPlayer.Length() < OBJECTIVE_TOUCH_DISTANCE )
	{
		return true;
	}
	return false;
}



//-----------------------------------------------------------------------------------------------
void World::Render() const
{
	if( m_objective != nullptr )
		m_objective->Render();

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
}
