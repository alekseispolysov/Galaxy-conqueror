#pragma once
#include "StarSystem.h"
#include "SpaceShip.h"
#include <iostream>
#include <variant>
#include <algorithm>
#include <tuple>
#include "DynamicSparseSet.h"
#include <cassert>
#include <unordered_map>
#include <cmath>
#include <utility>  // For std::pair
#include <TGUI/TGUI.hpp>
#include <TGUI/Core.hpp>          // Core system for GUI management
#include <TGUI/Widgets/Button.hpp> // Include specific widget headers, like Button
#include <TGUI/Backend/SFML-Graphics.hpp>

class Team
{
private:

public:
	// I need id, color

	int team_id;
	sf::Color teamColor;

	DynamicSparseSet<int> teamShips;
	DynamicSparseSet<int> teamStars;

	Team(int team_id, sf::Color teamColor);
	~Team();

	void addShipToTeam(int shipID);
	void removeShipFromTeam(int shipID);

	void addStarToTeam(int starID);
	void removeStarFromTeam(int starID);

	bool checkTeamIsAlive();

};

