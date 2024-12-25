// ConsoleApplicationSFML.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <variant>
#include <algorithm>
#include <memory>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Core.hpp>          // Core system for GUI management
#include <TGUI/Widgets/Button.hpp> // Include specific widget headers, like Button
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <tuple>
#include "DynamicSparseSet.h"
#include "StarSystem.h"
#include "Map.h"
#include "SpaceShip.h"
#include <unordered_map>
#include <cmath>
#include <utility>  // For std::pair



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
    // Создаем окно с размерами 800x600 и названием "SFML Test"   , sf::Style::Fullscreen
    // everything about video setting we will find here
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Star Strategy Game");
    
    sf::Vector2u sizeWin = window.getSize();
    


    // setting view
    sf::View view(sf::FloatRect(-static_cast<int>(window.getSize().x/4), -static_cast<int>(window.getSize().y/4), window.getSize().x, window.getSize().y));
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

    // loading icons
    sf::Texture iconsSpreadsheet;
    if (!iconsSpreadsheet.loadFromFile("images/Icons and UI/tilemap_packed.png")) {
        std::cerr << "Failed to load icons!" << std::endl;
        return -1;
    }

    // loading complete

    // GUI creation and set settings
    tgui::Gui gui(window); // selection menu

    // preparing icons for UI
    sf::IntRect chatIconRect(0, 0, 16, 16);
    // Load the texture into TGUI::Texture using the sub-rectangle
    auto textureFilePath = "images/Icons and UI/tilemap_packed.png"; // Path to the sprite sheet
    //tgui::Texture tguiTexture(textureFilePath, { chatIconRect.left, chatIconRect.top, chatIconRect.width, chatIconRect.height });
    // Load the texture into TGUI::Texture
    //tgui::Texture tguiTexture("images/Icons and UI/tilemap_packed.png");

    //tguiTexture.setSubRect(chatIconRect);
    //sf::Image subImage = iconsSpreadsheet.copyToImage();
    //subImage = subImage.copy(chatIconRect.left, chatIconRect.top, chatIconRect.width, chatIconRect.height);


    // selection pannel
    auto panel = tgui::Panel::create();
    panel->setSize(sizeWin.x / 5.6, sizeWin.y / 1.5);
    panel->setPosition(150, 150);
    panel->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255, 125)); // Light blue color
    gui.add(panel);

    auto picture = tgui::Picture::create("Images/Placeholder/SpaceBackground-3.jpg");
    picture->setSize(panel->getSize().x - 40, 150);  // Image size
    picture->setPosition(20, 20);  // Position inside the panel
    panel->add(picture);

    // initially dark and not here
    panel->setVisible(false);

    /*auto button = tgui::Button::create("Click Me");
    button->setSize(150, 100);
    button->setPosition(150, 100);
    gui.add(button);*/

    // settings menu button on the top
    auto settingsMenuButton = tgui::Button::create("...");
    settingsMenuButton->setSize(50, 50);
    settingsMenuButton->setPosition(50, 50);
    settingsMenuButton->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255, 255));
    

    //settingsMenuButton->setTexture(iconTexture);
    /*tgui::Texture tguiPicture; 
    tguiPicture.loadFromPixelData(sfPicture.getSize(), sfPicture.copyToImage().getPixelsPtr());
    auto picture = tgui::Picture::create(std::move(tguiPicture));*/

    //sf::Image iconImage = iconSprite.getTexture()->copyToImage();  // Get the image from sprite 

    //tguiTexture.setData(sfTexture);
    //tgui::Texture tguiTexture(iconTexture);
    sf::Texture croppedTexture;
    croppedTexture.loadFromImage(iconsSpreadsheet.copyToImage(), sf::IntRect(80, 128, 16, 16));

    tgui::Texture chatIcon;
    chatIcon.loadFromPixelData(croppedTexture.getSize(), croppedTexture.copyToImage().getPixelsPtr());

    //chatIcon.~Texture


    //tguiTexture.loadFromPixelData(tguiTexture.getImageSize(), );
    settingsMenuButton->getRenderer()->setTexture(chatIcon);
    //settingsMenuButton->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255));
    //settingsMenuButton->getRenderer()->
    
    //settingsMenuButton->getRenderer()->setTexture(iconsSpreadsheet);
    gui.add(settingsMenuButton);

    // chat button in bottom left cornder
    auto chatButton = tgui::Button::create("Chat");
    chatButton->setSize(50, 50);
    chatButton->setPosition(50, sizeWin.y - 200);
    chatButton->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255));
    gui.add(chatButton);

    // GUI settings end

    // first star object


    // logic of creating objects

    sf::CircleShape dot(3);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition(400, 300);

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
  
    mapGameObject.addStar(star1);
    mapGameObject.addStar(star2);
    mapGameObject.addStar(star3);
    mapGameObject.addStar(star4);
    mapGameObject.addStar(star5);
    mapGameObject.addStar(star6);

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

    SpaceShip rocket = SpaceShip(sf::Vector2f(200.0f, 200.0f), &shipImageTexture, 1);
    SpaceShip superShip = SpaceShip(sf::Vector2f(170.0f, 150.0f), &shipImageTexture, 1);
    SpaceShip newShip = SpaceShip(sf::Vector2f(200.0f, 300.0f), &shipImageTexture, 2);


    //
    mapGameObject.addShip(rocket);
    mapGameObject.addShip(superShip);
    mapGameObject.addShip(newShip);

    std::cout << "Can go far there" << std::endl;
    for (size_t i = 0; i < mapGameObject.allShips.size(); ++i)
    {
        // get id of the ship

        int ship_id = mapGameObject.allShips.getElements()[i].id;
        // when got id, then use it as it should be used
        std::cout << "err not happend yet" << std::endl;
        mapGameObject.allShips.get(ship_id).Display(window);
    }
    // THIS IS EXAMPLE -------------------------------------------------------------
    std::cout << "we are pass this loop" << std::endl;
    //mapGameObject.determineSectorsForObjects();
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

   
    // This is a part of selection logic (rectangle selection)
    sf::Vector2i mouseWindowOnButtonPress;
    bool mouseStilPressed = false;
    sf::Vector2i mouseWindowOnButtonRelease;
    sf::RectangleShape mouseBox;
    mouseBox.setFillColor(sf::Color(255, 0, 0, 128));
    mouseBox.setOutlineColor(sf::Color(255, 0, 0));

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
            gui.handleEvent(event);



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
            sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window); 
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

            bool isOverGui;
            tgui::Widget::Ptr widget = gui.getWidgetAtPos(sf::Vector2f(mouseWindowPos.x, mouseWindowPos.y), true);
            if (widget) {
                isOverGui = true;
                //std::cout << isOverGui << std::endl;
            }
            else {
                isOverGui = false;
                //std::cout << isOverGui << std::endl;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isOverGui) {
                

                

                // selection with control
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    // inside selected?
                    // loop throw sheeps and check, if I selected a one
                    // this will fail
                    for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
                    {
                        int ship_id = mapGameObject.allShips.getElements()[i].id;
                        //mapGameObject.allShips.get(ship_id).Display(window);
                        if (mapGameObject.allShips.get(ship_id).shipSprite.getGlobalBounds().contains(mouseWorldPos)) {
                        // select
                            //int ship_id = mapGameObject.allShips.get(i).id;

                            if (mapGameObject.selectedShips.contains(ship_id)) {
                                mapGameObject.selectedShips.erase(ship_id);
                                std::cout << "DELTED SHIP WITH CTRL, SHIP ID: " << ship_id << std::endl;
                                break;
                            }
                            else {
                                mapGameObject.selectObject(ship_id, "ship"); // can I rather make it with booleans?
                                std::cout << "ADDED SHIP WITH CTRL, SHIP ID: " << ship_id<< std::endl;
                                break;
                            }
                            // inside selected == true?
                            
                            break;
                        }

                    }
                    for (size_t i = 0; i < mapGameObject.stars.size(); i++)
                    {
                        int star_id = mapGameObject.stars.getElements()[i].id;
                        if (mapGameObject.stars.get(i).star.getGlobalBounds().contains(mouseWorldPos)) {
                            // select
                            

                            if (mapGameObject.selectedStars.contains(star_id)) {
                                mapGameObject.selectedStars.erase(star_id);
                                std::cout << "DELTED STAR WITH CTRL, STAR_ID: " << star_id << std::endl;
                                break;
                            }
                            else {
                                mapGameObject.selectObject(star_id, "star"); // can I rather make it with booleans?
                                std::cout << "ADDED STAR WITH CTRL, STAR_ID: " << star_id << std::endl;

                                break;
                            }
                            // inside selected == true?

                        }

                    }


                    // loop throw stars and check if I selected a one (in sectors)
                }
                else {
                    mapGameObject.cleanSelection();
                    // this will fail
                    for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
                    {
                        int ship_id = mapGameObject.allShips.getElements()[i].id;

                        if (mapGameObject.allShips.get(ship_id).shipSprite.getGlobalBounds().contains(mouseWorldPos)) {
                            // select
                            
                            mapGameObject.selectObject(ship_id, "ship"); // can I rather make it with booleans?
                               
                            // inside selected == true?
                            std::cout << "SELECTED SHIP AND CLEARED SELECTION, SHIP_ID: " << ship_id << std::endl;

                            break;
                        }

                    }
                    for (size_t i = 0; i < mapGameObject.stars.size(); i++)
                    {
                        int star_id = mapGameObject.stars.getElements()[i].id;
                        if (mapGameObject.stars.get(star_id).star.getGlobalBounds().contains(mouseWorldPos)) {
                            // select
                            
                            mapGameObject.selectObject(star_id, "star"); // can I rather make it with booleans?
                            std::cout << "SELECTED STAR AND CLEARED SELECTION, STAR_ID: " << star_id << std::endl;

                            break;
                            // inside selected == true?

                        }

                    }
                }
                
                
            }

            bool selectionClear = true;
            // selection without control
            if (event.type == sf::Event::MouseButtonPressed) {
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    selectionClear = false;
                }

                if (selectionClear && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    mapGameObject.cleanSelection();
                }

                //sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);
                //sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

                // detect mouse and write it to vectors
                mouseWindowOnButtonPress = sf::Vector2i(mouseWorldPos.x ,mouseWorldPos.y);
                mouseStilPressed = true;
                

                if (event.mouseButton.button == sf::Mouse::Right) {
                    //std::cout << "Coordinates of mouse:" << event.mouseButton.x << " X, " << event.mouseButton.y << " Y.\n";
                    // it's printing the coordinates of window. Question is, how do I get real coordinates?

                    sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);

                    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

                   
                    // that is how I can iterate throw selectedShips
                    for (const auto& elem : mapGameObject.selectedShips.getElements())
                    {
                        int ship_id = mapGameObject.selectedShips.get(elem);
                        if (!mapGameObject.allShips.get(elem).inMotion) {
                            mapGameObject.movingShips.insert(ship_id, ship_id);
                            std::cout << "Inserted ship into moving objects, ship id: " << ship_id << std::endl;
                        }
                        std::cout << "Set new target for selected ship, ship id: " << ship_id << std::endl;
                        mapGameObject.allShips.get(elem).setNewTarget(mouseWorldPos);
                    }
                    
                }
            }

            if (mouseStilPressed) {
                mouseWindowOnButtonRelease = sf::Vector2i(mouseWorldPos.x, mouseWorldPos.y);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                // selection logic here
                mouseStilPressed = false;
                // before I do that, I need always swap coordinates, so function will always work
                // coordinates needs to be swaped to left up corner
                if (mouseWindowOnButtonPress.x > mouseWindowOnButtonRelease.x) {
                    std::swap(mouseWindowOnButtonPress.x, mouseWindowOnButtonRelease.x);
                }
                if (mouseWindowOnButtonPress.y > mouseWindowOnButtonRelease.y) {
                    std::swap(mouseWindowOnButtonPress.y, mouseWindowOnButtonRelease.y);
                }

                // also I don't understand, why am I selecting all cells, instead of cells that having something inside, It is possible, that I don't have this funciton
                auto relevantCells = mapGameObject.getFilledCells(sf::Vector2f(mouseWindowOnButtonPress.x, mouseWindowOnButtonPress.y), sf::Vector2f(mouseWindowOnButtonRelease.x - mouseWindowOnButtonPress.x, mouseWindowOnButtonRelease.y - mouseWindowOnButtonPress.y));
                
                // I will make this cells work, after that I will take up code, that is responsible for
                // coordinates adjustments

                // get all ids from vectors into set
                // select

                DynamicSparseSet<int> selectedObjects;
                // iterate throw relevant cells
                int x = 0;
                for (const auto& cell : relevantCells) {
                    auto& objects = mapGameObject.grid[cell];
                    std::cout << "Iteration in grid: " << x << std::endl;
                    x += 1;
                    
                    // after this I need to check if it is preciselly inside rectangle

                    // iterate throw vector and
                    for (auto& vectorElement : objects)
                    {
                        std::cout << "we are not here" << std::endl;

                        int type = mapGameObject.getTypeObject(vectorElement);
                        std::cout << "we are here" << std::endl;
                        // insert its objects into its dynamic sparse set, based on Id of the object
                        if (type == 2) {
                            std::cout << "We are selected ship: " << vectorElement << std::endl;
                            mapGameObject.selectedShips.insert(vectorElement, vectorElement);
                        }
                        if (type == 1) {
                            std::cout << "We are selected star: " << vectorElement << std::endl;
                            mapGameObject.selectedStars.insert(vectorElement, vectorElement);
                        }
                    }



                    


                    //.push_back(star.id);
                }

                // iterate throw positions in

            }

            // logic of zooming
            // #todo add maximum zooming and minimum zooming (done)
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
               

                view.setSize(windowSize.x / zoomFactor, windowSize.y / zoomFactor);
                shaderStar.setUniform("zoomFactor", zoomFactor);
            }
            


        }

        // moving all objects before rendering
        // Calculate elapsed time
        float deltaTime = clock.restart().asSeconds(); // this thing here, because all objects depend on same time.

        

        window.clear(sf::Color::Black);

        window.draw(backgroundSprite); 

        // check collision inside this loop
       
        // possibly broken iteration     
        // 
        // Retrieve the dense vector of selected ships
        auto& movingShips = mapGameObject.movingShips.getElements();

        for (size_t i = 0; i < mapGameObject.movingShips.size(); ++i)
        {
            const auto& elem = movingShips[i];
            std::cout << "Current element in moving ships: " << elem << std::endl;
            int ship_id = mapGameObject.movingShips.get(elem);
            std::cout << "Got the ship id: " << ship_id << std::endl;

            int sectorX = (mapGameObject.allShips.get(elem).pos.x) / mapGameObject.sectorSize;
            int sectorY = (mapGameObject.allShips.get(elem).pos.y) / mapGameObject.sectorSize;


            // I need to determine object grid and then draw it inside

            sf::Vector2f oldPos = mapGameObject.allShips.get(elem).pos;
            //mapGameObject.removeFromHashMap(elem, mapGameObject.allShips.get(elem).pos);
            mapGameObject.allShips.get(elem).Move(deltaTime);

            // instead of removing, update object position
            //currentlyOccupiedCells;
            mapGameObject.updateObjectPosition(elem, oldPos, mapGameObject.allShips.get(elem).pos);
            
            if (!mapGameObject.allShips.get(elem).inMotion) {
                mapGameObject.movingShips.erase(elem);
                break;
            }

            // this don't work as it should
            DynamicSparseSet<int> nearbyObjects = mapGameObject.queryHashMap(mapGameObject.allShips.get(elem).pos, 15, elem);
            std::cout << "Size of nearbyObjets: " << nearbyObjects.size() << std::endl;
            if (nearbyObjects.size() > 0) {
                std::cout << "Coliding this are all nearby objects!" << std::endl;
                
                auto& nearObjects = nearbyObjects.getElements();
                // u need to iterate using other way...
                for (size_t j = 0; j < nearbyObjects.size(); ++j)
                {
                    // use nearby Element
                    const auto& nearbyElement = nearObjects[j];

                    std::cout << "Objects: " << nearbyObjects.get(nearbyElement) << std::endl;

                    int objectTypeThis = mapGameObject.getTypeObject(elem);
                    int objectTypeNearby = mapGameObject.getTypeObject(nearbyElement);
                    // here I will make logic of colision

                    // add complex logic of colision (detect preciselly, if objects are colliding
                    bool collides = mapGameObject.colisionCheck(elem, nearbyElement);

                    if (collides) {

                        // making ships destroyable
                        if (objectTypeThis == objectTypeNearby == 1) {
                            // if other ships has other team, then I will make them destroy each other for now... 
                            int thisTeam = mapGameObject.allShips.get(elem).teamID;
                            int otherTeam = mapGameObject.allShips.get(nearbyElement).teamID;
                            if (thisTeam != otherTeam) {
                                std::cout << "Destroying ship with id: " << elem << ", " << nearbyElement << std::endl;
                                mapGameObject.destroyShip(elem);
                                mapGameObject.destroyShip(nearbyElement);
                                nearbyObjects.erase(nearbyElement);
                                // delete them from grid too
                                break;
                            }

                        }
                    }


                }
            }
            
        }
        
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
       


        // Set the view to apply the scaling and translation
        window.setView(view);

        // error
        for (size_t i = 0; i < mapGameObject.allShips.size(); ++i)
        {
            int ship_id = mapGameObject.allShips.getElements()[i].id;
            mapGameObject.allShips.get(ship_id).Display(window);
        }

        window.draw(time_text);

        if (mapGameObject.selectedStars.size() < 1 && mapGameObject.selectedShips.size() < 1) {
            panel->setVisible(false);
        }
        else {
            panel->setVisible(true);
        }


        // this is the type of selection, that is under gui. Second type of selection would be on top of the gui
        if (mouseStilPressed) {
            // draw selection (correct)
            mouseBox.setPosition(sf::Vector2f(mouseWindowOnButtonPress.x, mouseWindowOnButtonPress.y));
            mouseBox.setSize(sf::Vector2f( mouseWindowOnButtonRelease.x - mouseWindowOnButtonPress.x,  mouseWindowOnButtonRelease.y - mouseWindowOnButtonPress.y));
            window.draw(mouseBox);
        }


        gui.draw();
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
