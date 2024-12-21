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

//class DynamicSparseSet;

//define vairants for dynamic arrays
using VariantType = std::variant<SpaceShip*, StarSystem*>;




class PairHash {
public:
	size_t operator()(const std::pair<int, int>& p) const;
};


// boundary class to represent rectangle region
class SectorBoundary {
public:
	float x, y;
	// since boundary is rectangle, we will not use size
	int boundary_size;

	SectorBoundary(float x, float y, int boundary_size);
	~SectorBoundary();

	// check, if rectangle, contains point
	bool contains(sf::Vector2f point);
	bool intersects(); // ideally object itself
};


// sector is quad tree
class Sector {
public:
	int id = 0;
	sf::Vector2f position;
	float witdth;
	float height;
	int sector_size;
	sf::RectangleShape sectorRect;
	
	DynamicSparseSet <int> sectorStars;
	DynamicSparseSet <int> sectorShips; 
	

	// child nodes:


	sf::Vertex sectorBorder[8];

	// will be passed base value
	Sector(int sectorSize, sf::Vector2f position);
	~Sector();
	//std::vector <VariantType> checkColisionInSector();
	void displaySector(sf::RenderWindow& win);
	void subdivide();


private:

};

//   ===========================================================================================================
class Map
{
private:

	// spatial hashing functionality
	float cellSize = 15.0f;
	std::unordered_map<std::pair<int, int>, std::vector<int>, PairHash> grid;
	std::pair<int, int> getCell(const sf::Vector2f position);
	// some dynamic sparse set, to hold all active points in order to draw them
	std::vector<sf::Vector2f> activeCells;

public:
	// spatial hashing functionality
	void insertIntoHashMap(int objectID, sf::Vector2f position);
	void removeFromHashMap(int objectID, sf::Vector2f position);

	void visualizeHashMapFill(sf::RenderWindow& win);
	std::vector<std::pair<int, int>> getOccupiedCells(sf::Vector2f position, sf::Vector2f size);
	void updateObjectPosition(int objectID, sf::Vector2f oldPosition, sf::Vector2f newPosition);
	DynamicSparseSet<int> queryHashMap(sf::Vector2f position, float radius, int originId); 
	void clearHashMap();
	void drawGrid();
	
	int getTypeObject(int ObjectID);
	sf::Vector2f getObjectPosition(int objectID);

	// ids
	int unique_object_id = 0;
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


	DynamicSparseSet<DynamicSparseSet<Sector>> allSectors; // this data type... Should I even consider vector?
	// I DONT NEED TO USE SECTORS, when I am using hash map!


	//std::vector <Sector*> activeSectors; // this
	std::vector <int> shipsSectors; // this needs to be changed
	std::vector <int> starsSectors; 

	int sectorSize = 100;

	Map(sf::Vector2f mapSize);
	~Map();
	void Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor);
	
	//void selectObject(VariantType obj);
	void selectObject(int id, std::string type);
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




};

