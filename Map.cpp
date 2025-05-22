#include "Map.h"
#include "StarSystem.h"
#include "Particle.h"
#include "SpaceShip.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <any>
#include <variant>
#include <algorithm>
#include <tuple>
#include "DynamicSparseSet.h"
#include <cassert>
#include <unordered_map>
#include <cmath>
#include <utility> 
#include <vector>
#include <queue>
#include <cassert>



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
// for rectangle shapes
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
// for circular shapes:
std::vector<std::pair<int, int>> Map::getOccupiedCellsForCircle(sf::Vector2f position, float radius) {
	std::vector<std::pair<int, int>> occupiedCells;

	// Calculate the bounding box of the circle
	int minX = static_cast<int>(std::floor((position.x - radius) / cellSize));
	int maxX = static_cast<int>(std::floor((position.x + radius) / cellSize));
	int minY = static_cast<int>(std::floor((position.y - radius) / cellSize));
	int maxY = static_cast<int>(std::floor((position.y + radius) / cellSize));

	// Iterate through all grid cells in the bounding box
	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			// Check if the grid cell intersects with the circle
			sf::Vector2f cellCenter((x + 0.5f) * cellSize, (y + 0.5f) * cellSize); // Center of the cell
			float dx = position.x - cellCenter.x;
			float dy = position.y - cellCenter.y;

			// Check if the cell is within the circle's radius
			float distanceSquared = dx * dx + dy * dy;
			if (distanceSquared <= radius * radius) {
				occupiedCells.emplace_back(x, y);
			}
		}
	}

	return occupiedCells;
}


std::vector<std::pair<int, int>> Map::getFilledCells(sf::Vector2f position, sf::Vector2f size)
{
	std::vector<std::pair<int, int>> filledCells;

	int minX = static_cast<int>(std::floor(position.x / cellSize));
	int maxX = static_cast<int>(std::floor((position.x + size.x) / cellSize));
	int minY = static_cast<int>(std::floor(position.y / cellSize));
	int maxY = static_cast<int>(std::floor((position.y + size.y) / cellSize));



	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			;
			if (grid.find({x, y}) != grid.end()) {

				filledCells.emplace_back(x, y);
				
			}
			
		}

		
	}

	return filledCells;

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

// get nearby objects
DynamicSparseSet<int> Map::queryHashMap(sf::Vector2f position, float radius, int originID) {
	DynamicSparseSet<int> nearbyObjects;

	int minX = static_cast<int>(std::floor((position.x - radius) / cellSize));
	int maxX = static_cast<int>(std::floor((position.x + radius) / cellSize));
	int minY = static_cast<int>(std::floor((position.y - radius) / cellSize));
	int maxY = static_cast<int>(std::floor((position.y + radius) / cellSize));

	
	float radiusSquared = radius * radius;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			std::pair<int, int> cell = std::make_pair(x, y);
			if (grid.find(cell) != grid.end()) {
				for (int& objectID : grid[cell]) {
					
							if (!nearbyObjects.contains(objectID)) {
								nearbyObjects.insert(objectID, objectID);

							}
						
				}
			}
		}
	}

	return nearbyObjects;
}

int Map::queryHashMapFirst(sf::Vector2f position, float radius, int detectType)
{

	DynamicSparseSet<int> nearbyObjects;

	int minX = static_cast<int>(std::floor((position.x - radius) / cellSize));
	int maxX = static_cast<int>(std::floor((position.x + radius) / cellSize));
	int minY = static_cast<int>(std::floor((position.y - radius) / cellSize));
	int maxY = static_cast<int>(std::floor((position.y + radius) / cellSize));

	float radiusSquared = radius * radius;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			std::pair<int, int> cell = std::make_pair(x, y);
			if (grid.find(cell) != grid.end()) {
				// for list in grid cell
				for (int& objectID : grid[cell]) {
					
					if (!nearbyObjects.contains(objectID)) {
						if (detectType != 0) {
							int objectType = getTypeObject(objectID);
							if (objectType == detectType) {
								return objectID;
							}
						}
						else {
							return objectID;
						}
					}
					
				}
			}
		}
	}

	
	return -1;
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
	if (allShips.contains(ObjectID)) {
		return 2;
	}
	else if (stars.contains(ObjectID)) {
		return 1;
	}



	return 0;
}

std::any Map::getObject(int ObjectID)
{
	int objType = getTypeObject(ObjectID);

	if (objType == 1) {
		return stars.get(ObjectID);
	}
	else if (objType == 2) {
		return allShips.get(ObjectID);
	}

	
	throw std::runtime_error("Invalid object type");
}

sf::Vector2f Map::getObjectPosition(int objectID)
{
	
	if (allShips.contains(objectID)) {
		return allShips.get(objectID).pos;
	}
	if (stars.contains(objectID)) {
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

	float sectorVectorSizeX = mapSize.x / sectorSize; //8
	float sectorVectorSizeY = mapSize.y / sectorSize; //6

	
	for (size_t i = 0; i < sectorVectorSizeX; ++i)
	{
		DynamicSparseSet<Sector> dynamicSparseSet;
		allSectors.insert(i, dynamicSparseSet);

		for (size_t j = 0; j < sectorVectorSizeY; ++j) {
			Sector s = Sector(sectorSize, sf::Vector2f(i * sectorSize, j*sectorSize));
			s.id = sector_id_count;
			sector_id_count += 1;
			allSectors.get(i).insert(j, s); 
			std::cout << "Created new sector at position (" << i * sectorSize << ", " << j * sectorSize << ")" << std::endl;
		}
	}

}

Map::~Map() {


}

void Map::Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor)
{
	

	for (size_t i = 0; i < allShips.size(); ++i)
	{
		int ship_id = allShips.getElements()[i].id;
		allShips.get(ship_id).Display(win);
	}

	for (size_t i = 0; i < stars.size(); ++i) {
		int star_id = stars.getElements()[i].id;
		stars.get(star_id).Display(win, shader, zoomFactor);
	}

	
	

	for (size_t i = 0; i < stars.size(); ++i) {
		int star_id = stars.getElements()[i].id;
		
		for (size_t j = 0; j < stars.get(star_id).connections.size(); j++)
		{
			
			
			int other_star_id = stars.get(star_id).connections.getElements()[j];
			
				sf::Vertex line[] = {
				sf::Vertex(sf::Vector2f(stars.get(star_id).starXposMap,stars.get(star_id).starYposMap), sf::Color(stars.get(star_id).starColor.r, stars.get(star_id).starColor.g, stars.get(star_id).starColor.b, 55.0f)),
				sf::Vertex(sf::Vector2f(stars.get(other_star_id).starXposMap, stars.get(other_star_id).starYposMap), sf::Color(stars.get(other_star_id).starColor.r, stars.get(other_star_id).starColor.g, stars.get(other_star_id).starColor.b, 55.0f)),
				};
				win.draw(line, 2, sf::Lines);
			
		}
	}
	

	// DEBUG HASHMAP
	//visualizeHashMapFill(win);
	// draw map Borders
	win.draw(mapBorder, 8, sf::Lines);

	
}
void Map::selectObject(int id, std::string type)
{
	if (type == "star") {
		selectedStars.insert(id, id);
	}
	if (type == "ship") {
		selectedShips.insert(id, id);
	}
	
}



void Map::addStar(StarSystem& star) 
{
	
	std::cout << "Added star with star id: " << unique_object_id << std::endl;
	
	star.id = unique_object_id;
	stars.insert(unique_object_id, star);
	insertIntoHashMap(star.id, sf::Vector2f(star.starXposMap, star.starYposMap));
	unique_object_id += 1;
	star_id_count += 1;

	auto occupiedCells = getOccupiedCellsForCircle(sf::Vector2f(star.starXposMap, star.starYposMap), star.radius);
	for (const auto& cell : occupiedCells) {
		grid[cell].push_back(star.id);
	}


}

void Map::addParticle(Particle& particle)
{
	particle.id = particle_id_counter;
	particleManager.insert(particle.id, particle);
	particle_id_counter += 1;


}

void Map::selectStar(int star_id)
{
	selectedStars.insert(star_id, star_id);
}

void Map::selectShip(int ship_id)
{
	selectedShips.insert(ship_id, ship_id);
	
}

void Map::cleanSelection() {
	
	selectedShips.clear();
	selectedStars.clear();
	
}

void Map::addShip(SpaceShip ship)
{
	
	ship.id = unique_object_id;
	unique_object_id += 1;
	std::cout << "inserting ship, id of the ship: " << ship.id << std::endl;
	allShips.insert(ship.id, ship);
	insertIntoHashMap(ship.id, ship.pos);

	
	auto occupiedCells = getOccupiedCells(ship.pos, ship.spriteSize);
	for (size_t i = 0; i < occupiedCells.size(); i++)
	{
		ship.currentlyOccupiedCells.push_back(occupiedCells[i]);
	}
	for (const auto& cell : occupiedCells) {
		grid[cell].push_back(ship.id);
	}



	ship_id_count += 1;
}

// destroy ship cleanup functino
void Map::destroyShip(int ship_id)
{
	auto occupiedCells = getOccupiedCells(allShips.get(ship_id).pos, allShips.get(ship_id).spriteSize);

	for (const auto& cell : occupiedCells) {
		auto& objects = grid[cell];
		objects.erase(std::remove(objects.begin(), objects.end(), ship_id), objects.end());
		if (objects.empty()) {
			grid.erase(cell);
		}
	}

	allShips.erase(ship_id);
	if (selectedShips.contains(ship_id)) {
		selectedShips.erase(ship_id);
	}
	
	if (movingShips.contains(ship_id)) {
		movingShips.erase(ship_id);
	}
	std::cout << "deletion complete" << std::endl;

}

void Map::connect_stars(int origin_star, int other_star)
{
	stars.get(origin_star).connections.insert(other_star, other_star);
	stars.get(other_star).connections.insert(origin_star, origin_star);
}


bool Map::colisionCheck(int originID, int otherID)
{
	if (allShips.contains(otherID)) {
		return allShips.get(originID).shipSprite.getGlobalBounds().intersects(allShips.get(otherID).shipSprite.getGlobalBounds());
	}
	if (stars.contains(otherID)) {
		auto& star = stars.get(otherID);
		auto& ship = allShips.get(originID);

		float circleX = star.star.getPosition().x + star.star.getRadius();
		float circleY = star.star.getPosition().y + star.star.getRadius();
		float radius = star.star.getRadius();

		float rectX = ship.shipSprite.getGlobalBounds().left;
		float rectY = ship.shipSprite.getGlobalBounds().top;
		float rectWidth = ship.shipSprite.getGlobalBounds().width;
		float rectHeight = ship.shipSprite.getGlobalBounds().height;

		float closestX = std::clamp(circleX, rectX, rectX + rectWidth);
		float closestY = std::clamp(circleY, rectY, rectY + rectHeight);

		float dx = circleX - closestX;
		float dy = circleY - closestY;
		float distanceSquared = dx * dx + dy * dy;

		
		return distanceSquared <= radius * radius;
	}

	return false;
}

bool Map::colisionPointCheck(int originID, sf::Vector2f& position)
{
	if (getTypeObject(originID) == 2) {
		return allShips.get(originID).shipSprite.getGlobalBounds().contains(position);
	}
	if (getTypeObject(originID) == 1) {
		auto& star = stars.get(originID);
		float dx = star.star.getPosition().x + star.star.getRadius() - position.x;
		float dy = star.star.getPosition().y + star.star.getRadius() - position.y;
		float distanceSquared = dx * dx + dy * dy;
		return distanceSquared <= (star.star.getRadius() * star.star.getRadius());
	}


	return false;
}

void Map::printSectors()
{
	
	std::cout << "This function, should print sector, but func is changed and function is not working" << std::endl;
}

void Map::determineSectorsForObjects()
{
	int xSector;
	int ySector;
	for (size_t i = 0; i < allShips.size(); ++i)
	{
		int sectorX = (allShips.get(i).pos.x) / sectorSize;
		int sectorY = (allShips.get(i).pos.y) / sectorSize;
		allSectors.get(sectorX).get(sectorY).sectorShips.insert(allShips.get(i).id, allShips.get(i).id);
		allShips.get(i).sector[0] = sectorX;
		allShips.get(i).sector[1] = sectorY;
		std::cout << "there are coordinates for the sector, that is currectly processed: " << sectorX << ", " << sectorY << std::endl;
		
	}
	
	for (size_t i = 0; i < stars.size(); ++i)
	{
		for (size_t z = 0; z < mapSize.x / sectorSize; ++z)
		{
			for (size_t j = 0; j < mapSize.y / sectorSize; ++j)
			{
				if (stars.get(i).star.getGlobalBounds().intersects(allSectors.get(z).get(j).sectorRect.getGlobalBounds())) { 
					starsSectors.push_back(allSectors.get(z).get(j).id); 
					allSectors.get(z).get(j).sectorStars.insert(stars.get(i).id, stars.get(i).id);
					std::cout << "to sector: " << allSectors.get(z).get(j).id << " has been added star" << stars.get(i).id << std::endl;
				}
			}
		}
	}
}

std::tuple<bool, int> Map::collisionBetweenSectors(int ship_id, int sector_id)
{

	return std::make_tuple(false, -1);
}

// Update Gui function
void Map::updateSelectionUI(DynamicSparseSet<int> objects, tgui::ScrollablePanel::Ptr selectionPanel, tgui::Texture tguishipTextureShared, tgui::Texture tguiSharedTexture)
{
	int count = 0;
	for (const auto& elem : objects.getElements()) {
		// gui creation
		auto panel = tgui::Panel::create();
		panel->setSize({ "90%", "50" });
		panel->setPosition("5%", (150 + 30 + 10 + count * 60)); // Position dynamically

		// image creation
		auto pictureSelectionPanel = tgui::Picture::create();
		pictureSelectionPanel->setSize(30, 30);
		pictureSelectionPanel->setPosition(10, 10);
		pictureSelectionPanel->getRenderer()->setTexture(tguiSharedTexture);
		panel->add(pictureSelectionPanel);
		
		
		// label creationadad

		auto labelSelectionPanel = tgui::Label::create();
		labelSelectionPanel->setText("Object with id: 1");
		labelSelectionPanel->setPosition("20%", "5");
		panel->add(labelSelectionPanel);

		// input field creation
		auto inputSelectionPanel = tgui::EditBox::create();
		inputSelectionPanel->setPosition("20%", "20");
		inputSelectionPanel->setSize("30%", "20");
		inputSelectionPanel->setDefaultText("Place holder"); // Default text inside the input field
		panel->add(inputSelectionPanel);

		// button creation
		auto buttonSelectionPanel = tgui::Button::create();
		buttonSelectionPanel->setSize(20, 20);
		buttonSelectionPanel->setPosition("90%", "20");
		panel->add(buttonSelectionPanel);

		int type = getTypeObject(elem);
		if (type == 2) {
			auto ship = allShips.get(elem);
			pictureSelectionPanel->getRenderer()->setTexture(tguishipTextureShared);
			labelSelectionPanel->setText("Object with id: " + std::to_string(ship.id));
			selectedShips.insert(ship.id, ship.id);
		}
		if (type == 1) {
			auto star = stars.get(elem);
			pictureSelectionPanel->getRenderer()->setTexture(tguiSharedTexture);
			labelSelectionPanel->setText("Object with id: " + std::to_string(star.id));
			selectedStars.insert(star.id, star.id);
		}

		selectionPanel->add(panel);

		count += 1;
	}

}

// Group udpate logic
void Map::updateSelectionGroup(int groupNumber, DynamicSparseSet<int> selectedObjects)
{

	if (groupNumber == 1) {
		group1.clear();
		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group1.insert(object_id, object_id);
		}
		

	}
	if (groupNumber == 2) {
		group2.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group2.insert(object_id, object_id);
		}
	}
	if (groupNumber == 3) {
		group3.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group3.insert(object_id, object_id);
		}

	}
	if (groupNumber == 4) {
		group4.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group4.insert(object_id, object_id);
		}

	}
	if (groupNumber == 5) {
		group5.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group5.insert(object_id, object_id);
		}
	}
	if (groupNumber == 6) {
		group6.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group6.insert(object_id, object_id);
		}
	}
	if (groupNumber == 7) {
		group7.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group7.insert(object_id, object_id);
		}
	}
	if (groupNumber == 8) {
		group8.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group8.insert(object_id, object_id);
		}
	}
	if (groupNumber == 9) {
		group9.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group9.insert(object_id, object_id);
		}
	}
	if (groupNumber == 0) {
		group0.clear();

		for (size_t j = 0; j < selectedObjects.size(); ++j)
		{
			int object_id = selectedObjects.getElements()[j];
			group0.insert(object_id, object_id);
		}

	}
}

// Group selection
DynamicSparseSet<int> Map::selectGroup(int groupNumber)
{	
	DynamicSparseSet<int> selectedObjects;

	if (groupNumber == 1) {
		for (size_t j = 0; j < group1.size(); ++j)
		{
			int object_id = group1.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 2) {
		for (size_t j = 0; j < group2.size(); ++j)
		{
			int object_id = group2.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 3) {
		for (size_t j = 0; j < group3.size(); ++j)
		{
			int object_id = group3.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 4) {
		for (size_t j = 0; j < group4.size(); ++j)
		{
			int object_id = group4.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 5) {
		for (size_t j = 0; j < group5.size(); ++j)
		{
			int object_id = group5.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 6) {
		for (size_t j = 0; j < group6.size(); ++j)
		{
			int object_id = group6.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 7) {
		for (size_t j = 0; j < group7.size(); ++j)
		{
			int object_id = group7.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 8) {
		for (size_t j = 0; j < group8.size(); ++j)
		{
			int object_id = group8.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 9) {
		for (size_t j = 0; j < group9.size(); ++j)
		{
			int object_id = group9.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}
	if (groupNumber == 0) {
		for (size_t j = 0; j < group0.size(); ++j)
		{
			int object_id = group0.getElements()[j];
			selectedObjects.insert(object_id, object_id);
		}
	}

	return selectedObjects;
}

float Map::calculate_distance(sf::Vector2f A, sf::Vector2f B)
{
	// hypot => x2 + y2
	//return sqrt(std::powf(B.x - A.x, 2) + std::powf(B.y - A.y, 2));
	return std::hypot(B.x - A.x, B.y - A.y);
}

DynamicSparseSet<sf::Vector2f> Map::aPosNodeFinder(int startNodeId, int endNodeId, sf::Vector2f startPoint, sf::Vector2f endPoint)
{
	float distance = calculate_distance(startPoint, endPoint);
	int Node_id = 1;

	// get list of nodes from starting nodes
	float f;
	float g;


	return DynamicSparseSet<sf::Vector2f>();
}

// Struct to hold priority queue nodes (for A*)
struct AStarNode {
	int id;
	float f, g;
	bool operator>(const AStarNode& other) const {
		return f > other.f;
	}
};

// Reconstructs the path using the cameFrom map
std::vector<int> Map::reconstruct_path(std::unordered_map<int, int>& cameFrom, int current) {
	std::vector<int> total_path = { current };
	while (cameFrom.count(current)) {
		current = cameFrom[current];
		total_path.push_back(current);
	}
	std::reverse(total_path.begin(), total_path.end());
	return total_path;
}

// A* pathfinding algorithm using DynamicSparseSet
std::vector<int> Map::astar_pathfinding(
	int startId,
	int goalId)
	
{
	std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;
	std::unordered_map<int, float> gScore;
	std::unordered_map<int, float> fScore;
	std::unordered_map<int, int> cameFrom;

	gScore[startId] = 0.0f;
	fScore[startId] = calculate_distance(sf::Vector2f(static_cast<float>(stars.get(startId).starXposMap), static_cast<float>(stars.get(startId).starYposMap)), 
		sf::Vector2f(static_cast<float>(stars.get(goalId).starXposMap), static_cast<float>(stars.get(goalId).starYposMap)));
	openSet.push({ startId, fScore[startId], 0.0f });

	while (!openSet.empty()) {
		int currentId = openSet.top().id;
		openSet.pop();

		if (currentId == goalId) {
			return reconstruct_path(cameFrom, currentId);
		}

		StarSystem& current = stars.get(currentId);
		for (int neighborId : current.connections.getElements()) {
			std::cout << "ID that we are trying to access: " << neighborId << std::endl;
			if (!stars.contains(neighborId)) continue;
			float distance_currentTneighbor = calculate_distance(sf::Vector2f(static_cast<float>(stars.get(currentId).starXposMap), static_cast<float>(stars.get(currentId).starYposMap)),
				sf::Vector2f(static_cast<float>(stars.get(neighborId).starXposMap), static_cast<float>(stars.get(neighborId).starYposMap)));

			float tentative_gScore = gScore[currentId] + distance_currentTneighbor;

			if (!gScore.count(neighborId) || tentative_gScore < gScore[neighborId]) {
				cameFrom[neighborId] = currentId;
				gScore[neighborId] = tentative_gScore;
				float distance_neighborTgoal = calculate_distance(sf::Vector2f(static_cast<float>(stars.get(neighborId).starXposMap), static_cast<float>(stars.get(neighborId).starYposMap)),
					sf::Vector2f(static_cast<float>(stars.get(goalId).starXposMap), static_cast<float>(stars.get(goalId).starYposMap)));
				fScore[neighborId] = tentative_gScore + distance_neighborTgoal;
				openSet.push({ neighborId, fScore[neighborId], tentative_gScore });
			}
		}
	}

	return {}; // No path found
}

size_t PairHash::operator()(const std::pair<int, int>& p) const
{
	return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
}


