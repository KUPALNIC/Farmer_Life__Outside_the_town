#include "World.hpp"

World::World() {
    int halfSize = worldSize / 2;

    // Генерация биомов
    generateBiome(0, 0, halfSize, halfSize, Biome::PLAINS);
    generateBiome(0, halfSize, halfSize, halfSize, Biome::SWAMP);
    generateBiome(halfSize, halfSize, halfSize, halfSize, Biome::FOREST);
    generateBiome(halfSize, 0, halfSize, halfSize, Biome::MOUNTAINS);

    cell.setSize(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));

    // Инициализация клеток как пустых
    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            cellGrid[y][x] = CellType::EMPTY;
        }
    }
}

void World::generateBiome(int startX, int startY, int width, int height, Biome biome) {
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int y = startY; y < startY + height; ++y) {
        for (int x = startX; x < startX + width; ++x) {
            worldGrid[y][x] = biome;
            colorGrid[y][x] = getRandomBiomeColor(biome);
        }
    }
}

sf::Color World::getRandomBiomeColor(Biome biome) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> variation(0, 50);

    switch (biome) {
    case Biome::PLAINS:
        return sf::Color(150 + variation(rng), 255, 150 + variation(rng));
    case Biome::FOREST:
        return sf::Color(0, 100 + variation(rng), 0);
    case Biome::SWAMP:
        return sf::Color(100 + variation(rng), 70 + variation(rng), 50);
    case Biome::MOUNTAINS:
        return sf::Color(200 + variation(rng), 150 + variation(rng), 100 + variation(rng));
    }
    return sf::Color::White;
}

void World::update(float deltaTime) {
    // Обновление логики мира
}

int World::getCellSize() const {
    return cellSize;
}


void World::render(sf::RenderWindow& window) {
    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            cell.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
            cell.setFillColor(colorGrid[y][x]);
            window.draw(cell);
        }
    }
}

void World::interactWithCell(int gridX, int gridY, CellType type) {
    if (gridX < 0 || gridX >= worldSize || gridY < 0 || gridY >= worldSize) return;

    switch (type) {
    case CellType::BED:
        cellGrid[gridY][gridX] = CellType::BED;
        colorGrid[gridY][gridX] = sf::Color::Green; // Цвет для грядки
        break;
    case CellType::TREE:
        cellGrid[gridY][gridX] = CellType::TREE;
        colorGrid[gridY][gridX] = sf::Color::Blue; // Цвет для дерева
        break;
    case CellType::EMPTY:
        cellGrid[gridY][gridX] = CellType::EMPTY;
        colorGrid[gridY][gridX] = getRandomBiomeColor(worldGrid[gridY][gridX]);
        break;
    }
}

CellType World::getCellType(int gridX, int gridY) const {
    if (gridX < 0 || gridX >= worldSize || gridY < 0 || gridY >= worldSize) return CellType::EMPTY;
    return cellGrid[gridY][gridX];
}