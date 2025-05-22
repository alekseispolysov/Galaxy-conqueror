
#include "Particle.h"
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

Particle::Particle(const sf::Texture& texture, sf::Vector2f position, float duration, int frames, int frameWidth, int frameHeight)
    : texture(texture), position(position), duration(duration), frames(frames), frameWidth(frameWidth), frameHeight(frameHeight)
{
    sprite.setTexture(this->texture);
    sprite.setPosition(this->position);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(sf::Vector2f(0.05, 0.05));
}

// will accept window, where to draw particle
void Particle::update(sf::RenderWindow& win)
{
    // Calculate total duration for a single frame
    float frameDuration = duration / frames;

    // Update time
    elapsed += timer.restart().asSeconds();

    // Advance frame if needed
    if (elapsed >= frameDuration && currentFrame < frames) {
        ++currentFrame;
        elapsed = 0.f;

        // Update texture rect (assuming horizontal layout)
        int x = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(x, 0, frameWidth, frameHeight));
    }

    // Draw the sprite
    win.draw(sprite);
}

bool Particle::isAlive() const
{
    return timer.getElapsedTime().asSeconds() < duration;
}
