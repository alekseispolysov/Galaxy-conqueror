#pragma once
#include "StarSystem.h"
#include "SpaceShip.h"
#include <iostream>
#include <variant>
#include <algorithm>
#include <tuple>

//define vairants for dynamic arrays
using VariantType = std::variant<SpaceShip*, StarSystem*>;

class Sector {
public:
	sf::Vector2f position;
	float witdth;
	float height;
	int sector_size;
	sf::RectangleShape sectorRect;
	std::vector <StarSystem*> sectorStars;
	std::vector <SpaceShip*> sectorShips; 

	sf::Vertex sectorBorder[8];

	// will be passed base value
	Sector(int sectorSize, sf::Vector2f position);
	~Sector();
	std::vector <VariantType> checkColisionInSector();
	void displaySector(sf::RenderWindow& win);

private:

};

//   ===========================================================================================================
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
	std::vector <SpaceShip*> movingShips;

	std::vector <std::vector<Sector*>> allSectors;
	std::vector <Sector*> activeSectors;
	std::vector <Sector*> shipsSectors;
	std::vector <Sector*> starsSectors;

	int sectorSize = 100;

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

	std::vector <VariantType> checkColisionForAllShips();
	void printSectors();
	void determineSectorsForObjects();
	std::tuple<bool, SpaceShip*> collisionBetweenSectors(SpaceShip* ship, Sector* s); 
private:

};

