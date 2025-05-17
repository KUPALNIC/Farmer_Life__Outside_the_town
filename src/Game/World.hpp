#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <random>

enum class Biome {
    PLAINS,
    FOREST,
    SWAMP,
    MOUNTAINS
};

enum class CellType {
    EMPTY,
    BED,
    TREE
};

class World {
public:
    World();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    int getCellSize() const;
    void interactWithCell(int gridX, int gridY, CellType type);
    CellType getCellType(int gridX, int gridY) const;

private:
    const int worldSize = 100;
    const int cellSize = 32;
    std::array<std::array<Biome, 100>, 100> worldGrid;
    std::array<std::array<CellType, 100>, 100> cellGrid;
    std::array<std::array<sf::Color, 100>, 100> colorGrid;

    // Индексы для всех биомов (индекс по клетке, который спрайт брать)
    std::array<std::array<int, 100>, 100> plainsGrassIndex;
    std::array<std::array<int, 100>, 100> swampGrassIndex;
    std::array<std::array<int, 100>, 100> forestGrassIndex;
    std::array<std::array<int, 100>, 100> mountainGrassIndex;

    sf::RectangleShape cell;
    sf::Texture tilesetTexture;
    sf::Texture bedTexture;
    sf::Texture HouseTexture;

    void generateBiome(int startX, int startY, int width, int height, Biome biome);
    sf::Color getRandomBiomeColor(Biome biome);
};
