#pragma once
#include "StarSystem.h"
#include "SpaceShip.h"
#include <iostream>
#include <variant>
#include <algorithm>
#include <tuple>
#include "DynamicSparseSet.h"
#include <cassert>

//class DynamicSparseSet;

//define vairants for dynamic arrays
using VariantType = std::variant<SpaceShip*, StarSystem*>;

class Sector {
public:
	int sector_id = 0;
	sf::Vector2f position;
	float witdth;
	float height;
	int sector_size;
	sf::RectangleShape sectorRect;
	DynamicSparseSet <int> sectorStars;// this needs to be changed
	DynamicSparseSet <int> sectorShips;// this needs to be changed
	

	sf::Vertex sectorBorder[8];

	// will be passed base value
	Sector(int sectorSize, sf::Vector2f position);
	~Sector();
	//std::vector <VariantType> checkColisionInSector();
	void displaySector(sf::RenderWindow& win);

private:

};

//   ===========================================================================================================
class Map
{
public:
	// ids
	int star_id_count = 0;
	int ship_id_count = 0;
	int sector_id_count = 0;

	// Map size
	sf::Vector2f mapSize;
	// vertex array uninicialized
	sf::Vertex mapBorder[8];

	// SETS WITH actual objects
	DynamicSparseSet <StarSystem> stars;
	DynamicSparseSet <SpaceShip> allShips;


	DynamicSparseSet <int> selectedShips;
	DynamicSparseSet <int> selectedStars; 
	// replace selectable objects with dynamicSparse set
	/*std::vector <VariantType> selectedObjects;

	std::vector <VariantType> selectableObjects; */

	// need to remove Variant Type, because I don't need it


	DynamicSparseSet <int> movingShips; // this

	//DynamicSparseSet<SpaceShip*> newAllShips;


	std::vector <std::vector<Sector>> allSectors; // this data type... Should I even consider vector?
	//std::vector <Sector*> activeSectors; // this
	std::vector <int> shipsSectors; // this needs to be changed
	std::vector <int> starsSectors; 

	int sectorSize = 100;

	Map(sf::Vector2f mapSize);
	~Map();
	void Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);
	
	void selectObject(VariantType obj);
	bool checkIfObjectSelected(VariantType obj, bool del=false);

	void addStar(StarSystem star);
	void addShip(SpaceShip ship);
	void selectStar(int star_id);
	void selectShip(int ship_id);
	void destroyShip(int ship_id);

	void cleanSelection();
	//std::vector <VariantType> checkColisionForAllShips();
	void printSectors();
	void determineSectorsForObjects();
	std::tuple<bool, int> collisionBetweenSectors(int ship_id, int sector_id);



private:

};

