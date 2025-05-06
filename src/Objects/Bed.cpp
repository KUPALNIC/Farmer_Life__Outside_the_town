#include "Bed.hpp"

Bed::Bed() : occupied(false) {
    sprite.setSize({32.0f, 32.0f});
    sprite.setFillColor(sf::Color::Magenta);
}

void Bed::plantCrop() {
    if (!occupied) {
        occupied = true;
        sprite.setFillColor(sf::Color::Green); // Условно: грядка занята
    }
}

void Bed::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Bed::hasCrop() const {
    return occupied;
}