#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class Camera {
public:
    Camera();

    void handleInput(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& targetPosition);
    void apply(sf::RenderWindow& window);

private:
    sf::View view;
    float zoomLevel;
};