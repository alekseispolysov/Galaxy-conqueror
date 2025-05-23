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
    bool inMotion = false;
    sf::Vector2f pos;
    sf::Vector2f endPos;
    sf::Vector2f direction = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f shipOrigin;
    std::vector<std::pair<int, int>> currentlyOccupiedCells;
    float speed = 100.0f;
    int sector[2];
    int id;
    int teamID;
    int home_node;
    
    std::string team = ("base_team");
    
    float curAngle = 0;
    float endAngle = 0;
    float directionOfRotation = 0;
    float rotationSpeed = 150.00f;



    bool visiable = true;
    bool freeFly;
    float damage;
    float health;
    float fuel;

    sf::Texture* shipTexutre;
    sf::Sprite shipSprite;
    sf::Vector2f spriteSize;
    
    std::vector<sf::Vector2f> MemoryPath;
    // replace vector with 2D coordinates array. If ship path is reset, then it just resets array. Ship path can only be set by stars
    //std::vector<StarSystem*> path;

    SpaceShip(sf::Vector2f pos, sf::Texture* shipTexutre, int teamID, int home_node, bool freFly=false, int id = 0);

    ~SpaceShip();

    //collision check
    bool ColisionCheck(SpaceShip* ship);

    void SetTeam(std::string team);

    // drawing function
    void Display(sf::RenderWindow& win);

    void setNewTarget(sf::Vector2f position, bool newCords);
    void setNewTarget();

    void UpdateTargetList(std::vector<sf::Vector2f> cordList); 




    // flying to another star function (or to place on the map)
    // flying to another position. In the future, this function would be remade to only allow it between stars
    void Move(float deltaTime);
    // turning function
    void Turn(float angle);

    



private:
    

};

