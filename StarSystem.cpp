#include "StarSystem.h"
#include "SpaceShip.h"
#include "DynamicSparseSet.h"
#include <SFML/Graphics.hpp>
#include <iostream>


StarSystem::StarSystem(int starXposMap, int starYposMap, float radius, sf::Color starColor, std::string name)
    :starXposMap(starXposMap), starYposMap(starYposMap), radius(radius), starColor(starColor), name(name)
{
    // Initialize member variables

    //this->radius = radius;
    //this->starColor = starColor;
    //this->name = name;
    // array with connections
    //this->connections = connections;

    // Intializing star object
    star = sf::CircleShape(radius);
    star.setFillColor(starColor);
    star.setPosition(starXposMap - radius, starYposMap - radius);

    //

}

StarSystem::~StarSystem()
{
    // Any necessary cleanup goes here (if needed)
    /*for (StarSystem* star : connections) {
        delete star;
    }
    connections.clear();*/

}


// takes display and shaders
void StarSystem::Display(sf::RenderWindow& win, sf::Shader& shader, float zoomFactor)
{
    // Display connections first
    // 
     
    // Display star itself

    sf::RenderStates states;
    states.shader = this->setShader(win, shader, zoomFactor);


    win.draw(star, states);


}

void StarSystem::ConnectTo(StarSystem* otherSystem) {
    connections.push_back(otherSystem);
}

void StarSystem::DrawAllConnections(sf::RenderWindow& win) {

    for (size_t i = 0; i < this->connections.size(); ++i)
    {
        
        sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(starXposMap,starYposMap), sf::Color(starColor.r, starColor.g, starColor.b, 55.0f)),
        sf::Vertex(sf::Vector2f(this->connections[i]->starXposMap,this->connections[i]->starYposMap), sf::Color(this->connections[i]->starColor.r, this->connections[i]->starColor.g, this->connections[i]->starColor.b, 55.0f)),
        };
        win.draw(line, 2, sf::Lines);
    }
}

bool StarSystem::checkCollisionShip(SpaceShip* ship)
{
    return star.getGlobalBounds().intersects(ship->shipSprite.getGlobalBounds());
}

sf::Shader* StarSystem::setShader(sf::RenderWindow& win,sf::Shader& shader, float zoomFactor)
{
    // Extract Vec3 object from starColor

    sf::Glsl::Vec3 colorVec(
        this->starColor.r / 255.0f,  // Normalize red
        this->starColor.g / 255.0f,  // Normalize green
        this->starColor.b / 255.0f   // Normalize blue
    );

    // I need to pass here window
 
    sf::View currentView = win.getView();
    sf::Vector2f viewSize = currentView.getSize(); 

    //std::cout << "Current ViewSize X:" << viewSize.x << "\nCurrent ViewSize Y:" << viewSize.y;

    sf::Vector2f currentCenter = currentView.getCenter();

    // reversing both Vector and Window size
    currentCenter.y *= -1 ;

    // Get window size as sf::Vector2u
    sf::Vector2u sizeU = win.getSize();
    // Convert to sf::Vector2f
    //sf::Vector2f sizeF(static_cast<float>(sizeU.x), static_cast<float>(sizeU.y));
    
    // applying offsets to shaders position
    // # add zooming logic to update shader correctlly
    sf::Vector2f viewOffset = (currentCenter - sf::Vector2f(sizeU.x / 2, 0)  + sf::Vector2f(0, sizeU.y / 2));
    //sf::Vector2f viewOffset = currentCenter - viewSize / 2.0f; 

    //viewOffset.y = -viewOffset.y; 

    // Shader applying logic, before drawing star on screen
    shader.setUniform("center", (sf::Vector2f(this->starXposMap, this->starYposMap)));
    shader.setUniform("color", colorVec);
    shader.setUniform("radius", this->radius);
    shader.setUniform("windowHeight", static_cast<float>(win.getSize().y));
    shader.setUniform("viewSize", viewSize);
    shader.setUniform("viewOffset", viewOffset);
    shader.setUniform("screenSize", sf::Vector2f(sizeU.x, sizeU.y));




    return &shader;
}



    




