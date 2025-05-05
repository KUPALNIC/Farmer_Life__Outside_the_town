#pragma once
#include <SFML/Graphics.hpp>
#include <array>

enum class Biome { PLAINS, FOREST, SWAMP, MOUNTAINS };

class World {
public:
    World();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

private:
    const int worldSize = 150;
    std::array<std::array<Biome, 150>, 150> worldGrid;

    sf::RectangleShape cell; // Временный спрайт клетки
};