#pragma once
#include <SFML/Graphics.hpp>
#include </home/kupalnic/CLionProjects/Farmer Life: Outside the town/src/UI/Inventory.hpp>

class Camera {
public:
    Camera();

    void handleInput(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& targetPosition);
    void apply(sf::RenderWindow& window);
    void updateInventoryPosition(Inventory& inventory) {
        inventory.updatePosition(view);
    }

private:
    sf::View view;
    float zoomLevel;
    float targetZoomLevel; // Целевое значение зума
    const float zoomSpeed = 2.0f; // Скорость изменения зума
    const float minZoom = 0.5f; // Минимальный зум
    const float maxZoom = 5.0f; // Максимальный зум
};