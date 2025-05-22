#pragma once
#include "StarSystem.h"
#include "SpaceShip.h"
#include "Particle.h"
#include <iostream>
#include <any>
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
	std::pair<int, int> getCell(const sf::Vector2f position);
	// some dynamic sparse set, to hold all active points in order to draw them
	std::vector<sf::Vector2f> activeCells;

public:
	std::unordered_map<std::pair<int, int>, std::vector<int>, PairHash> grid;
	// spatial hashing functionality
	void insertIntoHashMap(int objectID, sf::Vector2f position);
	void removeFromHashMap(int objectID, sf::Vector2f position);

	void visualizeHashMapFill(sf::RenderWindow& win);
	std::vector<std::pair<int, int>> getOccupiedCells(sf::Vector2f position, sf::Vector2f size);
	std::vector<std::pair<int, int>> getOccupiedCellsForCircle(sf::Vector2f position, float radius);
	std::vector<std::pair<int, int>> getFilledCells(sf::Vector2f position, sf::Vector2f size);
	void updateObjectPosition(int objectID, sf::Vector2f oldPosition, sf::Vector2f newPosition);
	DynamicSparseSet<int> queryHashMap(sf::Vector2f position, float radius, int originId);
	int queryHashMapFirst(sf::Vector2f position, float radius, int detectType=0);
	
	void clearHashMap();
	void drawGrid();
	
	int getTypeObject(int ObjectID);

	std::any getObject(int ObjectID);

	sf::Vector2f getObjectPosition(int objectID);

	// ids
	int unique_object_id = 0;
	int star_id_count = 0;
	int ship_id_count = 0;
	int sector_id_count = 0;

	int debug = 1;

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
	// selection groups
	// group seletion
	DynamicSparseSet<int> group1;
	DynamicSparseSet<int> group2;
	DynamicSparseSet<int> group3;
	DynamicSparseSet<int> group4;
	DynamicSparseSet<int> group5;
	DynamicSparseSet<int> group6;
	DynamicSparseSet<int> group7;
	DynamicSparseSet<int> group8;
	DynamicSparseSet<int> group9;
	DynamicSparseSet<int> group0;

	int hoveredObject = -1;
	sf::Color originHoverColor = sf::Color(255,255,255,255);

	

	DynamicSparseSet<Particle> particleManager;
	int particle_id_counter = 0;

	DynamicSparseSet <int> movingShips; // thisssss

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

	void addStar(StarSystem& star);
	void addParticle(Particle& particle); 
	void addShip(SpaceShip ship); 
	void selectStar(int star_id);
	void selectShip(int ship_id);
	void destroyShip(int ship_id);
	void connect_stars(int origin_star, int other_star);

	void cleanSelection();
	//std::vector <VariantType> checkColisionForAllShips();
	bool colisionCheck(int originID, int otherID);
	bool colisionPointCheck(int originID, sf::Vector2f& position);
	void printSectors();
	void determineSectorsForObjects();
	std::tuple<bool, int> collisionBetweenSectors(int ship_id, int sector_id);

	// UI function
	// I think this function needs to be in main file, because it does not related to map
	void updateSelectionUI(DynamicSparseSet<int> objects, tgui::ScrollablePanel::Ptr selectionPanel, tgui::Texture tguishipTextureShared, tgui::Texture tguiSharedTexture);

	void updateSelectionGroup(int groupNumber, DynamicSparseSet<int> selectedObjects);
	DynamicSparseSet<int> selectGroup(int groupNumber); 

	// path finding
	float calculate_distance(sf::Vector2f A, sf::Vector2f B);
	DynamicSparseSet<sf::Vector2f> aPosNodeFinder(int startNodeId, int endNodeId, sf::Vector2f startPoint, sf::Vector2f endPoint);
	std::vector<int> reconstruct_path(std::unordered_map<int, int>& cameFrom, int current);
	std::vector<int> astar_pathfinding(int startId, int goalId);
};

