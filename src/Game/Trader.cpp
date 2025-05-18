#include "Trader.hpp"

Trader::Trader(const sf::Vector2f& pos) : position(pos) {}

void Trader::render(sf::RenderWindow& window, sf::Texture& texture) {
    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    sprite.setScale({1.f, 1.f}); // 64x64 пикселя
    window.draw(sprite);
}