#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera();

    void handleInput(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& targetPosition);
    void apply(sf::RenderWindow& window);

private:
    sf::View view;
    float zoomLevel;
    float targetZoomLevel; // Целевое значение зума
    const float zoomSpeed = 2.0f; // Скорость изменения зума
    const float minZoom = 0.5f; // Минимальный зум
    const float maxZoom = 5.0f; // Максимальный зум
};