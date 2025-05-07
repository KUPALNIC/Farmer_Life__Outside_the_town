#include "Crop.hpp"

Crop::Crop() : growthStage(0.0f) {
    sprite.setSize({32.0f, 32.0f});
    sprite.setFillColor(sf::Color::Green);
}

void Crop::grow(float deltaTime) {
    growthStage += deltaTime * 0.1f; // Рост со временем
    if (growthStage > 1.0f) growthStage = 1.0f;

    // Изменение цвета в зависимости от стадии роста
    sprite.setFillColor(sf::Color(0, static_cast<int8_t>(255 * growthStage), 0));
}

void Crop::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Crop::isReadyToHarvest() const {
    return growthStage >= 1.0f;
}

void Crop::plant() {
    growthStage = 0.0f; // Сброс стадии роста при посеве
    sprite.setFillColor(sf::Color::Red);
}