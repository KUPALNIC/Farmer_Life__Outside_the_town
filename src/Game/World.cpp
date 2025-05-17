#include "World.hpp"


World::World() {
    int halfSize = worldSize / 2;

    tilesetTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/floortileset.png");
    bedTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/bed.jpg");
    HouseTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/House.png");

    generateBiome(0, 0, halfSize, halfSize, Biome::PLAINS);
    generateBiome(0, halfSize, halfSize, halfSize, Biome::SWAMP);
    generateBiome(halfSize, halfSize, halfSize, halfSize, Biome::FOREST);
    generateBiome(halfSize, 0, halfSize, halfSize, Biome::MOUNTAINS);

    cell.setSize(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));

    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            cellGrid[y][x] = CellType::EMPTY;
        }
    }
    for (int y = 0; y < worldSize; ++y)
        for (int x = 0; x < worldSize; ++x)
            biomeOpened[y][x] = (worldGrid[y][x] == Biome::PLAINS);
}

bool World::isBiomeOpened(int gridX, int gridY) const {
    if (gridX < 0 || gridX >= worldSize || gridY < 0 || gridY >= worldSize) return false;
    return biomeOpened[gridY][gridX];
}

// Получить биом по мировой позиции
Biome World::getBiomeAt(float x, float y) const {
    int gridX = int(x) / cellSize;
    int gridY = int(y) / cellSize;
    if (gridX < 0 || gridX >= worldSize || gridY < 0 || gridY >= worldSize) return Biome::PLAINS;
    return worldGrid[gridY][gridX];
}

// Заготовка для условия открытия биома
void World::tryOpenBiome(Biome biome) {
    // TODO: тут будет условие открытия
    for (int y = 0; y < worldSize; ++y)
        for (int x = 0; x < worldSize; ++x)
            if (worldGrid[y][x] == biome)
                biomeOpened[y][x] = true;
}

void World::generateBiome(int startX, int startY, int width, int height, Biome biome) {
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int y = startY; y < startY + height; ++y) {
        for (int x = startX; x < startX + width; ++x) {
            worldGrid[y][x] = biome;
            colorGrid[y][x] = getRandomBiomeColor(biome);

            if (biome == Biome::PLAINS) {
                std::uniform_int_distribution<int> dist(0, 7); // 8 вариантов
                plainsGrassIndex[y][x] = dist(rng);
            }
            if (biome == Biome::SWAMP) {
                std::uniform_int_distribution<int> dist(0, 3); // 4 варианта
                swampGrassIndex[y][x] = dist(rng);
            }
            if (biome == Biome::FOREST) {
                std::uniform_int_distribution<int> dist(0, 3); // 4 варианта
                forestGrassIndex[y][x] = dist(rng);
            }
            if (biome == Biome::MOUNTAINS) {
                // 9 уникальных вариантов: 0..4 на Y=0, 0,2,3,4 на Y=1
                std::vector<std::pair<int,int>> mountainTiles = {
                    {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, // первая строка
                    {0,1}, {2,1}, {3,1}, {4,1}         // вторая строка
                };
                std::uniform_int_distribution<int> dist(0, mountainTiles.size() - 1);
                mountainGrassIndex[y][x] = dist(rng);
            }
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
    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            switch (cellGrid[y][x]) {
                case CellType::BED:
                    colorGrid[y][x] = sf::Color::Magenta;
                break;
                case CellType::TREE:
                    colorGrid[y][x] = sf::Color::Blue;
                break;
                default:
                break;
            }
        }
    }
}

int World::getCellSize() const {
    return cellSize;
}

void World::render(sf::RenderWindow& window) {
    // Координаты тайлов для гор
    static const std::vector<std::pair<int,int>> mountainTiles = {
        {0,0}, {1,0}, {2,0}, {3,0}, {4,0},
        {0,1}, {2,1}, {3,1}, {4,1}
    };

    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            Biome biome = worldGrid[y][x];
            sf::Sprite tileSprite(tilesetTexture);

            if (biome == Biome::PLAINS) {
                int idx = plainsGrassIndex[y][x];
                tileSprite.setTextureRect(sf::IntRect({idx * 32, 7 * 32}, {32, 32}));
                tileSprite.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                window.draw(tileSprite);
            } else if (biome == Biome::SWAMP) {
                int idx = swampGrassIndex[y][x];
                tileSprite.setTextureRect(sf::IntRect({idx * 32, 6 * 32}, {32, 32}));
                tileSprite.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                window.draw(tileSprite);
            } else if (biome == Biome::FOREST) {
                int idx = forestGrassIndex[y][x];
                tileSprite.setTextureRect(sf::IntRect({idx * 32, 2 * 32}, {32, 32}));
                tileSprite.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                window.draw(tileSprite);
            } else if (biome == Biome::MOUNTAINS) {
                int idx = mountainGrassIndex[y][x];
                int tx = mountainTiles[idx].first;
                int ty = mountainTiles[idx].second;
                tileSprite.setTextureRect(sf::IntRect({tx * 32, ty * 32}, {32, 32}));
                tileSprite.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                window.draw(tileSprite);
            } else {
                // fallback (на всякий случай)
                cell.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                cell.setFillColor(colorGrid[y][x]);
                window.draw(cell);
            }
            if (!biomeOpened[y][x]) {
                sf::RectangleShape shadow(sf::Vector2f(cellSize, cellSize));
                shadow.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                shadow.setFillColor(sf::Color(0, 0, 0, 128)); // 128 = ~50% прозрачности
                window.draw(shadow);
            }

            if (cellGrid[y][x] == CellType::BED) {
                sf::Sprite bedSprite(bedTexture);
                bedSprite.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                window.draw(bedSprite);
            }
        }
    }
}

void World::interactWithCell(int gridX, int gridY, CellType type) {
    if (gridX < 0 || gridX >= worldSize || gridY < 0 || gridY >= worldSize) return;

    switch (type) {
    case CellType::BED:
        cellGrid[gridY][gridX] = CellType::BED;
        colorGrid[gridY][gridX] = sf::Color::Magenta;
        break;
    case CellType::TREE:
        cellGrid[gridY][gridX] = CellType::TREE;
        colorGrid[gridY][gridX] = sf::Color::Blue;
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
