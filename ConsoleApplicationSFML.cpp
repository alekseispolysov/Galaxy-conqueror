// ConsoleApplicationSFML.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <variant>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "StarSystem.h"
#include "Map.h"
#include "SpaceShip.h"

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
    SpaceShip newShip = SpaceShip(sf::Vector2f(200.0f, 100.0f), &shipImageTexture);

    mapGameObject.addShip(&rocket);
    mapGameObject.addShip(&superShip);
    mapGameObject.addShip(&newShip);
    // define clocks

    sf::Clock clock; // from here impolimenting object movement

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


        // check collision inside this loop

        for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
        {
            // check colision between star and ship
            for (size_t j = 0; j < mapGameObject.stars.size(); ++j) {
                if (mapGameObject.stars[j]->checkCollisionShip(mapGameObject.allShips[i])) {
                    mapGameObject.allShips[i]->visiable = false;
                    std::cout << "Coliding"; // why when it gets created, it colides? ?????!??!?!?!?!??!?!?!?!
                    break;
                }
                else {
                    mapGameObject.allShips[i]->visiable = true;

                }
            }
            // check colision between ships


            mapGameObject.allShips[i]->Move(deltaTime);
        }




        // Очистка окна (цвет фона)
        window.clear(sf::Color::Black);

        window.draw(backgroundSprite);


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
