#pragma once
#ifndef INCLUDED_WORLD_HPP
#define INCLUDED_WORLD_HPP

#include <utility>
#include <vector>
#include "../Engine/Camera.hpp"
#include "Entity.hpp"
#include "LaserBeam.hpp"

struct MaterialComponent;

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
	Entity* FindPlayerTouchingObjective();
	Entity* FindPlayerWithID( unsigned short targetID );
	bool RemovePlayer( Entity* player );

	void CheckForLaserImpacts( std::vector< std::pair< const Entity*, const Entity* > >& out_impactsThisFrame );
	unsigned char GetNextPlayerID();
	unsigned int GetNumberOfPlayers() const { return m_players.size(); }
	const Entity* GetObjective() { return m_objective; }
	void HandleFireEventFromPlayer( const Entity* player );
	bool PlayerIsTouchingObjective( Entity* player );
	void RenderFloor() const;
	void SetObjective( Entity* newObjective );
	void UpdateLasers( float deltaSeconds );

	void Render() const;
	void Update( float deltaSeconds );

private:
	Camera m_camera;
	Entity* m_objective;
	std::vector< LaserBeam* > m_activeLasers;
	std::vector< Entity* > m_players;
	unsigned char m_nextPlayerID;
	MaterialComponent* m_floorMaterial;
};



//-----------------------------------------------------------------------------------------------
inline World::~World()
{
	if( m_objective != nullptr )
		delete m_objective;

	for( unsigned int i = 0; i < m_activeLasers.size(); ++i )
	{
		delete m_activeLasers[ i ];
	}
	m_activeLasers.clear();

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