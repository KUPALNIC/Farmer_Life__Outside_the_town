#include "World.hpp"


World::World() {
    int halfSize = worldSize / 2;
    tilesetTexture.loadFromFile("../assets/textures/floortileset.png");
    bedTexture.loadFromFile("../assets/textures/bed.jpg");
    HouseTexture.loadFromFile("../assets/textures/House.png");
    generateBiome(0, 0, halfSize, halfSize, Biome::PLAINS);
    generateBiome(0, halfSize, halfSize, halfSize, Biome::SWAMP);
    generateBiome(halfSize, halfSize, halfSize, halfSize, Biome::FOREST);
    generateBiome(halfSize, 0, halfSize, halfSize, Biome::MOUNTAINS);
    cell.setSize(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            cellGrid[y][x] = CellType::EMPTY;
            bedWatered[y][x] = false;
            biomeOpened[y][x] = (worldGrid[y][x] == Biome::PLAINS);
            bedGrid[y][x] = BedGrid::NONE;
        }
    }

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

}

int World::getCellSize() const {
    return cellSize;
}

void World::waterBed(int x, int y) {
    if (cellGrid[y][x] == CellType::BED)
        bedWatered[y][x] = true;
    auto it = beds.find({x, y});
    if (it != beds.end()) {
        it->second.water();
    }
}
void World::dryBed(int x, int y) {
    if (cellGrid[y][x] == CellType::BED)
        bedWatered[y][x] = false;
}
bool World::isWatered(int x, int y) {
    if (cellGrid[y][x] == CellType::BED && bedWatered[y][x]) return true;
    auto it = beds.find({x, y});
    if (it != beds.end()) {
        return it->second.isWatered();
    }
}

void World::render(sf::RenderWindow& window) {
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
                shadow.setFillColor(sf::Color(0, 0, 0, 128));
                window.draw(shadow);
            }

            if (cellGrid[y][x] == CellType::BED) {
                sf::Sprite bedSprite(bedTexture);
                bedSprite.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                window.draw(bedSprite);
                if (bedWatered[y][x]) {
                    sf::RectangleShape shadow(sf::Vector2f(cellSize, cellSize));
                    shadow.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
                    shadow.setFillColor(sf::Color(0, 0, 0, 128));
                    window.draw(shadow);

                }

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

void World::createBed(int x, int y) {
    beds[{x, y}] = Bed();
}

void World::removeBed(int x, int y) {
    beds.erase({x, y});
}

Bed* World::getBedAt(int x, int y) {
    auto it = beds.find({x, y});
    return (it != beds.end()) ? &it->second : nullptr;
}

void World::updateBeds(float dt) {
    for (auto& [pos, bed] : beds) {
        bed.update(dt);
    }
}

void World::renderBeds(sf::RenderWindow& window, sf::Texture& cropTexture) {
    for (const auto& [pos, bed] : beds) {
        sf::Vector2f pixelPos(pos.first * 32.0f, pos.second * 32.0f); // если размер клетки 32
        bed.render(window, cropTexture, pixelPos);
    }
}
