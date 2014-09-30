#pragma once
#ifndef INCLUDED_WORLD_HPP
#define INCLUDED_WORLD_HPP

#include <vector>
#include "Player.hpp"

//-----------------------------------------------------------------------------------------------
class World
{
	static const float OBJECTIVE_TOUCH_DISTANCE;

public:
	World();
	~World();

	void AddNewPlayer( Player* newPlayer ) { m_players.push_back( newPlayer ); }
	Player* FindPlayerWithID( unsigned short targetID );
	bool RemovePlayer( Player* player );

	Player* FindPlayerTouchingIt();

	void Render() const;
	void Update( float deltaSeconds );

private:
	std::vector< Player* > m_players;
};



//-----------------------------------------------------------------------------------------------
inline World::World()
{

}

//-----------------------------------------------------------------------------------------------
inline World::~World()
{
	for( unsigned int i = 0; i < m_players.size(); ++i )
	{
		delete m_players[ i ];
	}

	m_players.clear();
}

#endif //INCLUDED_WORLD_HPP