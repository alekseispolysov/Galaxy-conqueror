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
		stars.get(i).DrawAllConnections(win);
	}
	// display all Stars
	for (size_t i = 0; i < stars.size(); ++i) {
		stars.get(i).Display(win, shader, zoomFactor);
	}
	// draw all ships

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
	
	std::cout << "Added star with star id: " << star_id_count << std::endl;
	star.id = star_id_count;
	stars.insert(star_id_count, star);
	
	star_id_count += 1;
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

	allShips.insert(ship_id_count, ship);
	ship_id_count += 1;
	//selectableObjects.push_back(ship);
	//std::cout << "Added ship into variant array: " << ship << std::endl;
}

void Map::destroyShip(int ship_id)
{


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

