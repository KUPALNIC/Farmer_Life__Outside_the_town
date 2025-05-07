#pragma once

#include <SFML/Graphics.hpp>

class Crop {
public:
    Crop();

    void grow(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isReadyToHarvest() const;

    // Новый метод для посева
    void plant();

private:
    float growthStage;      // Текущая стадия роста (от 0 до 1)
    sf::RectangleShape sprite; // Временное представление для тестирования
};