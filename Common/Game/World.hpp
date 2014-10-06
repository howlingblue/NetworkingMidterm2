#pragma once
#ifndef INCLUDED_WORLD_HPP
#define INCLUDED_WORLD_HPP

#include <vector>
#include "Entity.hpp"

//-----------------------------------------------------------------------------------------------
class World
{
	static const float OBJECTIVE_TOUCH_DISTANCE;

public:
	static const unsigned char MAX_HEALTH = 1;
	static const unsigned char SCORE_NEEDED_TO_WIN = 10;

	World();
	~World();

	void AddNewPlayer( Entity* newPlayer ) { m_players.push_back( newPlayer ); }
	Entity* FindPlayerWithID( unsigned short targetID );
	bool RemovePlayer( Entity* player );

	unsigned char GetNextPlayerID();
	unsigned int GetNumberOfPlayers() const { return m_players.size(); }
	const Entity* GetObjective() { return m_objective; }
	Entity* FindPlayerTouchingObjective();
	bool PlayerIsTouchingObjective( Entity* player );
	void SetObjective( Entity* newObjective );

	void Render() const;
	void Update( float deltaSeconds );

private:
	Entity* m_objective;
	std::vector< Entity* > m_players;
	unsigned char m_nextPlayerID;
};



//-----------------------------------------------------------------------------------------------
inline World::World()
	: m_objective( nullptr )
	, m_nextPlayerID( 0 )
{
}

//-----------------------------------------------------------------------------------------------
inline World::~World()
{
	if( m_objective != nullptr )
		delete m_objective;

	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		delete m_players[ i ];
	}

	m_players.clear();
}

//-----------------------------------------------------------------------------------------------
inline unsigned char World::GetNextPlayerID()
{
	unsigned char nextPlayerID = m_nextPlayerID;
	++m_nextPlayerID;
	return nextPlayerID;
}

//-----------------------------------------------------------------------------------------------
inline void World::SetObjective( Entity* newObjective )
{
	delete m_objective;
	m_objective = newObjective;
}

#endif //INCLUDED_WORLD_HPP