#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpaceShip.h"
#include "DynamicSparseSet.h"

class SpaceShip;

class StarSystem
{
public:
	StarSystem(int starXposMap, int starYposMap, float radius, int teamID, sf::Color starColor, std::string name);
	~StarSystem();
	int starXposMap;
	int starYposMap;
	int id = 0;
	float radius;
	int teamID;

	//sf::RectangleShape;
	sf::RectangleShape smallHitBox;
	

	// connections array
	// I think it would be a graph
	// is this really necesery? Should I just use ids of the other stars?
	// I need to change this first
	//std::vector<StarSystem*> connections;
	DynamicSparseSet<int> connections;

	DynamicSparseSet<int> ships;

	std::string name;
	sf::Color starColor;
	sf::CircleShape star;
	//std::vector<std::pair<int, int>> currentlyOccupiedCells; don't think I need this
	void Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);
	//void ConnectTo(StarSystem* otherSystem);
	//void DrawAllConnections(sf::RenderWindow& win, DynamicSparseSet<StarSystem&> connected_stars);

	bool checkCollisionShip(SpaceShip* ship);

private:
	sf::Shader* setShader(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);



};

