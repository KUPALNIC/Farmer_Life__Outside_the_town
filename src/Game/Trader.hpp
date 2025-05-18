#pragma once
#include <SFML/Graphics.hpp>

class Trader {
public:
    Trader(const sf::Vector2f& position);

    void render(sf::RenderWindow& window, sf::Texture& texture);
    const sf::Vector2f& getPosition() const { return position; }

private:
    sf::Vector2f position;
};