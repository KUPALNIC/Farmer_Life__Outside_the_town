#include "World.hpp"

World::World() {
    // Инициализация мира
    for (int y = 0; y < worldSize/2; ++y) {
        for (int x = 0; x < worldSize/2; ++x) {
            worldGrid[y][x] = Biome::PLAINS;
            worldGrid[y+worldSize/2][x] = Biome::FOREST;
            worldGrid[y][x+worldSize/2] = Biome::SWAMP;
            worldGrid[y+worldSize/2][x+worldSize/2] = Biome::MOUNTAINS;
        }
    }

    cell.setSize(sf::Vector2f(32.0f, 32.0f));
    cell.setFillColor(sf::Color::Green);
}

void World::update(float deltaTime) {
    // Логика обновления мира
}

void World::render(sf::RenderWindow& window) {
    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            cell.setPosition({x * 32.0f, y * 32.0f});
            window.draw(cell);
        }
    }
}