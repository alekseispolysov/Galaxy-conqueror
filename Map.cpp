#include "Map.h"
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


SectorBoundary::SectorBoundary(float x, float y, int boundary_size) : x(x), y(y), boundary_size(boundary_size){}

bool SectorBoundary::contains(sf::Vector2f point)
{
	return (point.x >= (x - boundary_size) && point.x <= (x + boundary_size) &&
		point.y >= (y - boundary_size) && point.y <= (y + boundary_size));
}





Sector::Sector(int sectorSize, sf::Vector2f position):sector_size(sectorSize),position(position)
{
	sectorRect.setPosition(position);
	sectorRect.setSize(sf::Vector2f(static_cast<float>(sectorSize), static_cast<float>(sectorSize)));

	// prespsat logiku
	sectorBorder[0] = sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color::White);
	sectorBorder[1] = sf::Vertex(sf::Vector2f(position.x + sectorSize, position.y), sf::Color::White);
	sectorBorder[2] = sf::Vertex(sf::Vector2f(position.x + sectorSize, position.y), sf::Color::White);
	sectorBorder[3] = sf::Vertex(sf::Vector2f(position.x + sectorSize, position.y + sectorSize), sf::Color::White);
	sectorBorder[4] = sf::Vertex(sf::Vector2f(position.x + sectorSize, position.y + sectorSize), sf::Color::White);
	sectorBorder[5] = sf::Vertex(sf::Vector2f(position.x, position.y + sectorSize), sf::Color::White);
	sectorBorder[6] = sf::Vertex(sf::Vector2f(position.x, position.y + sectorSize), sf::Color::White);
	sectorBorder[7] = sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color::White);

}

Sector::~Sector()
{
}

//std::vector<VariantType> Sector::checkColisionInSector()
//{
//	return std::vector<VariantType>();
//}

void Sector::displaySector(sf::RenderWindow& win)
{
	win.draw(sectorBorder, 8, sf::Lines);
}



// hash map functionality
std::pair<int, int> Map::getCell(const sf::Vector2f position)
{
	int gridX = static_cast<int>(std::floor(position.x / cellSize));
	int gridY = static_cast<int>(std::floor(position.y / cellSize));
	return std::make_pair(gridX, gridY);
}

void Map::insertIntoHashMap(int objectID, sf::Vector2f position)
{
	std::pair<int, int> cell = getCell(position);
	grid[cell].push_back(objectID);
}

void Map::removeFromHashMap(int objectID, sf::Vector2f position)
{
	std::pair<int, int> cell = getCell(position);
	std::vector<int>& cellObjects = grid[cell];
	cellObjects.erase(std::remove(cellObjects.begin(), cellObjects.end(), objectID), cellObjects.end());

	if (cellObjects.empty()) {
		grid.erase(cell);
	}
}


void Map::visualizeHashMapFill(sf::RenderWindow& win)
{
	// Iterate through all the cells in the unordered map
	for (const auto& pair : grid)
	{
		// Extract the grid cell coordinates
		std::pair<int, int> cell = pair.first;

		// Convert grid cell coordinates back to world position
		sf::Vector2f worldPosition = sf::Vector2f(cell.first * cellSize, cell.second * cellSize);

		// Draw a small dot for visualization
		sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
		rect.setFillColor(sf::Color::Red);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(-1.0f);
		rect.setPosition(worldPosition.x, worldPosition.y);
		win.draw(rect);
	}
}

std::vector<std::pair<int, int>> Map::getOccupiedCells(sf::Vector2f position, sf::Vector2f size)
{
	 std::vector<std::pair<int, int>> occupiedCells;

	 int minX = static_cast<int>(std::floor((position.x - (size.x/2)) / cellSize));
	 int maxX = static_cast<int>(std::floor(((position.x - (size.x / 2) + size.x)) / cellSize));
	 int minY = static_cast<int>(std::floor((position.y - (size.y / 2)) / cellSize));
	 int maxY = static_cast<int>(std::floor(((position.y - (size.y / 2) + size.y)) / cellSize));

	 for (int x = minX; x <= maxX; ++x) {
		 for (int y = minY; y <= maxY; ++y) {
			 occupiedCells.emplace_back(x, y);
		 }
	 }

	 return occupiedCells;
}

void Map::updateObjectPosition(int objectID, sf::Vector2f oldPosition, sf::Vector2f newPosition)
{
	auto oldCells = getOccupiedCells(oldPosition, allShips.get(objectID).spriteSize);
	auto newCells = getOccupiedCells(newPosition, allShips.get(objectID).spriteSize);

	if (oldCells == newCells) return; // Skip if no grid cell change.

	// Remove from old cells
	for (const auto& cell : oldCells) {
		auto& objects = grid[cell];
		objects.erase(std::remove(objects.begin(), objects.end(), objectID), objects.end());
		if (objects.empty()) {
			grid.erase(cell);
		}
	}

	// Add to new cells
	for (const auto& cell : newCells) {
		grid[cell].push_back(objectID);
	}
}

//void Map::visualizeHashMapFill(sf::RenderWindow& win)
//{
//	for (size_t i = 0; i < activeCells.size(); ++i)
//	{
//		sf::CircleShape dot(2);
//		dot.setFillColor(sf::Color::Red);
//		dot.setPosition(activeCells[i].x, activeCells[i].y);
//		win.draw(dot);
//	}
//}

// get nearby objects
DynamicSparseSet<int> Map::queryHashMap(sf::Vector2f position, float radius, int originID) {
	//std::vector<int> nearbyObjects;
	DynamicSparseSet<int> nearbyObjects;

	// because it is int, thats why I am flooring, radius of how much grid cells in radius I need to detect in position
	// obivously rectangle A B C D
	int minX = static_cast<int>(std::floor((position.x - radius) / cellSize));
	int maxX = static_cast<int>(std::floor((position.x + radius) / cellSize));
	int minY = static_cast<int>(std::floor((position.y - radius) / cellSize));
	int maxY = static_cast<int>(std::floor((position.y + radius) / cellSize));

	// square
	float radiusSquared = radius * radius;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			// for all intire square I check every position
			std::pair<int, int> cell = std::make_pair(x, y);
			// if we call grid method find and it is not grid int, meaning it is present inside
			if (grid.find(cell) != grid.end()) {
				// for list in grid cell
				for (int& objectID : grid[cell]) {
					// get object in position
					sf::Vector2f objectPosition = getObjectPosition(objectID); // Fetch object position.
					float distanceSquared = (position.x - objectPosition.x) * (position.x - objectPosition.x) +
						(position.y - objectPosition.y) * (position.y - objectPosition.y);
					if (distanceSquared <= radiusSquared) {
						// I don't understand, why is that thing working like that?
						// if distant, based on position, that we are detecting
						if (originID != objectID) {
							if (!nearbyObjects.contains(objectID)) {
								nearbyObjects.insert(objectID, objectID);

							}
						}
					}
				}
			}
		}
	}

	return nearbyObjects;
}

void Map::clearHashMap()
{
	grid.clear();
}

void Map::drawGrid()
{

}



int Map::getTypeObject(int ObjectID)
{
	// stars are 1, ships are 2
	if (allShips.contains(ObjectID)) {
		return 2;
	}
	else if (stars.contains(ObjectID)) {
		return 1;
	}



	return 0;
}

sf::Vector2f Map::getObjectPosition(int objectID)
{
	// try to access it in
	if (allShips.contains(objectID)) {
		return allShips.get(objectID).pos;
	}
	else if (stars.contains(objectID)) {
		return sf::Vector2f(stars.get(objectID).starXposMap, stars.get(objectID).starYposMap);
	}
	else {
		throw std::runtime_error("Object with this ID does not exists");
		return sf::Vector2f(0, 0);
	}


}

Map::Map(sf::Vector2f mapSize):mapSize(mapSize){
	
	mapSize = mapSize;
	mapBorder[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White);
	mapBorder[1] = sf::Vertex(sf::Vector2f(mapSize.x, 0), sf::Color::White);
	mapBorder[2] = sf::Vertex(sf::Vector2f(mapSize.x, 0), sf::Color::White);
	mapBorder[3] = sf::Vertex(sf::Vector2f(mapSize.x, mapSize.y), sf::Color::White); 
	mapBorder[4] = sf::Vertex(sf::Vector2f(mapSize.x, mapSize.y), sf::Color::White); 
	mapBorder[5] = sf::Vertex(sf::Vector2f(0, mapSize.y), sf::Color::White); 
	mapBorder[6] = sf::Vertex(sf::Vector2f(0, mapSize.y), sf::Color::White);
	mapBorder[7] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White); 

	// populate 2 dimensional vector
	float sectorVectorSizeX = mapSize.x / sectorSize; //8
	float sectorVectorSizeY = mapSize.y / sectorSize; //6

	// Resize the outer vector (size for sectors along X)
	//allSectors.resize(sectorVectorSizeX);

	for (size_t i = 0; i < sectorVectorSizeX; ++i)
	{
		// Resize inner vector for each row (sectorVectorSizeY)
		DynamicSparseSet<Sector> dynamicSparseSet;
		allSectors.insert(i, dynamicSparseSet);
		//allSectors.get(i).clear(); // Clear the vector to ensure it's empty before adding new elements
		//allSectors.get(i).reserve(sectorVectorSizeY); // Optionally reserve space for efficiency

		for (size_t j = 0; j < sectorVectorSizeY; ++j) {
			Sector s = Sector(sectorSize, sf::Vector2f(i * sectorSize, j*sectorSize));
			s.id = sector_id_count;
			sector_id_count += 1;
			allSectors.get(i).insert(j, s); // Emplace new Sector directly
			std::cout << "Created new sector at position (" << i * sectorSize << ", " << j * sectorSize << ")" << std::endl;
		}
	}

}

Map::~Map() {

	/*for (StarSystem* star : stars) {
		delete star;
	}
	stars.clear();*/


}

void Map::Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor)
{
	// display background

	// display all other objects

	// display all partiles

	// display all connections
	for (size_t i = 0; i < stars.size(); ++i) {
		int star_id = stars.getElements()[i].id;
		stars.get(star_id).DrawAllConnections(win);
	}
	// display all Stars
	for (size_t i = 0; i < stars.size(); ++i) {
		int star_id = stars.getElements()[i].id;
		stars.get(star_id).Display(win, shader, zoomFactor);
	}
	// draw all ships
	visualizeHashMapFill(win);
	// draw map Borders
	win.draw(mapBorder, 8, sf::Lines);

	// display UI? Is it better display it only if player object is present and playing is assigned? (spectator mode)

}

// rewrite this funciton into select ship and select star
void Map::selectObject(int id, std::string type)
{
	if (type == "star") {
		selectedStars.insert(id, id);
	}
	if (type == "ship") {
		selectedShips.insert(id, id);
	}
	//selectedObjects.insert(obj);

}

//bool Map::checkIfObjectSelected(VariantType obj, bool del)
//{
//	return false;
//	for (size_t j = 0; j < 4; ++j) { // *==============================================!
//		// if this ship, star or other object is present in this array ( passed as argument ), then remove it and return true;
//		if (true) { // obj == selectedObjects[j]
//			// delete object while iterating if del is true;
//			if (del) {
//				//selectedObjects.erase(std::remove(selectedObjects.begin(), selectedObjects.end(), obj), selectedObjects.end());
//			}
//			return true;
//		}
//	}
//	return false;
//}

void Map::addStar(StarSystem star)
{
	
	std::cout << "Added star with star id: " << unique_object_id << std::endl;
	// redo this part of code, star Id either should be created independently, or otherwise
	
	star.id = unique_object_id;
	stars.insert(unique_object_id, star);
	insertIntoHashMap(star.id, sf::Vector2f(star.starXposMap, star.starYposMap));
	unique_object_id += 1;
	star_id_count += 1;

	// inserting ship into multiple cells
	auto occupiedCells = getOccupiedCells(sf::Vector2f(star.starXposMap, star.starYposMap), sf::Vector2f(star.radius, star.radius));
	for (const auto& cell : occupiedCells) {
		grid[cell].push_back(star.id);
	}

	//selectableObjects.push_back(star);
	// determine sector, when added


}

// typed functions????????
void Map::selectStar(int star_id)
{
	selectedStars.insert(star_id, star_id);
}

void Map::selectShip(int ship_id)
{
	selectedShips.insert(ship_id, ship_id);
	
}

void Map::cleanSelection() {
	// cleaning stars selection?

	// cleaning selection of ships

	selectedShips.clear();
	selectedStars.clear();
	//selectedObjects.clear(); #todo
}

void Map::addShip(SpaceShip ship)
{
	// get all ship size and on this make id
	//ship->id = newAllShips.size();

	//newAllShips.insert(ship.id, ship);
	//newAllShips.print();

	ship.id = unique_object_id;
	unique_object_id += 1;
	std::cout << "inserting ship, id of the ship: " << ship.id << std::endl;
	allShips.insert(ship.id, ship);
	insertIntoHashMap(ship.id, ship.pos);

	// inserting ship into multiple cells
	auto occupiedCells = getOccupiedCells(ship.pos, ship.spriteSize);
	for (size_t i = 0; i < occupiedCells.size(); i++)
	{
		//ship.currentlyOccupiedCells
		ship.currentlyOccupiedCells.push_back(occupiedCells[i]);
	}
	for (const auto& cell : occupiedCells) {
		grid[cell].push_back(ship.id);
	}



	ship_id_count += 1;
	//selectableObjects.push_back(ship);
	//std::cout << "Added ship into variant array: " << ship << std::endl;
}

// it will acept star id, if ship is in the star object
void Map::destroyShip(int ship_id)
{
	// destroying ship functionality
	// remove it from all ships

	//SpaceShip& ship = allShips.get(ship_id);
	auto occupiedCells = getOccupiedCells(allShips.get(ship_id).pos, allShips.get(ship_id).spriteSize);

	for (const auto& cell : occupiedCells) {
		auto& objects = grid[cell];
		objects.erase(std::remove(objects.begin(), objects.end(), ship_id), objects.end());
		if (objects.empty()) {
			grid.erase(cell);
		}
	}

	allShips.erase(ship_id); // this is not working
	// if it is in selected ships, remove it from selected ships
	if (selectedShips.contains(ship_id)) {
		selectedShips.erase(ship_id);
	}
	// if it is in moving objects, remove it from moving objects
	if (movingShips.contains(ship_id)) {
		movingShips.erase(ship_id);
	}
	// if it is in nearbyObjects, remove it from nearby objects
	// don't forget to remove them from the grid
	std::cout << "deletion complete" << std::endl;

	// remove it from star, if necesery

}

//std::vector<VariantType> Map::checkColisionForAllShips()
//{
//
//	for (size_t i = 0; i < activeSectors.size(); ++i)
//	{
//
//	}
//
//	return std::vector<VariantType>();
//}

void Map::printSectors()
{
	//std::cout << "This is one of the sectors" << allSectors[3][2] << std::endl;
	std::cout << "This function, should print sector, but func is changed and function is not working" << std::endl;
}

void Map::determineSectorsForObjects()
{
	int xSector;
	int ySector;
	// determine sectors for ships
	// possibly broken, if I want implement deletion
	for (size_t i = 0; i < allShips.size(); ++i)
	{
		int sectorX = (allShips.get(i).pos.x) / sectorSize;
		int sectorY = (allShips.get(i).pos.y) / sectorSize;
		//allSectors.get(sectorX).get(sectorY).sectorShips;
		allSectors.get(sectorX).get(sectorY).sectorShips.insert(allShips.get(i).id, allShips.get(i).id);
		allShips.get(i).sector[0] = sectorX;
		allShips.get(i).sector[1] = sectorY;
		std::cout << "there are coordinates for the sector, that is currectly processed: " << sectorX << ", " << sectorY << std::endl;
		//std::cout << "to sector object on pointer: " << s << ", has been added new ship, that has pointer: " << ship << std::endl;
	}
	// determine sectors for stars
	for (size_t i = 0; i < stars.size(); ++i)
	{
		for (size_t z = 0; z < mapSize.x / sectorSize; ++z)
		{
			for (size_t j = 0; j < mapSize.y / sectorSize; ++j)
			{
				// check for collisions with every sector -> if sector's x, y collides with star rect, then add it to the sector
				//std::cout << "this parametrs are errorfuls: " << z << ", " << j << std::endl;
				//sector* s = allsectors[z][j]; // error
				// get star object
				if (stars.get(i).star.getGlobalBounds().intersects(allSectors.get(z).get(j).sectorRect.getGlobalBounds())) { // colides
					// add star to sector
					// then add star to sector
					starsSectors.push_back(allSectors.get(z).get(j).id); // debug purposes
					allSectors.get(z).get(j).sectorStars.insert(stars.get(i).id, stars.get(i).id);
					std::cout << "to sector: " << allSectors.get(z).get(j).id << " has been added star" << stars.get(i).id << std::endl;
				}
			}
		}
	}


	// possibly broken too
	//for (size_t i = 0; i < selectableobjects.size(); ++i)
	//{
	//	/*int xsector;
	//	int ysector;*/
	//	// get variant object here
	//	//if (std::holds_alternative<spaceship*>(selectableobjects[i])) {
	//	//	spaceship* ship = std::get<spaceship*>(selectableobjects[i]);
	//	//	// get center of the ship and relatively to its center add it to right sector
	//	//	// write this logic and after this try to understand why we should write all variables in private?
	//	//	// get variant type?
	//	//	int sectorx = (ship->pos.x) / sectorsize;
	//	//	std::cout << ship->shiporigin.x + ship->pos.x << std::endl;
	//	//	int sectory = (ship->pos.y) / sectorsize;
	//	//	sector* s = allsectors[sectorx][sectory];
	//	//	s->sectorships.push_back(ship);
	//	//	ship->sector[0] = sectorx;
	//	//	ship->sector[1] = sectory;
	//	//	std::cout << "there are coordinates for the sector, that is currectly processed: " << sectorx << ", " << sectory << std::endl;
	//	//	std::cout << "to sector object on pointer: " << s << ", has been added new ship, that has pointer: " << ship << std::endl;
	//	//	// change active sectors
	//	//	//activesectors.resize();
	//	//	//activesectors[i].resize(sectorvectorsizey);
	//	//	//activesectors.push_back(s);

	//	//}
	//	else if (std::holds_alternative<starsystem*>(selectableobjects[i])) {
	//		starsystem* star = std::get<starsystem*>(selectableobjects[i]);
	//		// launch colision only for stars
	//		// here i need to iterate throw all of the sectors and understand, which sector is it
	//		// i am doing it wrong, because it is not okay value
	//		// this algorithm is slow. i need to get neigboor sectors, depended on size of the star and size of the sectors (for loading faster)
	//		std::cout << mapsize.x / sectorsize << std::endl;
	//		for (size_t z = 0; z < mapsize.x / sectorsize; ++z)
	//		{
	//			for (size_t j = 0; j < mapsize.y / sectorsize; ++j)
	//			{
	//				// check for collisions with every sector -> if sector's x, y collides with star rect, then add it to the sector
	//				//std::cout << "this parametrs are errorfuls: " << z << ", " << j << std::endl;
	//				sector* s = allsectors[z][j]; // error
	//				if (star->star.getglobalbounds().intersects(s->sectorrect.getglobalbounds())) { // colides
	//					// add star to sector
	//					// then add star to sector
	//					starssectors.push_back(s); // debug purposes
	//					s->sectorstars.push_back(star);
	//					std::cout << "to sector: " << s << " has been added star" << star << std::endl;
	//				}
	//			}
	//		}


	//	}

	//}
}

std::tuple<bool, int> Map::collisionBetweenSectors(int ship_id, int sector_id)
{

	// ooou I need DynamicSparseSet!!!
	//for (size_t j = 0; j < s->sectorShips.size(); ++j)
	//{
		// collision between ships close sectors
		/*if (ship->ColisionCheck(s->sectorShips[j])) {
			return std::make_tuple(true, ship);
		}*/
	//}
	
	// if return -1 - did not found an id
	return std::make_tuple(false, -1);
}

//void Map::removeShip(SpaceShip* ship)
//{
	
	// deleting ship from vector all ships
	// all.erase(std::remove(.begin(), .end(), ship), .end());
	// erase from selectable objects

	// erase from selected objects

	// erase ship from flying ships

	// try to remove it from stars

	// erase from sector
	//allSectors[ship->sector[0]][ship->sector[1]];
	// erase from all ships
	//allShips.erase(std::remove(allShips.begin(), allShips.end(), ship->id), allShips.end()); // this line is a problem

	//// deleting ship
	//delete ship;


//}

size_t PairHash::operator()(const std::pair<int, int>& p) const
{
	return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
}


