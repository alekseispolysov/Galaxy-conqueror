#include <iostream>
#include <any>
#include <variant>
#include <algorithm>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Core.hpp>        
#include <TGUI/Widgets/Button.hpp> 
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <tuple>
#include "DynamicSparseSet.h"
#include "StarSystem.h"
#include "Map.h"
#include "SpaceShip.h"
#include "Team.h"
#include "Particle.h"
#include <unordered_map>
#include <cmath>
#include <utility>  



// function to compare two arrays with 2 elements
bool compareArrays(const int a[2], const int b[2]) {
    for (int i = 0; i < 2; ++i) {
        if (a[i] != b[i]) {
            return false; 
        }
    }
    return true; 
}




// Variables for debug
bool soundOn = true;


int main()
{
    
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Star Strategy Game");
    
    sf::Vector2u sizeWin = window.getSize();
    


    // setting view
    sf::View view(sf::FloatRect(-static_cast<int>(window.getSize().x/4), -static_cast<int>(window.getSize().y/4), window.getSize().x, window.getSize().y));
    window.setView(view);

    float zoomFactor = 1.0f;

    // Loading star shader
    sf::Shader shaderStar;
    if (!shaderStar.loadFromFile("radialgradient.frag", sf::Shader::Fragment)) {
        std::cerr << "Failed to load Star Shader!" << std::endl;
        return -1;
    }
    shaderStar.setUniform("zoomFactor", zoomFactor); 

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Images/background/bg1.jpg")) {
        std::cerr << "Failed to load backgoround Image!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    sf::IntRect textureRect(200, 200, 800, 600);
    backgroundSprite.setTextureRect(textureRect);

    sf::Texture sharedTexturePlaceholderPic;
    if (!sharedTexturePlaceholderPic.loadFromFile("Images/Placeholder/Background-1.jpg")) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }

    // texture for explosion:
    sf::Texture explosionTexture;
    if (!explosionTexture.loadFromFile("Images/Particles/Explosion/explosion 3.png")) {
        std::cerr << "Failed to load explosion texture!" << std::endl;
        return -1;
    }


    sf::Texture shipImageTexture;
    if (!shipImageTexture.loadFromFile("Images/Ships/Ship.png")) {
        std::cerr << "Failed to load Ship Image!" << std::endl;
        return -1;
    }
    
    // load sounds here
    sf::SoundBuffer bufferSelectingUnits;
    sf::SoundBuffer bufferSendingShips;
    if (soundOn) {

    
    if (!bufferSelectingUnits.loadFromFile("Sounds/Units/rollover1.ogg")) {
        std::cerr << "Error loading selecting ship sound file" << std::endl;
        return -1; 
    }

    if (!bufferSendingShips.loadFromFile("Sounds/Units/Ship flying.wav")) {
        std::cerr << "Error loading sending ship sound file" << std::endl;
        return -1; 
    }
    }
    // creating sound object
    sf::Sound sound; 

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
    tgui::Gui gui(window); 

    tgui::Texture tguiSharedTexture;
    tguiSharedTexture.loadFromPixelData(
        sharedTexturePlaceholderPic.getSize(),               
        sharedTexturePlaceholderPic.copyToImage().getPixelsPtr() 
    );

    tgui::Texture tguishipTextureShared;
    tguishipTextureShared.loadFromPixelData(
        shipImageTexture.getSize(),               
        shipImageTexture.copyToImage().getPixelsPtr()
    );

    // preparing icons for UI
    sf::IntRect chatIconRect(0, 0, 16, 16);
    auto textureFilePath = "images/Icons and UI/tilemap_packed.png"; 

    // selection pannel
    auto panelSelection = tgui::ScrollablePanel::create(); 
    panelSelection->setSize(sizeWin.x / 5.6, sizeWin.y / 1.5);
    panelSelection->setPosition(150, 150);
    panelSelection->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255, 125)); 
    gui.add(panelSelection);

    auto picture = tgui::Picture::create("Images/Placeholder/SpaceBackground-3.jpg");
    picture->setSize(panelSelection->getSize().x - 40, 150);  
    picture->setPosition(20, 20);  
    panelSelection->add(picture);

    DynamicSparseSet<tgui::Panel::Ptr> selectionPannels;
    
    panelSelection->setVisible(false);

    // settings menu button on the top
    auto settingsMenuButton = tgui::Button::create("...");
    settingsMenuButton->setSize(50, 50);
    settingsMenuButton->setPosition(50, 50);
    settingsMenuButton->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255, 255));
    

    sf::Texture croppedTexture;
    croppedTexture.loadFromImage(iconsSpreadsheet.copyToImage(), sf::IntRect(80, 128, 16, 16));

    tgui::Texture chatIcon;
    chatIcon.loadFromPixelData(croppedTexture.getSize(), croppedTexture.copyToImage().getPixelsPtr());

    settingsMenuButton->getRenderer()->setTexture(chatIcon);
 
    gui.add(settingsMenuButton);

    // chat button in bottom left cornder
    auto chatButton = tgui::Button::create("Chat");
    chatButton->setSize(50, 50);
    chatButton->setPosition(50, sizeWin.y - 200);
    chatButton->getRenderer()->setBackgroundColor(tgui::Color(100, 150, 255));
    gui.add(chatButton);

    // Object with all game logic
    Map mapGameObject = Map(sf::Vector2f(800.0f, 600.0f));

    // debug red dot
    sf::CircleShape dot(3);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition(400, 300);

    // create three teams
    Team team1 = Team(1, sf::Color(0, 255, 0));
    Team team2 = Team(2, sf::Color(255, 165, 0));
    Team rebels = Team(0, sf::Color(255, 255, 255));

    DynamicSparseSet<Team> teams;
    teams.insert(team1.team_id, team1);
    teams.insert(team2.team_id, team2);
    teams.insert(rebels.team_id, rebels);

    StarSystem star1 = StarSystem(450, 350, 30, team2.team_id, team2.teamColor, "Spira");
    StarSystem star2 = StarSystem(400, 300, 20, rebels.team_id, rebels.teamColor, "Rius");
    StarSystem star3 = StarSystem(100, 100, 20, team1.team_id, team1.teamColor, "Sagitarius");
    StarSystem star4 = StarSystem(120, 150, 40, team1.team_id, team1.teamColor, "Oloma");
    StarSystem star5 = StarSystem(300, 100, 60, rebels.team_id, rebels.teamColor, "Abarak");
    StarSystem star6 = StarSystem(310, 230, 60, rebels.team_id, rebels.teamColor, "Yrokoj");
    StarSystem star7 = StarSystem(500, 500, 30, team2.team_id, team2.teamColor, "Sardy");
    
    mapGameObject.addStar(star1);
    mapGameObject.addStar(star2);
    mapGameObject.addStar(star3);
    mapGameObject.addStar(star4);
    mapGameObject.addStar(star5);
    mapGameObject.addStar(star6);
    mapGameObject.addStar(star7);

    // add stars to teams:
    team2.addStarToTeam(star1.id);
    team2.addStarToTeam(star7.id);
    team1.addStarToTeam(star3.id);
    team1.addStarToTeam(star4.id);
    rebels.addStarToTeam(star2.id);
    rebels.addStarToTeam(star5.id);
    rebels.addStarToTeam(star6.id);

    mapGameObject.connect_stars(star1.id, star2.id);
    mapGameObject.connect_stars(star2.id, star6.id); 
    mapGameObject.connect_stars(star4.id, star5.id);
    mapGameObject.connect_stars(star6.id, star5.id);
    mapGameObject.connect_stars(star6.id, star4.id);
    mapGameObject.connect_stars(star3.id, star4.id);
    mapGameObject.connect_stars(star3.id, star5.id);
    mapGameObject.connect_stars(star7.id, star1.id);

    std::cout << star1.id << std::endl;
    std::cout << star2.id << std::endl;
    std::cout << star3.id << std::endl;
    std::cout << star4.id << std::endl;
    std::cout << star5.id << std::endl;
    std::cout << star6.id << std::endl;

    sf::CircleShape testShape(30);
    testShape.setFillColor(sf::Color::White);
    testShape.setPosition(20, 500);

    // creating line in sfml
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(400,300), sf::Color(255,255,255,50)),
        sf::Vertex(sf::Vector2f(500,400), sf::Color(255,255,255, 50)),
    };

    // Drawing border of the map
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

    SpaceShip rocket = SpaceShip(sf::Vector2f(100.0f, 200.0f), &shipImageTexture, team1.team_id, 3, true);
    SpaceShip superShip = SpaceShip(sf::Vector2f(100.0f, 120.0f), &shipImageTexture, team1.team_id,2, true);
    SpaceShip newShip = SpaceShip(sf::Vector2f(300.0f, 300.0f), &shipImageTexture, team2.team_id, 0, true);
    SpaceShip otherShip = SpaceShip(sf::Vector2f(100.0f, 20.0f), &shipImageTexture, team1.team_id, 2, false);

    mapGameObject.addShip(rocket);
    mapGameObject.addShip(superShip);
    mapGameObject.addShip(newShip);
    mapGameObject.addShip(otherShip);

    for (size_t i = 0; i < mapGameObject.allShips.size(); ++i)
    {
        int ship_id = mapGameObject.allShips.getElements()[i].id;
        mapGameObject.allShips.get(ship_id).Display(window);
    }
   

    sf::Clock clock;
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

   
    // This is a part of selection logic (rectangle selection)
    sf::Vector2i mouseWindowOnButtonPress;
    bool mouseStilPressed = false;
    bool continiousSelection = false;
    sf::Vector2i mouseWindowOnButtonRelease;
    sf::RectangleShape mouseBox;
    mouseBox.setFillColor(sf::Color(255, 0, 0, 128));
    mouseBox.setOutlineColor(sf::Color(255, 0, 0));

    // selection logic
    constexpr float DOUBLE_CLICK_TIME = 0.3f;
    constexpr float DOUBLE_CLICK_RADIUS = 10.f;
    sf::Clock double_click_timer;
    float lastClickTime = -DOUBLE_CLICK_TIME;
    sf::Vector2f lastClickPos;
    bool dragging = false;
    sf::Vector2i dragStart;


    // Main loop
    while (window.isOpen())
    {
        sf::Vector2f curCenter = view.getCenter();
        sf::Vector2u curPosCenter = window.getSize();
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2f viewOffset = sf::Vector2f(curCenter - viewSize / 2.0f);
        time_text.setPosition(viewOffset);
        
        float time_seconds = game_time.getElapsedTime().asSeconds();
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

        time_text.setString(timeString); 


        // Event handler
        sf::Event event;
        while (window.pollEvent(event))
        {
            gui.handleEvent(event);

            // Closing window
            if (event.type == sf::Event::Closed){
                window.close();
            }

            else if (event.type == sf::Event::KeyPressed) {
                // Moving camera
                if (event.key.code == sf::Keyboard::W) {
                    view.setCenter(view.getCenter().x, view.getCenter().y - 40); 
                }
                if (event.key.code == sf::Keyboard::A && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    view.setCenter(view.getCenter().x - 40, view.getCenter().y);
                    
                }
                if (event.key.code == sf::Keyboard::D) {
                    view.setCenter(view.getCenter().x + 40, view.getCenter().y);
                    
                }
                if (event.key.code == sf::Keyboard::S) {
                    view.setCenter(view.getCenter().x, view.getCenter().y + 40);
                    
                }

                // Selection wia groups, press 1,2,3,4,5,6,7,8,9,0
                {
                    DynamicSparseSet<int> selectedObjects;
                    if (event.key.code == sf::Keyboard::Num1 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        std::cout << "Pressed 1" << std::endl; 
                        selectedObjects = mapGameObject.selectGroup(1);
                    }
                    if (event.key.code == sf::Keyboard::Num2 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(2);
                    }
                    if (event.key.code == sf::Keyboard::Num3 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(3);
                    }
                    if (event.key.code == sf::Keyboard::Num4 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(4);
                    }
                    if (event.key.code == sf::Keyboard::Num5 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(5);
                    }
                    if (event.key.code == sf::Keyboard::Num6 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(6);
                    }
                    if (event.key.code == sf::Keyboard::Num7 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(7);
                    }
                    if (event.key.code == sf::Keyboard::Num8 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(8);
                    }
                    if (event.key.code == sf::Keyboard::Num9 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(9);
                    }
                    if (event.key.code == sf::Keyboard::Num0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.cleanSelection();
                        selectedObjects = mapGameObject.selectGroup(0);
                    }
                    for (size_t j = 0; j < selectedObjects.size(); ++j)
                    {
                        std::cout << "Selected element: " << selectedObjects.getElements()[j] << std::endl;
                        int el_id = selectedObjects.getElements()[j];

                        bool inShips = mapGameObject.allShips.contains(el_id);
                        bool inStars = mapGameObject.stars.contains(el_id);

                        if (inShips) {
                            mapGameObject.selectedShips.insert(el_id, el_id);
                        }
                        if (inStars) {
                            mapGameObject.selectedStars.insert(el_id, el_id);
                        }
                    }

                    mapGameObject.updateSelectionUI(selectedObjects, panelSelection, tguishipTextureShared, tguiSharedTexture);
                }

                // selection assigning to groups:
                {
                    DynamicSparseSet<int> selectedObjects;

                    for (size_t j = 0; j < mapGameObject.selectedStars.size(); ++j)
                    {
                        int star_id = mapGameObject.selectedStars.getElements()[j];
                        mapGameObject.selectStar(star_id);
                        selectedObjects.insert(star_id, star_id);
                    }
                    for (size_t j = 0; j < mapGameObject.selectedShips.size(); ++j)
                    {
                        int ship_id = mapGameObject.selectedShips.getElements()[j];
                        mapGameObject.selectShip(ship_id);
                        selectedObjects.insert(ship_id, ship_id);

                    }

                    if (event.key.code == sf::Keyboard::Num1 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        std::cout << "Group assign 1" << std::endl;
                       
                    
                        mapGameObject.updateSelectionGroup(1, selectedObjects);

                    }
                    if (event.key.code == sf::Keyboard::Num2 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(2, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num3 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(3, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num4 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(4, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num5 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(5, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num6 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(6, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num7 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(7, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num8 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(8, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num9 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(9, selectedObjects);
                    }
                    if (event.key.code == sf::Keyboard::Num0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        mapGameObject.updateSelectionGroup(0, selectedObjects);
                    }
                }

                if (event.key.code == sf::Keyboard::A && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {

                    DynamicSparseSet<int> selectedObjects;

                    for (size_t j = 0; j < mapGameObject.stars.size(); ++j)
                    {
                        int star_id = mapGameObject.stars.getElements()[j].id;
                        mapGameObject.selectStar(star_id);
                        selectedObjects.insert(star_id, star_id);
                    }
                    for (size_t j = 0; j < mapGameObject.allShips.size(); ++j)
                    {
                        int ship_id = mapGameObject.allShips.getElements()[j].id;
                        mapGameObject.selectShip(ship_id);
                        selectedObjects.insert(ship_id, ship_id);

                    }
                    mapGameObject.updateSelectionUI(selectedObjects, panelSelection, tguishipTextureShared, tguiSharedTexture);

                }


                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            else if (event.type == sf::Event::Resized) {
                view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                
                // Update window size uniforms when the window is resized
                shaderStar.setUniform("windowHeight", static_cast<float>(event.size.height));
                
                shaderStar.setUniform("zoomFactor", zoomFactor); 
              
                sf::Vector2u windowSize = window.getSize();
                view.setSize(windowSize.x / zoomFactor, windowSize.y / zoomFactor);
            }
            sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window); 
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view); 

            // check if mouse is over gui
            bool isOverGui;
            tgui::Widget::Ptr widget = gui.getWidgetAtPos(sf::Vector2f(mouseWindowPos.x, mouseWindowPos.y), true);
            if (widget) {
                isOverGui = true;
                
            }
            else {
                isOverGui = false;
                
            }

            if (mapGameObject.hoveredObject != -1) {
                std::any object = mapGameObject.getObject(mapGameObject.hoveredObject);

                if (object.type() == typeid(SpaceShip)) {
                    mapGameObject.allShips.get(mapGameObject.hoveredObject).shipSprite.setColor(mapGameObject.originHoverColor);
                }
                else if (object.type() == typeid(StarSystem)) {
                    mapGameObject.stars.get(mapGameObject.hoveredObject).starColor = mapGameObject.originHoverColor;
                }
                
                mapGameObject.hoveredObject = -1;
            }

            // hover effect
            DynamicSparseSet<int> hoverQuery = mapGameObject.queryHashMap(mouseWorldPos, 3, 1);
            for (size_t i = 0; i < hoverQuery.size(); ++i)
            {
                int q_id = hoverQuery.getElements()[i];
                std::any object = mapGameObject.getObject(q_id); 

                sf::RectangleShape mouseB;
                mouseB.setSize(sf::Vector2f(3.f, 3.f)); 
                mouseB.setPosition(sf::Vector2f(mouseWorldPos)); 


                if (object.type() == typeid(SpaceShip)) {
                    SpaceShip ship = std::any_cast<SpaceShip>(object);
                    if (ship.shipSprite.getGlobalBounds().intersects(mouseB.getGlobalBounds())) {
                        mapGameObject.hoveredObject = ship.id;
                        mapGameObject.originHoverColor = ship.shipSprite.getColor();
                        std::cout << "Space ship: " << mapGameObject.hoveredObject << "/" << ship.id <<  std::endl;
                        mapGameObject.allShips.get(ship.id).shipSprite.setColor(sf::Color(190, 255, 190, 255));
                        break;
                    }
                }
                else if (object.type() == typeid(StarSystem)) {
                    StarSystem star = std::any_cast<StarSystem>(object);
                    if (star.star.getGlobalBounds().intersects(mouseB.getGlobalBounds())) {
                        mapGameObject.originHoverColor = star.starColor;
                        mapGameObject.stars.get(star.id).starColor = sf::Color(190, 255, 190, 255);
                        std::cout << "Color of the star after change: " << std::endl;
                        mapGameObject.hoveredObject = star.id;
                        break;
                    }
                }
                else {
                    mapGameObject.hoveredObject = -1;
                }

            }
           

            // start of dragging on press
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                dragging = true;
                dragStart = sf::Vector2i(mouseWorldPos.x, mouseWorldPos.y);
                std::cout << dragStart.x << ", " << dragStart.y << std::endl;
                std::cout << "Now it is preessed \n";
            }

            // Mouse Release:
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                dragging = false;
                // remove selection if not LControl pressed
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                    mapGameObject.cleanSelection();
                   
                    auto widgets = panelSelection->getWidgets();

                    // Iterate through the widgets and remove only those of type tgui::Panel/ clear selection panel
                    for (const auto& widget : widgets) {
                        if (std::dynamic_pointer_cast<tgui::Panel>(widget)) {
                            panelSelection->remove(widget); 
                        }
                    }
                }
                float timeSinceLastClick = double_click_timer.getElapsedTime().asSeconds() - lastClickTime;
                double_click_timer.restart();

                bool isDoubleClick = timeSinceLastClick < DOUBLE_CLICK_TIME && std::hypot(mouseWorldPos.x - lastClickPos.x, mouseWorldPos.y - lastClickPos.y) < DOUBLE_CLICK_RADIUS;
                std::cout << (timeSinceLastClick < DOUBLE_CLICK_TIME) << std::endl;
                std::cout << "Time sinse last click is: " << timeSinceLastClick << std::endl;
                lastClickTime = clock.getElapsedTime().asSeconds();
                lastClickPos = mouseWorldPos;

                DynamicSparseSet<int> selectedObjects;

                std::vector <std::pair<int, int>> relevantCells;

                //Rectangle selection
                if (std::abs(mouseWorldPos.x - dragStart.x) > 5 || std::abs(mouseWorldPos.y - dragStart.y) > 5) {
                    std::cout << "Selection box" << std::endl;
                    mouseWindowOnButtonRelease = sf::Vector2i(mouseWorldPos.x, mouseWorldPos.y);

                    if (dragStart.x > mouseWindowOnButtonRelease.x) {
                        std::swap(dragStart.x, mouseWindowOnButtonRelease.x);
                    }
                    if (dragStart.y > mouseWindowOnButtonRelease.y) {
                        std::swap(dragStart.y, mouseWindowOnButtonRelease.y);
                    }
                    relevantCells = mapGameObject.getFilledCells(sf::Vector2f(dragStart.x, dragStart.y), sf::Vector2f(mouseWindowOnButtonRelease.x - dragStart.x, mouseWindowOnButtonRelease.y - dragStart.y));
                    
                    int x = 0;
                    
                    sf::RectangleShape selectionBox(sf::Vector2f(mouseWindowOnButtonRelease.x - dragStart.x, mouseWindowOnButtonRelease.y - dragStart.y));
                    selectionBox.setPosition(sf::Vector2f(dragStart));
                    for (const auto& cell : relevantCells) {
                        auto& objects = mapGameObject.grid[cell];
                        x += 1;
                        for (auto& vectorElement : objects)
                        {
                            selectedObjects.insert(vectorElement, vectorElement);
                        }


                    }


                }
                //double click selection
                else if (isDoubleClick) {
                    
                    mouseWindowOnButtonRelease = sf::Vector2i(mouseWorldPos.x, mouseWorldPos.y);

                    relevantCells = mapGameObject.getFilledCells(sf::Vector2f(mouseWindowOnButtonRelease.x, mouseWindowOnButtonRelease.y), sf::Vector2f(10, 10));
                    int object_for_selection = -1;
                    DynamicSparseSet<int> objects_for_selection;
                    DynamicSparseSet<int> query = mapGameObject.queryHashMap(mouseWorldPos, 3, 1);
                    for (size_t j = 0; j < query.size(); ++j)
                    {
                        
                        int obj_id = query.getElements()[j];
                        std::any object = mapGameObject.getObject(obj_id);

                        sf::RectangleShape mouseB;
                        mouseB.setSize(sf::Vector2f(3.f, 3.f));
                        mouseB.setPosition(sf::Vector2f(mouseWorldPos));


                        if (object.type() == typeid(SpaceShip)) {
                            SpaceShip ship = std::any_cast<SpaceShip>(object);
                            if (ship.shipSprite.getGlobalBounds().intersects(mouseB.getGlobalBounds())) {
                                for (size_t i = 0; i < mapGameObject.allShips.size(); i++)
                                {
                                    int ship_id = mapGameObject.allShips.getElements()[i].id;
                                    objects_for_selection.insert(ship_id, ship_id);
                                }
                                break;
                            }
                        }
                        else if (object.type() == typeid(StarSystem)) {
                            StarSystem star = std::any_cast<StarSystem>(object);
                            if (star.star.getGlobalBounds().intersects(mouseB.getGlobalBounds())) {
                                for (size_t i = 0; i < mapGameObject.stars.size(); i++)
                                {
                                    int star_id = mapGameObject.stars.getElements()[i].id;
                                    objects_for_selection.insert(star_id, star_id);
                                }
                                break;
                            }
                        }

                        
                    }
                    
                        if (objects_for_selection.size() > 0) {
                            for (size_t e = 0; e < objects_for_selection.size(); ++e)
                            {
                                int id = objects_for_selection.getElements()[e];
                                selectedObjects.insert(id, id);
                            }
                        }
                        
                }
                // single click selection
                else {
                    

                    mouseWindowOnButtonRelease = sf::Vector2i(mouseWorldPos.x, mouseWorldPos.y);

                    relevantCells = mapGameObject.getFilledCells(sf::Vector2f(mouseWindowOnButtonRelease.x, mouseWindowOnButtonRelease.y), sf::Vector2f(10, 10));

                    int object_for_selection = -1;
                    DynamicSparseSet<int> objects_for_selection;
                    DynamicSparseSet<int> query = mapGameObject.queryHashMap(mouseWorldPos, 3, 1);
                    for (size_t j = 0; j < query.size(); ++j)
                    {
                       
                        int obj_id = query.getElements()[j];
                        
                        std::any object = mapGameObject.getObject(obj_id);
                        
                        sf::RectangleShape mouseB;
                        mouseB.setSize(sf::Vector2f(3.f, 3.f));
                        mouseB.setPosition(sf::Vector2f(mouseWorldPos));

                        
                        if (object.type() == typeid(SpaceShip)){
                            SpaceShip ship = std::any_cast<SpaceShip>(object);
                            if (ship.shipSprite.getGlobalBounds().intersects(mouseB.getGlobalBounds())) {
                                object_for_selection = ship.id;
                                break;
                            }
                        }
                        else if (object.type() == typeid(StarSystem)) {
                            StarSystem star = std::any_cast<StarSystem>(object);
                            if (star.star.getGlobalBounds().intersects(mouseB.getGlobalBounds())) {
                                for (size_t i = 0; i < star.ships.size(); i++)
                                {
                                    int ship_id = star.ships.getElements()[i];
                                    objects_for_selection.insert(ship_id, ship_id);
                                }
                                object_for_selection = star.id;
                                break;
                            }
                        }

                       
                    }

                    if (object_for_selection != -1) {
                        if (objects_for_selection.size() > 0) {
                            for (size_t e = 0; e < objects_for_selection.size(); ++e)
                            {
                                int id = objects_for_selection.getElements()[e];
                                selectedObjects.insert(id, id);
                            }
                        }
                        selectedObjects.insert(object_for_selection, object_for_selection);
                    
                    }   
                }

                
                // this function is also sutomatically inserting it inside of selectedStars and ships. Don't know if I should separete it or not though
                mapGameObject.updateSelectionUI(selectedObjects, panelSelection, tguishipTextureShared, tguiSharedTexture);
                
                std::cout << "This is the list of selected objects: " << std::endl;
                selectedObjects.print();
            }

            // logic, when moving ships with right mouse button
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {

                sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);

                sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

                auto cell = mapGameObject.queryHashMap(mouseWorldPos, 3, 0);

                bool star_fly = false;

                int Node_destination = -1;

                for (size_t i = 0; i < cell.size(); ++i)
                {
                    int obj_id = cell.getElements()[i];

                    int objectType = mapGameObject.getTypeObject(obj_id);
                    if (objectType == 1) {
                        star_fly = true;
                        sf::Vector2f pos = mapGameObject.getObjectPosition(obj_id);
                        Node_destination = mapGameObject.stars.get(obj_id).id;
                        mouseWorldPos.x = pos.x;
                        mouseWorldPos.y = pos.y;
                    }

                }
                for (const auto& elem : mapGameObject.selectedShips.getElements())
                {
                    if (mapGameObject.allShips.get(elem).freeFly || star_fly) {
                        
                        int ship_id = mapGameObject.selectedShips.get(elem);
                        
                        if (!mapGameObject.allShips.get(elem).inMotion) {
                            mapGameObject.movingShips.insert(ship_id, ship_id);
                            
                        }
                        
                        int Node_start = mapGameObject.allShips.get(elem).home_node;
                        
                        // creating advanced path. It will be added to ship previous path
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                            
                            if (mapGameObject.allShips.get(elem).MemoryPath.size() > 0) {
                                Node_start = mapGameObject.queryHashMapFirst(mapGameObject.allShips.get(elem).MemoryPath.back(), 3, 1);////
                            }
                        }
                        
                        if (Node_destination != -1) {
                                // a* algorithm
                                std::vector<sf::Vector2f> cordList;
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                                    cordList = mapGameObject.allShips.get(elem).MemoryPath;
                                }
                                std::vector<int> pathFly = mapGameObject.astar_pathfinding(Node_start, Node_destination);
                                for (size_t r = 0; r < pathFly.size(); ++r)
                                {
                                    sf::Vector2f pos(mapGameObject.stars.get(pathFly[r]).starXposMap,
                                        mapGameObject.stars.get(pathFly[r]).starYposMap);
                                    
                                    cordList.push_back(pos);
                                }

                                
                                mapGameObject.allShips.get(elem).UpdateTargetList(cordList);

                                
                            }
                            else {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                                
                                mapGameObject.allShips.get(elem).UpdateTargetList({ mouseWorldPos });
                            }
                            else {

                                mapGameObject.allShips.get(elem).setNewTarget(mouseWorldPos, false);
                            }
                        }

                         
                        if (mapGameObject.selectedShips.size() > 0 && soundOn) { 
                            sound.setBuffer(bufferSendingShips); 
                            sound.play(); 
                        } 
                    }           
                }

            }
            
            // Zooming logic
            if (event.type == sf::Event::MouseWheelScrolled && !isOverGui)
            {
                if (event.mouseWheelScroll.delta > 0) { 
                    
                    if (!(zoomFactor < 0.5)) {
                    zoomFactor *= 0.9f;
                    shaderStar.setUniform("zoomFactor", zoomFactor);

                    }
                }
                else {
                    
                    if(!(zoomFactor > 3)){
                        zoomFactor *= 1.1f;
                        shaderStar.setUniform("zoomFactor", zoomFactor);
                    }
                    
                }

                // Apply new zoom factor
                sf::Vector2u windowSize = window.getSize();
                view.setSize(windowSize.x / zoomFactor, windowSize.y / zoomFactor);
                shaderStar.setUniform("zoomFactor", zoomFactor);
            }
        }

        float deltaTime = clock.restart().asSeconds(); 
        window.clear(sf::Color::Black);
        window.draw(backgroundSprite); 

        // Moving and collision logic
        auto& movingShips = mapGameObject.movingShips.getElements();

        for (size_t i = 0; i < mapGameObject.movingShips.size(); ++i)
        {
            auto elem = movingShips[i];
            int ship_id = mapGameObject.movingShips.get(elem);
            
            int sectorX = (mapGameObject.allShips.get(elem).pos.x) / mapGameObject.sectorSize;
            int sectorY = (mapGameObject.allShips.get(elem).pos.y) / mapGameObject.sectorSize;

            sf::Vector2f oldPos = mapGameObject.allShips.get(elem).pos;
            
            mapGameObject.allShips.get(elem).Move(deltaTime);

            mapGameObject.updateObjectPosition(elem, oldPos, mapGameObject.allShips.get(elem).pos);
            auto& ship = mapGameObject.allShips.get(elem);
            
            if (!ship.inMotion) {
                DynamicSparseSet<int> nearbyObjects = mapGameObject.queryHashMap(ship.pos, 15, elem);

                for (size_t j = 0; j < nearbyObjects.size(); ++j) {
                    const auto& nearbyElement = nearbyObjects.getElements()[j];

                    int objectTypeThis = mapGameObject.getTypeObject(elem);
                    int objectTypeNearby = mapGameObject.getTypeObject(nearbyElement);

                    if (objectTypeThis == 2 && objectTypeNearby == 1) {
                        
                        if (mapGameObject.colisionPointCheck(nearbyElement, ship.endPos)) {
                            if (std::hypot(ship.endPos.x - ship.pos.x, ship.endPos.y - ship.pos.y) < ship.speed * deltaTime) {
                                mapGameObject.stars.get(nearbyElement).ships.insert(elem, elem); 
                                mapGameObject.allShips.get(elem).visiable = false;
                                mapGameObject.allShips.get(elem).home_node = mapGameObject.stars.get(nearbyElement).id;
                                int teamIDstar = mapGameObject.stars.get(nearbyElement).teamID;
                                int teamIDship = mapGameObject.allShips.get(elem).teamID;

                                if (teamIDstar != teamIDship) {
                                    teams.get(teamIDstar).removeStarFromTeam(mapGameObject.stars.get(nearbyElement).id);//.!!!!!
                                    teams.get(teamIDship).addStarToTeam(mapGameObject.stars.get(nearbyElement).id);
                                    if (mapGameObject.stars.get(nearbyElement).id == mapGameObject.hoveredObject) {
                                        mapGameObject.originHoverColor = teams.get(teamIDship).teamColor;
                                    }
                                    else {
                                        mapGameObject.stars.get(nearbyElement).starColor = teams.get(teamIDship).teamColor;
                                    }
                                    mapGameObject.stars.get(nearbyElement).teamID = teamIDship;
                                }


                            }
                            
                            
                        }
                        
                    }
                    
                }

                // After checking if it's inside the star delete it from moving
                mapGameObject.movingShips.erase(elem);
                break;
            }

            DynamicSparseSet<int> nearbyObjects = mapGameObject.queryHashMap(mapGameObject.allShips.get(elem).pos, 15, elem);
            
            if (nearbyObjects.size() > 0) {
                
                auto& nearObjects = nearbyObjects.getElements();
                for (size_t j = 0; j < nearbyObjects.size(); ++j)
                {
                    const auto& nearbyElement = nearObjects[j];

                    int objectTypeThis = mapGameObject.getTypeObject(elem);
                    int objectTypeNearby = mapGameObject.getTypeObject(nearbyElement);
                    bool collides = mapGameObject.colisionCheck(elem, nearbyElement);

                    
                    if (collides) {
                        // coliding two ships
                        if (objectTypeThis == 2 && objectTypeNearby == 2) {
                            
                            auto thisShip = mapGameObject.allShips.get(elem);
                            auto otherShip = mapGameObject.allShips.get(nearbyElement);
                            int thisTeam = thisShip.teamID;
                            int otherTeam = otherShip.teamID;

                            // destroying ships and creating particles of explision
                            if (thisTeam != otherTeam) {
                                std::cout << "Destroying ship with id: " << elem << ", " << nearbyElement << std::endl;

                                if (mapGameObject.hoveredObject == elem || mapGameObject.hoveredObject == nearbyElement) {
                                    mapGameObject.hoveredObject = -1;
                                }
                                Particle first_ship_destr = Particle(explosionTexture, thisShip.pos, 3.f, 64, 512, 512);
                                mapGameObject.addParticle(first_ship_destr);
                                auto& homeStar = mapGameObject.stars.get(thisShip.home_node);
                                std::cout << "Home node on deleding this ship: " << homeStar.id << std::endl;
                                homeStar.ships.erase(thisShip.id);
                                int thisShipteam = thisShip.teamID;
                                teams.get(thisShipteam).removeShipFromTeam(thisShip.id);


                                auto& otherhomeStar = mapGameObject.stars.get(otherShip.home_node);
                                std::cout << "Home node on deleding other ship: " << otherhomeStar.id << std::endl;
                                otherhomeStar.ships.erase(otherShip.id);
                                int otherShipteam = otherShip.teamID;
                                teams.get(otherShipteam).removeShipFromTeam(otherShip.id);

                                mapGameObject.destroyShip(elem);
                                mapGameObject.destroyShip(nearbyElement);
                                nearbyObjects.erase(nearbyElement);
                                break;
                            }

                        }
                        // colision between star and ship
                        if (objectTypeThis == 2 && objectTypeNearby == 1) {
                            if (mapGameObject.colisionPointCheck(nearbyElement, mapGameObject.allShips.get(ship_id).endPos)) {
                                auto ship = mapGameObject.allShips.get(elem); 
                                auto star = mapGameObject.stars.get(nearbyElement);

                                if (ship.shipSprite.getGlobalBounds().intersects(star.smallHitBox.getGlobalBounds())) {
                                    mapGameObject.stars.get(nearbyElement).ships.insert(elem, elem);
                                    mapGameObject.allShips.get(elem).visiable = false;
                                    mapGameObject.allShips.get(elem).home_node = mapGameObject.stars.get(nearbyElement).id;
                                    std::cout << "Placed inside star" << std::endl;
                                    int teamIDstar = mapGameObject.stars.get(nearbyElement).teamID;
                                    int teamIDship = mapGameObject.allShips.get(elem).teamID;

                                    if (teamIDstar != teamIDship) {
                                        teams.get(teamIDstar).removeStarFromTeam(mapGameObject.stars.get(nearbyElement).id);//.!!!!!
                                        teams.get(teamIDship).addStarToTeam(mapGameObject.stars.get(nearbyElement).id);
                                        if (mapGameObject.stars.get(nearbyElement).id == mapGameObject.hoveredObject) {
                                            mapGameObject.originHoverColor = teams.get(teamIDship).teamColor;
                                        }
                                        else {
                                            mapGameObject.stars.get(nearbyElement).starColor = teams.get(teamIDship).teamColor;
                                        }
                                        mapGameObject.stars.get(nearbyElement).teamID = teamIDship;
                                    }
                                }
                                else {
                                    mapGameObject.allShips.get(ship_id).visiable = true;
                                    mapGameObject.stars.get(nearbyElement).ships.erase(ship_id);
                                    
                                }

                            }
                            else {
                                mapGameObject.stars.get(nearbyElement).ships.erase(elem); 
                                mapGameObject.allShips.get(elem).visiable = true; 
                            }

                        }

                    }


                }
            }
            
        }
        
        // Shaders
        shaderStar.setUniform("windowHeight", static_cast<float>(window.getSize().y));
        shaderStar.setUniform("center", sf::Vector2f(500, 400));
        shaderStar.setUniform("color", sf::Glsl::Vec3(1.0f, 1.0f, 1.0f));
        shaderStar.setUniform("radius", 30.0f);
        // To invert Y coordinates
        shaderStar.setUniform("windowHeight", static_cast<float>(window.getSize().y));

        // drawing objects
        mapGameObject.Display(window, shaderStar, zoomFactor);

        // debug dot
        window.draw(dot);
        // debug circle
        window.draw(testShape);
        window.setView(view);
        window.draw(time_text);

        if (mapGameObject.selectedStars.size() < 1 && mapGameObject.selectedShips.size() < 1) {
            panelSelection->setVisible(false);
        }
        else {
            panelSelection->setVisible(true);
        }

        for (size_t i = 0; i < mapGameObject.particleManager.size(); ++i)
        {
            mapGameObject.particleManager.get(i).update(window); 

        }

        // draw rectangle selection box
        if (dragging) {
            sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseWindowPos, view);

            mouseBox.setPosition(sf::Vector2f(dragStart.x, dragStart.y));
            mouseBox.setSize(sf::Vector2f(mouseWorldPos.x - dragStart.x, mouseWorldPos.y - dragStart.y));
            window.draw(mouseBox);
        }

        // draw UI
        gui.draw();
        
        // Update the screen
        window.display(); 
    }

    return 0;
}
