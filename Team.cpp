#include "Team.h"
#include "StarSystem.h"
#include "SpaceShip.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>
#include <algorithm>
#include <tuple>
#include "DynamicSparseSet.h"
#include <cassert>
#include <unordered_map>
#include <cmath>
#include <utility>  // For std::pair
#include <vector>
#include <queue>
#include <cassert>

Team::Team(int team_id, sf::Color teamColor) : team_id(team_id), teamColor(teamColor) 
{
}

Team::~Team()
{
}

void Team::addShipToTeam(int shipID)
{
	teamShips.insert(shipID, shipID);
}

void Team::removeShipFromTeam(int shipID)
{
	teamShips.erase(shipID);
}

void Team::addStarToTeam(int starID)
{
	teamStars.insert(starID, starID);
}

void Team::removeStarFromTeam(int starID)
{	
	teamShips.erase(starID);
}

bool Team::checkTeamIsAlive()
{
	if (teamShips.size() < 1 && teamStars.size() < 1) {
		return false;
	}
	return true;
}
