#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpaceShip.h"
#include "DynamicSparseSet.h"

class SpaceShip;

class StarSystem
{
public:
	StarSystem(int starXposMap, int starYposMap, float radius, sf::Color starColor, std::string name);
	~StarSystem();
	int starXposMap;
	int starYposMap;
	int id = 0;
	float radius;
	// connections array
	// I think it would be a graph
	std::vector<StarSystem*> connections;

	

	std::string name;
	sf::Color starColor;
	sf::CircleShape star;
	//std::vector<std::pair<int, int>> currentlyOccupiedCells; don't think I need this
	void Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);
	void ConnectTo(StarSystem* otherSystem);
	void DrawAllConnections(sf::RenderWindow& win);

	bool checkCollisionShip(SpaceShip* ship);

private:
	sf::Shader* setShader(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);



};

