#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <random>

enum class Biome { PLAINS, FOREST, SWAMP, MOUNTAINS };

class World {
public:
    World();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

private:
    const int worldSize = 150; // Размер карты (в клетках)
    const int cellSize = 32;   // Размер клетки (в пикселях)

    std::array<std::array<Biome, 150>, 150> worldGrid;  // Матрица биомов
    std::array<std::array<sf::Color, 150>, 150> colorGrid; // Цвета клеток биомов
    sf::RectangleShape cell;                            // Временный спрайт клетки

    // Генерация биома для определенной области
    void generateBiome(int startX, int startY, int width, int height, Biome biome);

    // Получение случайного цвета для конкретного биома
    sf::Color getRandomBiomeColor(Biome biome);

    // TODO: Заменить квадраты на спрайты
    // Вы можете добавить сюда std::map<Biome, sf::Texture> для хранения текстур.
};