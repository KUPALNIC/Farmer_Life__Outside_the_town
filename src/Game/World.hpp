#pragma once
#include <SFML/Graphics.hpp>
#include <array>

enum class Biome { PLAINS, FOREST, SWAMP, MOUNTAINS };
// написать класс biom у которго будет функция распределения клеток, из трех или четырех различных спрайтов, а от него дочерние классы
class World {
public:
    World();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

private:
    const int worldSize = 150;
    std::array<std::array<Biome, 150>, 150> worldGrid;

    // sf::RectangleShape cell; // Временный спрайт клетки

};