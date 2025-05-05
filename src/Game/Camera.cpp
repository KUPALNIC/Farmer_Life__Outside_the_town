#include "Camera.hpp"


Camera::Camera() : zoomLevel(1.0f) {
    view.setSize({800, 600});
    view.setCenter({400, 300});
}

void Camera::handleInput(const sf::Event& event) {
    if (event.is<sf::Event::MouseWheelScrolled>()) {
        if (sf::Event::MouseWheelScrolled::delta > 0) {
            zoomLevel *= 0.9f;
        } else {
            zoomLevel *= 1.1f;
        }
        view.zoom(zoomLevel);
    }
}

void Camera::update(float deltaTime, const sf::Vector2f& targetPosition) {
    view.setCenter(targetPosition);
}

void Camera::apply(sf::RenderWindow& window) {
    window.setView(view);
}