#include "World.hpp"

World::World() {
    // Генерация карты
    int halfSize = worldSize / 2; // Размер половины карты

    // Равнины (левый верхний угол)
    generateBiome(0, 0, halfSize, halfSize, Biome::PLAINS);

    // Болото (нижний левый угол)
    generateBiome(0, halfSize, halfSize, halfSize, Biome::SWAMP);

    // Лес (нижний правый угол)
    generateBiome(halfSize, halfSize, halfSize, halfSize, Biome::FOREST);

    // Горы (верхний правый угол)
    generateBiome(halfSize, 0, halfSize, halfSize, Biome::MOUNTAINS);

    // Настройка клеток
    cell.setSize(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
}

void World::generateBiome(int startX, int startY, int width, int height, Biome biome) {
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int y = startY; y < startY + height; ++y) {
        for (int x = startX; x < startX + width; ++x) {
            worldGrid[y][x] = biome;
            colorGrid[y][x] = getRandomBiomeColor(biome); // Генерируем цвет для клетки

            // TODO: Заменить на спрайты
            // Здесь вы можете загрузить текстуру для текущего биома и присвоить её клетке.
            // Пример:
            // if (biome == Biome::PLAINS) {
            //     cell.setTexture(&plainsTexture);
            // }
        }
    }
}

sf::Color World::getRandomBiomeColor(Biome biome) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> variation(0, 50); // Для небольшого изменения цвета

    switch (biome) {
    case Biome::PLAINS: // Светлые оттенки зеленого
        return sf::Color(150 + variation(rng), 255, 150 + variation(rng));
    case Biome::FOREST: // Темные оттенки зеленого
        return sf::Color(0, 100 + variation(rng), 0);
    case Biome::SWAMP: // Коричневые с зеленым
        return sf::Color(100 + variation(rng), 70 + variation(rng), 50);
    case Biome::MOUNTAINS: // Оранжевые с серым
        return sf::Color(200 + variation(rng), 150 + variation(rng), 100 + variation(rng));
    }
    return sf::Color::White; // Дефолтный цвет
}

void World::update(float deltaTime) {
    // Логика обновления мира
}

void World::render(sf::RenderWindow& window) {
    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            cell.setPosition({static_cast<float>(x * cellSize), static_cast<float>(y * cellSize)});
            cell.setFillColor(colorGrid[y][x]); // Устанавливаем цвет клетки из заранее сгенерированного массива

            // TODO: Заменить на спрайты
            // Здесь вы можете установить текстуру вместо цвета:
            // cell.setTexture(&yourTexture);
            window.draw(cell);
        }
    }
}