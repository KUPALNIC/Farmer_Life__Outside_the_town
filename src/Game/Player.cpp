#include "Player.hpp"

Player::Player() : position(400.0f, 300.0f), selectedSlot(0) {
    sprite.setRadius(16.0f);
    sprite.setFillColor(sf::Color::Green);
    sprite.setPosition(position);
}

void Player::handleInput(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        if (event.is<sf::Keyboard::Key> >= sf::Keyboard::Key::Num1 && event.key.code <= sf::Keyboard::Key::Num9) {
            selectedSlot = event.is<sf::Keyboard::Key>() - sf::Keyboard::Key::Num1;
        }
    }
}

void Player::update(float deltaTime) {
    const float speed = 200.0f;
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += speed * deltaTime;

    position += movement;
    sprite.setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const {
    return position;
}