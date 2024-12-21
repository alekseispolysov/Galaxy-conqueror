#include <cmath>
#include <iostream>
#include "SpaceShip.h"

// Function to normalize a vector, so we can move object towards coordinate
sf::Vector2f normalize(const sf::Vector2f& vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length == 0) return { 0, 0 };
	return vector / length; // Divide each component by the magnitude
}

SpaceShip::SpaceShip(sf::Vector2f pos, sf::Texture* shipTexutre, int teamID, int id):pos(pos), shipTexutre(shipTexutre), teamID(teamID), id(id) {
	//shipSprite.setTextureRect(sf::IntRect(0, 0, 30, 100)); // With that I can access the one sheet with all sprites
	shipSprite.setTexture(*shipTexutre);
	shipSprite.setScale(sf::Vector2f(0.1f, 0.1f)); // size 2times smaller
	sf::IntRect textureRect = shipSprite.getTextureRect();
	sf::Vector2f scale = shipSprite.getScale();
	spriteSize = sf::Vector2f(
		textureRect.width * scale.x,
		textureRect.height * scale.y
	);
	// # bug FIX CENTER DEPENDENCY ON SIZE OF THE TEXTURE
	shipOrigin = sf::Vector2f(spriteSize.x * 5, spriteSize.y * 5);
	shipSprite.setOrigin(spriteSize.x * 5 , spriteSize.y * 5); // Right now it is in center?
	//std::cout << "Sprite center is created in: " << shipSprite.getOrigin().x << ", " << shipSprite.getOrigin().y << std::endl;
	shipSprite.setPosition(sf::Vector2f(pos.x - shipSprite.getOrigin().x, pos.y - shipSprite.getOrigin().y));
	//shipSprite.setScale()
	Move(1.0f); // when I call move function on creation. It is not coliding
}


SpaceShip::~SpaceShip() {

}

bool SpaceShip::ColisionCheck(SpaceShip* ship)
{
	return shipSprite.getGlobalBounds().intersects(ship->shipSprite.getGlobalBounds());
}

void SpaceShip::SetTeam(std::string team)
{
	team = team;
}

void SpaceShip::Display(sf::RenderWindow& win)
{
	if (visiable) {
		win.draw(shipSprite);

	}
}

// called, when new target for ship is assigned
void SpaceShip::setNewTarget(sf::Vector2f position)
{

	// here we maybe move some angle logic
	endPos = position;
	// get current angle
	curAngle = shipSprite.getRotation();
	
	// get right angle

	// set rotating to true

	// Creating direction vector, that is responsible for moving ship into ther right direction
	direction = normalize(endPos - pos);

	/* std::atan2(y, x) computes the angle(in radians) between the positive
		x
		x - axis and the vector(direction).
		The result is converted to degrees by multiplying by
		180
		---
		P
		.*/
	// problem can be right here, or it can be in the rotation logic. Probably in both
	endAngle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f + 90.0f;		// arguments (y, x) + 90 degrees in the end to fix direction
	//std::cout << "Current angle:" << curAngle << std::endl;
	//std::cout << "Get new angle:" << endAngle << std::endl;
	
	isMoving = false;
	inMotion = true;
	if (endAngle != curAngle) {

	isRotating = true;
	}
}

// rename this function
void SpaceShip::Move(float deltaTime)
{
	// first rotate, then move
	
	if (isRotating) {
		directionOfRotation = endAngle - curAngle;
		if (directionOfRotation > 180.0f) directionOfRotation -= 360.0f;
		if (directionOfRotation < -180.0f) directionOfRotation += 360.0f;


		//std::cout << "Angle counting...: " << curAngle;

		// absolute means always positive number || or condition is ensure if curAngle 440 it can be over 360 deg bigger then endAngle
		if (std::abs(curAngle - endAngle) < 1.15f || std::abs(curAngle - endAngle) > 360 - 1.15) {
			//std::cout << "True, relocating angle" << std::endl;
            curAngle = endAngle; // Snap to target angle
            isRotating = false; // Rotation complete
            isMoving = true;    // Start moving phase
        } else {
			if (directionOfRotation > 0) {
				curAngle += rotationSpeed * deltaTime;
			}
			else if (directionOfRotation < 0) {
				curAngle -= rotationSpeed * deltaTime;
				/*std::cout << "Still working on that counting...: " << curAngle << std::endl;
				std::cout << "END: " << endAngle << std::endl;*/
			}
			else {
				isRotating = false;
				isMoving = true;
			}
        }
	}

	// when we finished rotating, we can move
	if (isMoving) {
		pos += direction * speed * deltaTime;


		// sqrt of (x2 + y2)
		if (std::hypot(endPos.x - pos.x, endPos.y - pos.y) < speed * deltaTime) {
			pos = endPos;
			isMoving = false;
			isRotating = false;
			inMotion = false;
		}

	}

	shipSprite.setRotation(curAngle);
	shipSprite.setPosition(pos);
}

// old logic
	// apperantly I need to call function new position. In move position I need just change some stuf
	//sf::Vector2f direction = normalize(position - pos);

	// fix position
	//shipSprite.setPosition(position);

	// logic of moving
	//if (true) {
	//	// simular angle transformation


	//	// simular 

	//	isMoving = true;
	//}

	// check if it reached destination to turn off moving.
	// 
	// new logic

	//// Update the position based on direction and speed// call move function here?
	//currentPosition += direction * speed * deltaTime;

	//// Check if the sprite is close enough to the target to stop
	//if (std::hypot(stop.x - currentPosition.x, stop.y - currentPosition.y) < speed * deltaTime) {
	//    currentPosition = stop; // Snap to the final position
	//}

	//sprite.setPosition(currentPosition);

void SpaceShip::Turn(float angle)
{
}


