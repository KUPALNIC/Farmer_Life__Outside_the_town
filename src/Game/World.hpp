#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <random>

enum class Biome { PLAINS, FOREST, SWAMP, MOUNTAINS };
enum class CellType { EMPTY, BED, TREE }; // Типы клеток

class World {
public:
    World();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    int getCellSize() const;
    // Новые методы для взаимодействия
    void interactWithCell(int gridX, int gridY, CellType type);
    CellType getCellType(int gridX, int gridY) const;
    private:
    const int worldSize = 100; // Размер карты (в клетках)
    const int cellSize = 32;   // Размер клетки (в пикселях)
    std::array<std::array<Biome, 100>, 100> worldGrid;  // Матрица биомов
    std::array<std::array<CellType, 100>, 100> cellGrid; // Типы клеток
    std::array<std::array<sf::Color, 100>, 100> colorGrid; // Цвета клеток
    sf::RectangleShape cell; // Временный спрайт клетки

    void generateBiome(int startX, int startY, int width, int height, Biome biome);
    sf::Color getRandomBiomeColor(Biome biome);
};