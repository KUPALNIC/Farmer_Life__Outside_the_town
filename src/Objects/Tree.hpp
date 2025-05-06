#pragma once

#include <SFML/Graphics.hpp>

class Tree {
public:
    Tree();

    void grow(float deltaTime);
    void render(sf::RenderWindow& window);

    bool canBeChopped() const;

private:
    float growthStage;      // Стадия роста
    sf::RectangleShape sprite; // Временное представление
};