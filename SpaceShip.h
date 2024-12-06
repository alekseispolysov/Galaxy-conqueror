#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "StarSystem.h"

class SpaceShip
{
public:

    bool selected = false;
    bool isMoving = false;
    bool isRotating = false;
    sf::Vector2f pos;
    sf::Vector2f endPos;
    sf::Vector2f direction = sf::Vector2f(0.0f, 0.0f);
    float speed = 100.0f;
    
    std::string team = ("base_team");
    
    float curAngle = 0;
    float endAngle = 0;
    float directionOfRotation = 0;
    float rotationSpeed = 150.00f;



    bool visiable = true;
    float damage;
    float health;
    float fuel;
    sf::Texture* shipTexutre;
    sf::Sprite shipSprite;
    // replace vector with 2D coordinates array. If ship path is reset, then it just resets array. Ship path can only be set by stars
    //std::vector<StarSystem*> path;

    SpaceShip(sf::Vector2f pos, sf::Texture* shipTexutre);

    ~SpaceShip();

    //collision check
    void ColisionCheck();

    void SetTeam(std::string team);

    // drawing function
    void Display(sf::RenderWindow& win);

    void setNewTarget(sf::Vector2f position);

    // flying to another star function (or to place on the map)
    // flying to another position. In the future, this function would be remade to only allow it between stars
    void Move(float deltaTime);
    // turning function
    void Turn(float angle);


private:
    

};

