#pragma once
#include <iostream>
#include <any>
#include <variant>
#include <algorithm>
#include <tuple>
#include "DynamicSparseSet.h"
#include <cassert>
#include <unordered_map>
#include <cmath>
#include <utility>  // For std::pair
#include <TGUI/TGUI.hpp>
#include <TGUI/Core.hpp>          // Core system for GUI management
#include <TGUI/Widgets/Button.hpp> // Include specific widget headers, like Button
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>


class Particle
{
public:
	Particle(const sf::Texture& texture, sf::Vector2f position, float duration, int frames, int frameWidth, int frameHeight);

	sf::Clock timer;
	const sf::Texture& texture; 
	sf::Vector2f position;
	int id;
	float duration;
	int frames;
	int frameWidth;
	int frameHeight;
	sf::Sprite sprite; 
	float elapsed = 0.f;      // Time since last frame change
	int currentFrame = 0;

	void update(sf::RenderWindow& win);


	bool isAlive() const;



};
