#include "World.hpp"

#include "../Engine/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const float World::OBJECTIVE_TOUCH_DISTANCE = 10.f;

//-----------------------------------------------------------------------------------------------
//Returns player pointer if player was found; nullptr otherwise
Player* World::FindPlayerWithID( unsigned short targetID )
{
	Player* foundPlayer = nullptr;

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		Player*& player = m_players[ i ];

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
bool World::RemovePlayer( Player* player )
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
Player* World::FindPlayerTouchingIt()
{
	Player* itPlayer = nullptr;
	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		if( m_players[ i ]->IsIt() )
		{
			itPlayer = m_players[ i ];
			break;
		}
	}


	if( itPlayer == nullptr )
		return nullptr;
	Vector2 itPlayerPosition = itPlayer->GetCurrentPosition();

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		if( m_players[ i ] == itPlayer )
			continue;

		Vector2 vectorFromItToLocalPlayer = itPlayerPosition - m_players[ i ]->GetCurrentPosition();
		if( vectorFromItToLocalPlayer.Length() < OBJECTIVE_TOUCH_DISTANCE )
		{
			return m_players[ i ];
		}
	}

	return nullptr;
}



//-----------------------------------------------------------------------------------------------
void World::Render() const
{
	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		m_players[ i ]->Render();
	}
}

//-----------------------------------------------------------------------------------------------
void World::Update( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		m_players[ i ]->Update( deltaSeconds );
	}
}
