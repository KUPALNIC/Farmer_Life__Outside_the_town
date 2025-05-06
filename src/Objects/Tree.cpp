#include "Tree.hpp"

Tree::Tree() : growthStage(0.0f) {
    sprite.setSize({32.0f, 64.0f}); // Дерево выше, чем другие объекты
    sprite.setFillColor(sf::Color(50, 150, 50)); // Зеленый оттенок
}

void Tree::grow(float deltaTime) {
    growthStage += deltaTime * 0.05f; // Медленный рост
    if (growthStage > 1.0f) growthStage = 1.0f;

    // Изменение высоты дерева
    sprite.setSize({32.0f, 32.0f + 32.0f * growthStage});
}

void Tree::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Tree::canBeChopped() const {
    return growthStage >= 1.0f;
}