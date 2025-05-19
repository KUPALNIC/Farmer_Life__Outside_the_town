#include "Camera.hpp"
#include <algorithm>
Camera::Camera() : zoomLevel(1.0f), targetZoomLevel(1.0f) {
    view.setSize({800, 600});
    view.setCenter({400, 300});
}

void Camera::handleInput(const sf::Event& event) {
    if (auto mouseWheelEvent = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheelEvent->delta > 0) {  // Приближение
            targetZoomLevel *= 0.9f;
        } else {  // Отдаление
            targetZoomLevel *= 1.1f;
        }

        // Ограничиваем целевое значение зума
        targetZoomLevel = std::clamp(targetZoomLevel, minZoom, maxZoom);
    }
}

void Camera::update(float deltaTime, const sf::Vector2f& targetPosition) {
    // Плавное изменение зума
    if (zoomLevel != targetZoomLevel) {
        float zoomChange = zoomSpeed * deltaTime;
        if (zoomLevel < targetZoomLevel) {
            zoomLevel = std::min(zoomLevel + zoomChange, targetZoomLevel);
        } else {
            zoomLevel = std::max(zoomLevel - zoomChange, targetZoomLevel);
        }

        // Обновляем размеры вида в соответствии с zoomLevel
        view.setSize({800 * zoomLevel, 600 * zoomLevel});
    }

    // Центрируем камеру на позиции игрока
    view.setCenter(targetPosition);
}

void Camera::apply(sf::RenderWindow& window) {
    window.setView(view);
}