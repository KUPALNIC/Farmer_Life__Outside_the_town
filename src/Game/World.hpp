#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <map>
#include "../Objects/Bed.hpp"

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
enum class BedGrid {NONE, BED};

class World {
public:
    World();
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    int getCellSize() const;
    void waterBed(int x, int y);

    void dryBed(int x, int y);

    bool isWatered(int x, int y);
    void interactWithCell(int gridX, int gridY, CellType type);
    CellType getCellType(int gridX, int gridY) const;
    Biome getBiomeAt(float x, float y) const;
    bool isBiomeOpened(int gridX, int gridY) const;
    void tryOpenBiome(Biome biome);
    void createBed(int x, int y);
    void removeBed(int x, int y);
    Bed* getBedAt(int x, int y);
    void updateBeds(float dt);
    void renderBeds(sf::RenderWindow& window, sf::Texture& cropTexture);

    static constexpr int worldSize = 100;
    static constexpr int cellSize = 32;


private:
    // const int worldSize = 100;
    // const int cellSize = 32;
    std::array<std::array<Biome, 100>, 100> worldGrid;
    std::array<std::array<CellType, 100>, 100> cellGrid;
    std::array<std::array<sf::Color, 100>, 100> colorGrid;

    // Индексы для всех биомов (индекс по клетке, который спрайт брать)
    std::array<std::array<int, 100>, 100> plainsGrassIndex;
    std::array<std::array<int, 100>, 100> swampGrassIndex;
    std::array<std::array<int, 100>, 100> forestGrassIndex;
    std::array<std::array<int, 100>, 100> mountainGrassIndex;
    std::array<std::array<bool, worldSize>, worldSize> biomeOpened;
    std::array<std::array<bool, worldSize>, worldSize> bedWatered;
    std::array<std::array<BedGrid, worldSize>, worldSize> bedGrid;
    std::map<std::pair<int, int>, Bed> beds;

    sf::RectangleShape cell;
    sf::Texture tilesetTexture;
    sf::Texture bedTexture;
    sf::Texture HouseTexture;

    void generateBiome(int startX, int startY, int width, int height, Biome biome);
    sf::Color getRandomBiomeColor(Biome biome);
};
