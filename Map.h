#pragma once
#include "StarSystem.h"
#include "SpaceShip.h"
#include <iostream>
#include <variant>
#include <algorithm>


//define vairants for dynamic arrays
using VariantType = std::variant<SpaceShip*, StarSystem*>;

class Map
{
public:
	// Map size
	sf::Vector2f mapSize;
	// vertex array uninicialized
	sf::Vertex mapBorder[8];

	// arrays with objects
	std::vector <StarSystem*> stars;
	std::vector <StarSystem*> selectedStars;
	std::vector <SpaceShip*> allShips;
	std::vector <SpaceShip*> selectedShips;
	std::vector <VariantType> selectedObjects;
	std::vector <VariantType> selectableObjects;

	Map(sf::Vector2f mapSize);
	~Map();
	void Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);
	
	void selectObject(VariantType obj);
	bool checkIfObjectSelected(VariantType obj, bool del=false);

	void addStar(StarSystem* star);
	void selectStar(StarSystem* star);
	void addShip(SpaceShip* ship);
	void selectShip(SpaceShip* ship);
	void cleanSelection();
	void destroyShip(SpaceShip* ship);

private:

};

