// ConsoleApplicationSFML.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <variant>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <tuple>
#include "StarSystem.h"
#include "Map.h"
#include "SpaceShip.h"




// function to compare two arrays with 2 elements
bool compareArrays(const int a[2], const int b[2]) {
    for (int i = 0; i < 2; ++i) {
        if (a[i] != b[i]) {
            return false; // Found a difference
        }
    }
    return true; // All elements match
}


int main()
{
    std::cout << "Application has started\n";
    // Создаем окно с размерами 800x600 и названием "SFML Test"
    sf::RenderWindow window(sf::VideoMode(800, 600), "Star Strategy Game");


    // setting view
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    window.setView(view);
    //view.setSize(800, 600);

    float zoomFactor = 1.0f;

    // Loading star shader
    sf::Shader shaderStar;
    if (!shaderStar.loadFromFile("radialgradient.frag", sf::Shader::Fragment)) {
        std::cerr << "Failed to load Star Shader!" << std::endl;
        return -1;
    }
    shaderStar.setUniform("zoomFactor", zoomFactor); 



    // first star object
  

    // logic of creating objects

    sf::CircleShape dot(3);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition(400, 300);

    
    // Loading all textures and images
    // loading background screen
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Images/background/bg1.jpg")) {
        std::cerr << "Failed to load backgoround Image!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    sf::IntRect textureRect(200, 200, 800, 600); // x, y, width, height
    backgroundSprite.setTextureRect(textureRect);


    sf::Texture shipImageTexture;
    if (!shipImageTexture.loadFromFile("Images/Ships/Ship.png")) {
        std::cerr << "Failed to load Ship Image!" << std::endl;
        return -1;
    }
    //sf::Sprite shipImageSprite;
    
    // loading font for text
    sf::Font font;
    if (!font.loadFromFile("fonts/ARIAL.TTF")) {
        std::cerr << "Failed to load font Arial!" << std::endl;
        return -1;
    }


    //shaderStar.setUniform("fadeFactor", 0.5f); // Adjust fade sharpness

    // second star object
    StarSystem star1 = StarSystem(450, 350, 30, sf::Color::Red, "Spira");
    StarSystem star2 = StarSystem(400, 300, 20, sf::Color::White, "Rius");
    StarSystem star3 = StarSystem(100, 100, 20, sf::Color::Green, "Sagitarius");
    StarSystem star4 = StarSystem(120, 150, 40, sf::Color::Green, "Oloma");
    StarSystem star5 = StarSystem(300, 100, 60, sf::Color::Yellow, "Abarak");
    StarSystem star6 = StarSystem(310, 230, 60, sf::Color::White, "Yrokoj");
    
    star1.ConnectTo(&star2);
    star2.ConnectTo(&star6);
    star4.ConnectTo(&star5); 
    star6.ConnectTo(&star5); 
    star6.ConnectTo(&star4); 
    star3.ConnectTo(&star4); 
    star3.ConnectTo(&star5); 

    Map mapGameObject = Map(sf::Vector2f(800.0f, 600.0f));
  
    mapGameObject.addStar(&star1);
    mapGameObject.addStar(&star2);
    mapGameObject.addStar(&star3);
    mapGameObject.addStar(&star4);
    mapGameObject.addStar(&star5);
    mapGameObject.addStar(&star6);

    sf::CircleShape testShape(30);
    testShape.setFillColor(sf::Color::White);
    testShape.setPosition(20, 500);

    //const int layers = 20;
    //const float maxRadius = 50;

    // creating line in sfml
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(400,300), sf::Color(255,255,255,50)),
        sf::Vertex(sf::Vector2f(500,400), sf::Color(255,255,255, 50)),
    };

    // Drawing border of the map
    // drawing line from one point to other point
    // point --> point (2 represents line)
    sf::Vertex mapBorder[] = {
        sf::Vertex(sf::Vector2f(0,0), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(800,0), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(800,0), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(800,600), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(800,600), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(0,600), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(0,600), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(0,0), sf::Color::Magenta),
    };

    SpaceShip rocket = SpaceShip(sf::Vector2f(200.0f, 200.0f), &shipImageTexture);
    SpaceShip superShip = SpaceShip(sf::Vector2f(170.0f, 150.0f), &shipImageTexture);
    SpaceShip newShip = SpaceShip(sf::Vector2f(200.0f, 300.0f), &shipImageTexture);



    mapGameObject.addShip(&rocket);
    mapGameObject.addShip(&superShip);
    mapGameObject.addShip(&newShip);

    /*for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
    {
        mapGameObject.allShips[i]->Display(window);
    }*/

    mapGameObject.determineSectorsForObjects();
    // define clocks

    sf::Clock clock; // from here impolimenting object movement

    sf::Clock game_time;

    sf::Text time_text;
    time_text.setFont(font);
    
    time_text.setString("No data");
    time_text.setFillColor(sf::Color::White);
    std::string timeString;
    std::string time_in_seconds;
    int time_minutes = 0;
    int time_hours = 0;
    float time_days;
    float time_weeks;
    float time_months;
    float time_years;

    //sf::CircleShape sprite(10.f);
    //sprite.setFillColor(sf::Color::Green);
    //sprite.setPosition(100.f, 100.f); // Starting position



    //sf::Vector2f start(100.f, 100.f);
    // 
    // MOVE THIS IN WHERE IT BELONGS
    //sf::Vector2f stop(500.f, 400.f); // in stop setting call
    //sf::Vector2f direction = normalize(stop - start); // Normalized direction vector // this will happen inside ship, maybe?
    //float speed = 100.f; // Pixels per second // inside ship?
    //sf::Clock clock; // For tracking time // here I defined this
    // 
    //sf::Vector2f currentPosition = start; // already defined on creating -----


    // Главный цикл программы
    while (window.isOpen())
    {
        // move it to another class ============================================================
        //time_text.setPosition(view.getCenter().x / 2, view.getCenter().y / 2); 
        sf::Vector2f curCenter = view.getCenter();
        sf::Vector2u curPosCenter = window.getSize();
        sf::Vector2f viewSize = view.getSize();
        //sf::Vector2f viewOffset = sf::Vector2f(curCenter.x - curPosCenter.x / 2 , curCenter.y - curPosCenter.y / 2);
        sf::Vector2f viewOffset = sf::Vector2f(curCenter - viewSize / 2.0f);
        time_text.setPosition(viewOffset);
        // I need to create function, that posistiones to left up corner
        float time_seconds = game_time.getElapsedTime().asSeconds(); // if i want it run faster just increase it like that * x
        if (time_seconds >= 60.0f) {
            game_time.restart();
            time_minutes += 1;
            time_seconds = 0;
        }
        if (time_minutes >= 60.0f) {
            time_minutes = 0;
            time_hours += 1;
        }
        if (time_seconds < 10) {
            time_in_seconds = "0" + std::to_string(time_seconds);
        }
        else {
            time_in_seconds = std::to_string(time_seconds); 
        }
        time_in_seconds = time_in_seconds.substr(0, 5);
        timeString = std::to_string(time_hours) + ":" + std::to_string(time_minutes) + ":" + time_in_seconds;

        time_text.setString(timeString); // I want clock that are not reset
        
        // clock LOGIC -> transfer to another class
        // move it to another class ===============================================================================



        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Закрытие окна при нажатии на крестик
            if (event.type == sf::Event::Closed){
                window.close();
            }

            else if (event.type == sf::Event::KeyPressed) {
                // Moving camera
                if (event.key.code == sf::Keyboard::W) {
                    view.setCenter(view.getCenter().x, view.getCenter().y - 40);
                    // I need to do change stars position

                    // test moving of 
                    
                    
                }
                if (event.key.code == sf::Keyboard::A) {
                    view.setCenter(view.getCenter().x - 40, view.getCenter().y);
                    
                }
                if (event.key.code == sf::Keyboard::D) {
                    view.setCenter(view.getCenter().x + 40, view.getCenter().y);
                    
                }
                if (event.key.code == sf::Keyboard::S) {
                    view.setCenter(view.getCenter().x, view.getCenter().y + 40);
                    
                }



                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    std::cout << "\nWindows closed by Escape key";
                }
            }
            // resize logic
            // todo, when I resize, I want to steal in the right place on the map
            else if (event.type == sf::Event::Resized) {
                // when I resize, I change coordinates of view, right here
                /////////////////////////////////////////////////////////////////
                view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                
                // if I want window go back I do this, but for now just comment this, because it is useless
                //view.setCenter(static_cast<float>(event.size.width) / 2, static_cast<float>(event.size.height) / 2);
                

                // Update window size uniforms when the window is resized
                shaderStar.setUniform("windowHeight", static_cast<float>(event.size.height));
                //shaderStar.setUniform("windowWidth", static_cast<float>(event.size.width));

                shaderStar.setUniform("zoomFactor", zoomFactor); 
                // Optional: Update any other uniforms or scaling logic
                sf::Vector2u windowSize = window.getSize();
                view.setSize(windowSize.x / zoomFactor, windowSize.y / zoomFactor);
            }
            //

            // selection logic
            bool control = false;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view); 

                bool disable_star_selection = false;
                StarSystem* storageStar;
                // selection with control
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    bool inSideSelected = false;
                    for (size_t i = 0; i < mapGameObject.selectableObjects.size(); i++)
                    {
                        // for loop that checks, if object inside selected objects, if so it just removes it and breaks cycle
                        // 
                        //std::cout << "This are selectable objects: " << mapGameObject.selectableObjects[i] << std::endl;
                        // fixing everything
                        
                        if (std::holds_alternative<SpaceShip*>(mapGameObject.selectableObjects[i])) {
                            std::cout << "here" << std::endl;
                            SpaceShip* ship = std::get<SpaceShip*>(mapGameObject.selectableObjects[i]);
                            if (ship->shipSprite.getGlobalBounds().contains(mouseWorldPos)) {
                                // try to find it in selected objects
                                //for (size_t j = 0; j < mapGameObject.selectedObjects.size(); ++j) {
                                //    // if this ship, star or other object is present in this array ( passed as argument ), then remove it and return true;
                                //    if (true) {

                                //    }
                                //}
                                inSideSelected = mapGameObject.checkIfObjectSelected(ship, true);
                                if (inSideSelected) {
                                    break;
                                }
                                mapGameObject.selectObject(ship);
                                std::cout << "I put ship object into selected objects: " << ship;
                                break;
                            }
                        }
                        else if (std::holds_alternative<StarSystem*>(mapGameObject.selectableObjects[i]) && !disable_star_selection) {
                            StarSystem* star = std::get<StarSystem*>(mapGameObject.selectableObjects[i]);
                            if (star->star.getGlobalBounds().contains(mouseWorldPos)) {
                                //std::cout << "it seems this is star object" << std::endl;
                                disable_star_selection = true;
                                storageStar = star;
                                mapGameObject.selectObject(star);
                                continue;
                            }
                        }

                    }
                    if (disable_star_selection) {
                        inSideSelected = mapGameObject.checkIfObjectSelected(storageStar, true);
                        if (inSideSelected) {
                            
                        }
                        else {
                        mapGameObject.selectObject(storageStar);
                        std::cout << "star is selected";

                        }
                    }
                }
                // logic without control
                else {
                    bool added = false;
                    for (size_t i = 0; i < mapGameObject.selectableObjects.size(); i++)
                    {
                        //std::cout << "This are selectable objects: " << mapGameObject.selectableObjects[i] << std::endl;
                        // fixing everything
                        if (std::holds_alternative<SpaceShip*>(mapGameObject.selectableObjects[i])) {
                            std::cout << "here" << std::endl;
                            SpaceShip* ship = std::get<SpaceShip*>(mapGameObject.selectableObjects[i]);
                            if (ship->shipSprite.getGlobalBounds().contains(mouseWorldPos)) {
                                mapGameObject.cleanSelection();
                                mapGameObject.selectObject(ship);
                                std::cout << "I put ship object into selected objects: " << ship;
                                added = true;
                                break;
                            }
                        }
                        else if (std::holds_alternative<StarSystem*>(mapGameObject.selectableObjects[i]) && !disable_star_selection) {
                            StarSystem* star = std::get<StarSystem*>(mapGameObject.selectableObjects[i]);
                            if (star->star.getGlobalBounds().contains(mouseWorldPos)) {
                                //std::cout << "it seems this is star object" << std::endl;
                                disable_star_selection = true;
                                storageStar = star;
                                mapGameObject.selectObject(star);
                                continue;
                            }
                        }

                    }
                    
                    if (disable_star_selection) {
                        mapGameObject.cleanSelection();
                        mapGameObject.selectObject(storageStar);
                        std::cout << "star is selected";
                    }
                    else if (!added) {
                        mapGameObject.cleanSelection();
                    }

                }

                ///// PROBABLY WRONG ARRAY ITERATING, NEED TO CRATE ARRAY OF ALL SELECTABLE OBJECTS <<<--- VARIANT FUCKER
                
                //control = true;
            }

            // selection without control
            if (event.type == sf::Event::MouseButtonPressed) {
                //if (event.mouseButton.button == sf::Mouse::Left && !control) {
                //    //std::cout << "Coordinates of mouse:" << event.mouseButton.x << " X, " << event.mouseButton.y << " Y.\n";
                //    // Selection of ships

                //    sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);
                //    bool ctrl = false;
                //    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

                //    



                //    // selection ships logic
                //    for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
                //    {
                //        if (mapGameObject.allShips[i]->shipSprite.getGlobalBounds().contains(mouseWorldPos)) {
                //            mapGameObject.cleanSelection();
                //            mapGameObject.selectShip(mapGameObject.allShips[i]);
                //            std::cout << "Selected: " << mapGameObject.allShips[i] << std::endl;
                //            break;
                //        }                    
                //        else if (!mapGameObject.allShips[i]->shipSprite.getGlobalBounds().contains(mouseWorldPos)) {
                //            mapGameObject.cleanSelection();
                //            std::cout << "Cleared selection";
                //        }
                //    }

                //    
                //    


                //}
                if (event.mouseButton.button == sf::Mouse::Right) {
                    //std::cout << "Coordinates of mouse:" << event.mouseButton.x << " X, " << event.mouseButton.y << " Y.\n";
                    // it's printing the coordinates of window. Question is, how do I get real coordinates?

                    sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);

                    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

                    //sf::Vector2f stop(500.f, 400.f); // in stop setting call
                    // move function were called here

                    // I need dictionary of all ship objects -> it will 
                    // all object selection
                    for (size_t i = 0; i < mapGameObject.selectedObjects.size(); ++i) {
                        //mapGameObject.selectedObjects[i]->setNewTarget(mouseWorldPos);
                        
                        if (std::holds_alternative<SpaceShip*>(mapGameObject.selectedObjects[i])) {
                            SpaceShip* ship = std::get<SpaceShip*>(mapGameObject.selectedObjects[i]);
                    
                            if (!ship->inMotion) {
                            mapGameObject.movingShips.push_back(ship);

                            }


                            ship->setNewTarget(mouseWorldPos);
                            
                        }


                    }
                    //rocket.setNewTarget(mouseWorldPos); =============================================================

                    
                }
            }

            // logic of zooming
            // #todo add maximum zooming and minimum zooming
            if (event.type == sf::Event::MouseWheelScrolled) 
            {
                if (event.mouseWheelScroll.delta > 0) { 
                    // Zoom in (scale the view by 0.9)
                    if (!(zoomFactor < 0.5)) {
                    zoomFactor *= 0.9f;
                    shaderStar.setUniform("zoomFactor", zoomFactor);

                    }
                }
                else {
                    // Zoom out (scale the view by 1.1)
                    if(!(zoomFactor > 3)){
                        //std::cout << "True";
                        zoomFactor *= 1.1f;
                        shaderStar.setUniform("zoomFactor", zoomFactor);
                    }
                    //std::cout << "zoomFactor is:" << zoomFactor;
                    

                }

                // Apply the new zoom factor
                sf::Vector2u windowSize = window.getSize();
                
                // debuging zoom factor
                /*std::cout << "\nWin X:" << windowSize.x << " Win Y:" << windowSize.y;
                std::cout << " -- View Size X:" << view.getSize().x << " View size Y: " << view.getSize().y;
                std::cout << " -- Zoom factor is:" << zoomFactor;*/

                view.setSize(windowSize.x / zoomFactor, windowSize.y / zoomFactor);
                shaderStar.setUniform("zoomFactor", zoomFactor);
            }
            


        }

        // moving all objects before rendering
        // Calculate elapsed time
        float deltaTime = clock.restart().asSeconds(); // this thing here, because all objects depend on same time.
        
        //// Update the position based on direction and speed// call move function here?
        //currentPosition += direction * speed * deltaTime;

        //// Check if the sprite is close enough to the target to stop
        //if (std::hypot(stop.x - currentPosition.x, stop.y - currentPosition.y) < speed * deltaTime) {
        //    currentPosition = stop; // Snap to the final position
        //}

        //sprite.setPosition(currentPosition);

        //rocket.Move(deltaTime);

        

        window.clear(sf::Color::Black);

        window.draw(backgroundSprite); 

        // check collision inside this loop
       
        for (size_t i = 0; i < mapGameObject.movingShips.size(); i++)
        {
            int sectorX = (mapGameObject.movingShips[i]->pos.x) / mapGameObject.sectorSize;
            int sectorY = (mapGameObject.movingShips[i]->pos.y) / mapGameObject.sectorSize;

            std::cout << "Sector: " << sectorX << ", " << sectorY << std::endl;

            // iterate throw the sector objects
            SpaceShip* curShip = mapGameObject.movingShips[i];
            Sector* s = mapGameObject.allSectors[sectorX][sectorY];

            //if (curShip->sector != {sectorX, sectorY}) {
            //}

            int sSector[2];
            sSector[0] = sectorX;
            sSector[1] = sectorY;
            // changing sector
            if (!compareArrays(curShip->sector, sSector)) {
                //std::cout << "\n\n\nCHANGE HAS HAPPEND \n\n\n";
                //mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.erase(std::remove(mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.begin(), mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.end(), ship), mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.end());
                
                auto new_end = std::remove(mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.begin(), mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.end(), curShip);

                // Erase the "removed" elements from the vector
                mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.erase(new_end, mapGameObject.allSectors[curShip->sector[0]][curShip->sector[1]]->sectorShips.end());

                mapGameObject.allSectors[sectorX][sectorY]->sectorShips.push_back(curShip);

                curShip->sector[0] = sectorX;
                curShip->sector[1] = sectorY;

            }

            //s->displaySector(window);
            mapGameObject.allSectors[sectorX][sectorY]->displaySector(window);

            for (size_t j = 0; j < s->sectorStars.size(); ++j)
            {
                if (s->sectorStars[j]->checkCollisionShip(mapGameObject.movingShips[i])) {
                    std::cout << "New collision is working" << std::endl;
                }
            }

            for (size_t j = 0; j < s->sectorShips.size(); j++)
            {
                // collision between ships in current sector
                if (mapGameObject.movingShips[i]->ColisionCheck(s->sectorShips[j]) && mapGameObject.movingShips[i] != s->sectorShips[j]) {
                    std::cout << "colision with ship are working" << std::endl;
                    std::cout << "destroying both ships" << std::endl;
                    delete curShip;
                }
            }



            // more visiably appearing collision check:
            // Calculate edges of the ship and the sector
            float shipLeft = curShip->pos.x - curShip->spriteSize.x;
            float shipRight = curShip->pos.x + curShip->spriteSize.x;
            float shipTop = curShip->pos.y - curShip->spriteSize.y;
            float shipBottom = curShip->pos.y + curShip->spriteSize.y;

            float sectorLeft = s->position.x;
            float sectorRight = s->position.x + s->sector_size;
            float sectorTop = s->position.y;
            float sectorBottom = s->position.y + s->sector_size;

            // Collision checks
            bool collidingRight = shipRight >= sectorRight;
            bool collidingUp = shipTop <= sectorTop;
            bool collidingLeft = shipLeft <= sectorLeft;
            bool collidingDown = shipBottom >= sectorBottom;

            bool collidingRightUp = collidingRight && collidingUp;
            bool collidingRightDown = collidingRight && collidingDown;
            bool collidingLeftUp = collidingLeft && collidingUp;
            bool collidingLeftDown = collidingLeft && collidingDown;

            bool notCollided = true;

            // Check for right and up collision simultaneously
            if (collidingRightUp) {
                std::cout << "Collision with right and up sector simultaneously" << std::endl;
                notCollided = false;

            }
            // Check for left and up collision simultaneously
            if (collidingLeftUp) {
                std::cout << "Collision with left and up sector simultaneously" << std::endl;
                notCollided = false;
            }
            // Check for left and down collision simultaneously
            if (collidingLeftDown) {
                std::cout << "Collision with left and down sector simultaneously" << std::endl;
                notCollided = false;
            }
            // Check for right and down collision simultaneously
            if (collidingRightDown) {
                std::cout << "Collision with right and down sector simultaneously" << std::endl;
                notCollided = false;
            }
            
            if (collidingDown && notCollided) {
                std::cout << "Down" << std::endl;
                Sector* s = mapGameObject.allSectors[sectorX][sectorY+1];
                std::cout << "COLISION IS:" << std::get<0>(mapGameObject.collisionBetweenSectors(curShip, s)) << std::endl;
            }
            if (collidingUp && notCollided) {
                std::cout << "Up" << std::endl;
                Sector* s = mapGameObject.allSectors[sectorX][sectorY - 1];
                std::cout << "COLISION IS:" << std::get<0>(mapGameObject.collisionBetweenSectors(curShip, s)) << std::endl;
            }
            if (collidingLeft && notCollided) {
                std::cout << "Left" << std::endl;
                Sector* s = mapGameObject.allSectors[sectorX - 1][sectorY];
                std::cout << "COLISION IS:" << std::get<0>(mapGameObject.collisionBetweenSectors(curShip, s)) << std::endl;

            }
            if (collidingRight && notCollided) {
                std::cout << "Right" << std::endl;
                Sector* s = mapGameObject.allSectors[sectorX + 1][sectorY];
                std::cout << "COLISION IS:" << std::get<0>(mapGameObject.collisionBetweenSectors(curShip, s)) << std::endl;
            }
            // sector colision for ship check






            // You can still keep individual checks for other directions if needed
            /*if (collidingRight) {
                std::cout << "Collision with right sector" << std::endl;
            }
            if (collidingUp) {
                std::cout << "Collision with up sector" << std::endl;
            }*/


            //// right sector check
            //if (curShip->spriteSize.x + curShip->pos.x >= s->position.x + s->sector_size) {
            //    // check for up and down
            //    std::cout << "Colision with right working" << curShip->pos.x << " + " << curShip->spriteSize.x << " => " << s->position.x << " + " << s->sector_size << std::endl;
            //}
            //// left sector check
            //if (curShip->pos.x - curShip->spriteSize.x <= s->position.x) {
            //    // check for up and down
            //    std::cout << "Colision with left working" << std::endl;
            //}
            //// up sector check
            //if (curShip->pos.y - curShip->spriteSize.x <= s->position.y) {
            //    // check for left and right
            //    std::cout << "Colision with up working" << std::endl;
            //}
            //// down sector check
            //if (curShip->spriteSize.x + curShip->pos.y >= s->position.y + s->sector_size) {
            //    // check for left and right
            //    std::cout << "Colision with down working" << std::endl;
            //}


            if (true) {
                // check if ships position, colide with active sector, that is assigned to ship
                // check if ship collides with sector, that its currently in
                mapGameObject.movingShips[i]->pos.x;
                mapGameObject.movingShips[i]->pos.y;
                


            }



            // check colision between star and ship
            //for (size_t j = 0; j < mapGameObject.stars.size(); ++j) {
            //    if (mapGameObject.stars[j]->checkCollisionShip(mapGameObject.allShips[i])) {
            //        mapGameObject.allShips[i]->visiable = false;
            //        std::cout << "Coliding"; // why when it gets created, it colides? ?????!??!?!?!?!??!?!?!?!
            //        break;
            //    }
            //    else {
            //        mapGameObject.allShips[i]->visiable = true;

            //    }
            //}
            // check colision between ships

            // colision of ships
            // When I set ship to move, I will add this sector to active. When it is not moving, I will delete it from active sectors
        // collision rewrite, optimization
            //for (size_t g = 0; g < mapGameObject.activeSectors.size(); ++g)
            //{
            //    //check colision in active sectors
            //    
            //     
            //    
            //    //std::cout << "size of active sectors.ships: " << mapGameObject.activeSectors[g]->sectorShips.size() << std::endl;
            //    for (size_t j = 0; j < mapGameObject.activeSectors[g]->sectorShips.size(); ++j)
            //    {
            //        // check if ship has left the sector
            //        if (true) {
            //            // remove it from this sector
            //            
            //            // add it to another sector

            //        }


            //        // check for collisions in ships, that are located between multiple sectors

            //        //check colision between ships and ships in active sectors

            //        //check colision between stars and ships in active sectors
            //        for (size_t e = 0; e < mapGameObject.activeSectors[g]->sectorStars.size(); ++e)
            //        {


            //            if (mapGameObject.activeSectors[g]->sectorStars[e]->checkCollisionShip(mapGameObject.activeSectors[g]->sectorShips[j])) {
            //                mapGameObject.activeSectors[g]->sectorShips[j]->visiable = false;
            //                std::cout << "Coliding"; // why when it gets created, it colides? ?????!??!?!?!?!??!?!?!?!
            //                break;
            //            }
            //            else {
            //                mapGameObject.activeSectors[g]->sectorShips[j]->visiable = true;

            //            }
            //        }
            //    }

            //}










            // I am calling move function here
            mapGameObject.movingShips[i]->Move(deltaTime);
            if (!mapGameObject.movingShips[i]->inMotion) {
                mapGameObject.movingShips.erase(std::remove(mapGameObject.movingShips.begin(), mapGameObject.movingShips.end(), mapGameObject.movingShips[i]), mapGameObject.movingShips.end());
                break;
            }
        }




        // Очистка окна (цвет фона)
        


        

        //mapGameObject.allSectors[3][2]->displaySector(window);

        // Drawing all active sectors 
        //for (size_t i = 0; i < mapGameObject.activeSectors.size(); i++)
        //{
        //    mapGameObject.activeSectors[i]->displaySector(window);
        //}

        //draw lines before stars
        //window.draw(line, 3, sf::Lines); 
        
        // Set Star Shader parametrs
        // 
        // To invert Y coordinates
        shaderStar.setUniform("windowHeight", static_cast<float>(window.getSize().y));



        // Drawing objects
        

        shaderStar.setUniform("center", sf::Vector2f(500, 400));
        shaderStar.setUniform("color", sf::Glsl::Vec3(1.0f, 1.0f, 1.0f));
        shaderStar.setUniform("radius", 30.0f);
        // To invert Y coordinates
        shaderStar.setUniform("windowHeight", static_cast<float>(window.getSize().y));

        //std::cout << "Second star x, y";

        //mapGameObject.printSectors(); test function, I can print what I want from sectors
        
        mapGameObject.Display(window, shaderStar, zoomFactor);

        // debug dot
        window.draw(dot);

        window.draw(testShape);


        
        /*star4.DrawAllConnections(window);
        star4.Display(window, shaderStar, zoomFactor);*/

        
       /* star3.DrawAllConnections(window);
        star3.Display(window, shaderStar, zoomFactor);
        star5.Display(window, shaderStar, zoomFactor);*/

         
        /*star6.DrawAllConnections(window);
        star6.Display(window, shaderStar, zoomFactor);*/




        //window.draw(mapBorder, 4, sf::Quads);
        //window.draw(mapGameObject.mapBorder, 8, sf::Lines);


        // Set the view to apply the scaling and translation
        window.setView(view);

        for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
        {
            mapGameObject.allShips[i]->Display(window);
        }

        window.draw(time_text);

        // Отображение окна на экране
        window.display();   
    }

    return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
