#include "Player.hpp"


Player::Player() : position(400.0f, 300.0f), PlayerSprite(PlayerTexture),selectedSlot(0) {
    PlayerTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/forward.png");
    sf::Sprite PlayerSprite(PlayerTexture);
    PlayerSprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
    PlayerSprite.setPosition(position);

}



void Player::handleInput(const sf::Event& event) {
    if (auto keyPressEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Convert key codes to integers for comparison
        auto keyCode = static_cast<std::underlying_type_t<sf::Keyboard::Key>>(keyPressEvent->code);
        auto num1Code = static_cast<std::underlying_type_t<sf::Keyboard::Key>>(sf::Keyboard::Key::Num1);

        if (keyCode >= num1Code && keyCode <= num1Code + 8) {
            selectedSlot = keyCode - num1Code;
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
    PlayerSprite.setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(PlayerSprite);
}

sf::Vector2f Player::getPosition() const {
    return position;
}