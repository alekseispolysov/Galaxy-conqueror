#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpaceShip.h"

class SpaceShip;

class StarSystem
{
public:
	StarSystem(int starXposMap, int starYposMap, float radius, sf::Color starColor, std::string name);
	~StarSystem();
	int starXposMap;
	int starYposMap;
	float radius;
	// connections array
	std::vector<StarSystem*> connections;

	std::string name;
	sf::Color starColor;
	sf::CircleShape star;

	void Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);
	void ConnectTo(StarSystem* otherSystem);
	void DrawAllConnections(sf::RenderWindow& win);

	bool checkCollisionShip(SpaceShip* ship);

private:
	sf::Shader* setShader(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);



};

