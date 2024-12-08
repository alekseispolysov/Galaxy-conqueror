#include "Map.h"
#include "StarSystem.h"
#include "SpaceShip.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>
#include <algorithm>



Sector::Sector(int sectorSize, sf::Vector2f position):sector_size(sectorSize),position(position)
{
	sectorRect.setPosition(position);
	sectorRect.setSize(sf::Vector2f(static_cast<float>(sectorSize), static_cast<float>(sectorSize)));
}

Sector::~Sector()
{
}

std::vector<VariantType> Sector::checkColisionInSector()
{
	return std::vector<VariantType>();
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

	allSectors.resize(sectorVectorSizeX);
	activeSectors.resize(sectorVectorSizeX);

	for (size_t i = 0; i < sectorVectorSizeX; ++i)
	{
		allSectors[i].resize(sectorVectorSizeY);
		//activeSectors[i].resize(sectorVectorSizeY);

		for (size_t j = 0; j < sectorVectorSizeY; ++j) {

			allSectors[i][j] = new Sector(sectorSize, sf::Vector2f(i*sectorSize, j*sectorSize)); // push position too
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
		stars[i]->DrawAllConnections(win);
	}
	// display all Stars
	for (size_t i = 0; i < stars.size(); ++i) {
		stars[i]->Display(win, shader, zoomFactor);
	}
	// draw all ships

	// draw map Borders
	win.draw(mapBorder, 8, sf::Lines);

	// display UI? Is it better display it only if player object is present and playing is assigned? (spectator mode)

}

void Map::selectObject(VariantType obj)
{
	selectedObjects.push_back(obj);
}

bool Map::checkIfObjectSelected(VariantType obj, bool del)
{
	for (size_t j = 0; j < selectedObjects.size(); ++j) {
		// if this ship, star or other object is present in this array ( passed as argument ), then remove it and return true;
		if (obj == selectedObjects[j]) {
			// delete object while iterating if del is true;
			if (del) {
				selectedObjects.erase(std::remove(selectedObjects.begin(), selectedObjects.end(), obj), selectedObjects.end());
			}
			return true;
		}
	}
	return false;
}

void Map::addStar(StarSystem* star)
{
	stars.push_back(star);
	selectableObjects.push_back(star);
	// determine sector, when added

}

// typed functions????????
void Map::selectStar(StarSystem* star)
{
	selectedStars.push_back(star);
}

void Map::selectShip(SpaceShip* ship)
{
	selectedShips.push_back(ship);
}

void Map::cleanSelection() {
	// cleaning stars selection?

	// cleaning selection of ships

	selectedShips.clear();
	selectedStars.clear();
	selectedObjects.clear();
}

void Map::addShip(SpaceShip* ship)
{
	allShips.push_back(ship);
	selectableObjects.push_back(ship);
	std::cout << "Added ship into variant array: " << ship << std::endl;
}

void Map::destroyShip(SpaceShip* ship)
{
}

std::vector<VariantType> Map::checkColisionForAllShips()
{

	for (size_t i = 0; i < activeSectors.size(); ++i)
	{

	}

	return std::vector<VariantType>();
}

void Map::printSectors()
{
	std::cout << "This is one of the sectors" << allSectors[3][2] << std::endl;
}

void Map::determineSectorsForObjects()
{

	for (size_t i = 0; i < selectableObjects.size(); ++i)
	{
		int xSector;
		int ySector;
		// get variant object here
		if (std::holds_alternative<SpaceShip*>(selectableObjects[i])) {
			SpaceShip* ship = std::get<SpaceShip*>(selectableObjects[i]);
			// get center of the ship and relatively to its center add it to right sector
			// write this logic and after this try to understand why we should write all variables in private?
			// GET VARIANT TYPE?
			int sectorX = (ship->shipOrigin.x + ship->pos.x) / sectorSize;
			std::cout << ship->shipOrigin.x + ship->pos.x << std::endl;
			int sectorY = (ship->shipOrigin.y + ship->pos.y) / sectorSize;
			Sector* s = allSectors[sectorX][sectorY];
			s->sectorShips.push_back(ship);
			std::cout << "There are coordinates for the sector, that is currectly processed: " << sectorX << ", " << sectorY << std::endl;
			std::cout << "To sector object on pointer: " << s << ", has been added new ship, that has pointer: " << ship << std::endl;
			// Change active sectors
			//activeSectors.resize();
			//activeSectors[i].resize(sectorVectorSizeY);
			activeSectors.push_back(s);

		}
		else if (std::holds_alternative<StarSystem*>(selectableObjects[i])) {
			StarSystem* star = std::get<StarSystem*>(selectableObjects[i]);
			// launch colision only for stars
			// Here I need to iterate throw all of the sectors and understand, which sector is it
			// I am doing it wrong, because it is not okay value
			std::cout << mapSize.x / sectorSize << std::endl;
			for (size_t z = 0; z < mapSize.x / sectorSize; ++z)
			{
				for (size_t j = 0; j < mapSize.y / sectorSize; ++j)
				{
					// check for collisions with every sector -> If sector's X, Y collides with star rect, then add it to the sector
					std::cout << "this parametrs are errorfuls: " << z << ", " << j << std::endl;
					Sector* s = allSectors[z][j]; // error
					if (star->star.getGlobalBounds().intersects(s->sectorRect.getGlobalBounds())) { // colides
						// add star to sector
						// then add star to sector

						s->sectorStars.push_back(star);
						std::cout << "To sector: " << s << " has been added star" << star << std::endl;
					}
				}
			}


		}

	}
}

