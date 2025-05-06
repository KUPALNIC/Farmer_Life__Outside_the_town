#pragma once

#include <SFML/Graphics.hpp>

class Bed {
public:
    Bed();

    void plantCrop();
    void render(sf::RenderWindow& window);

    bool hasCrop() const;

private:
    sf::RectangleShape sprite; // Временное представление
    bool occupied; // Занята ли грядка
};