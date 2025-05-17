#include "Player.hpp"
#include "../UI/Inventory.hpp"


Player::Player(World& worldRef) : world(worldRef),
                                  position(400.0f, 300.0f),
                                  currentFrame(0.0f),
                                  facingLeft(false),
                                  PlayerSprite(forwardTexture){
    forwardTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/forward.png");
    backwardTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/backward.png");
    PlayerSprite.setTexture(forwardTexture);
    PlayerSprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
    PlayerSprite.setOrigin({16.0f, 16.0f});
    PlayerSprite.setPosition(position);
    PlayerSprite.setScale({2.0f, 2.0f});
    extern sf::Texture toolsTexture;
    toolsTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/tools.png");
    hotbar[0] = new Tool(Tool::Type::Axe, toolsTexture, sf::IntRect({0,0},{16,16}));
    hotbar[1] = new Tool(Tool::Type::Hoe, toolsTexture, sf::IntRect({16,0},{16,16}));
    hotbar[2] = new Tool(Tool::Type::WateringCan, toolsTexture, sf::IntRect({32,0},{16,16}));

}

void Player::update(float deltaTime) {
    const float speed = 300.0f;
    sf::Vector2f movement(0.0f, 0.0f);
    bool isMoving = false;

    // Handle movement and animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movement.y -= speed * deltaTime;
        PlayerSprite.setTexture(backwardTexture);
        currentFrame +=  0.1f * deltaTime * 60.0f;
        if (currentFrame >= 4.0f) currentFrame -= 4.0f;
        PlayerSprite.setTextureRect(sf::IntRect({32 * static_cast<int>(currentFrame), 0}, {32, 32}));
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += speed * deltaTime;
        PlayerSprite.setTexture(forwardTexture);
        currentFrame +=  0.1f * deltaTime * 60.0f;
        if (currentFrame >= 4.0f) currentFrame -= 4.0f;
        PlayerSprite.setTextureRect(sf::IntRect({32 * static_cast<int>(currentFrame), 0}, {32, 32}));
        isMoving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        movement.x -= speed * deltaTime;
        PlayerSprite.setTexture(forwardTexture);
        currentFrame +=  0.1f * deltaTime * 60.0f;
        if (currentFrame >= 4.0f) currentFrame -= 4.0f;
        PlayerSprite.setTextureRect(sf::IntRect({32 * static_cast<int>(currentFrame), 0}, {32, 32}));
        PlayerSprite.setScale({-2.0f, 2.0f}); // Flip sprite horizontally
        facingLeft = true;
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += speed * deltaTime;
        PlayerSprite.setTexture(forwardTexture);
        currentFrame +=  0.1f * deltaTime * 60.0f;
        if (currentFrame >= 4.0f) currentFrame -= 4.0f;
        PlayerSprite.setTextureRect(sf::IntRect({32 * static_cast<int>(currentFrame), 0}, {32, 32}));
        PlayerSprite.setScale({2.0f, 2.0f}); // Normal scale
        facingLeft = false;
        isMoving = true;
    }

    // Update position
    sf::Vector2f newPosition = position + movement;
    PlayerSprite.setPosition(newPosition);
    // Ограничение по границе карты:
    newPosition.x = std::max(0.f, std::min(newPosition.x, this->world.worldSize * this->world.cellSize - 1.f));
    newPosition.y = std::max(0.f, std::min(newPosition.y, this->world.worldSize * this->world.cellSize - 1.f));

    // Ограничение по открытому биому:
    Biome newBiome = this->world.getBiomeAt(newPosition.x, newPosition.y);
    int gridX = int(newPosition.x) / this->world.cellSize;
    int gridY = int(newPosition.y) / this->world.cellSize;
    if (this->world.isBiomeOpened(gridX, gridY)) {
        position = newPosition;
        PlayerSprite.setPosition(position);
    }
    // иначе — стоим на месте
    // If not moving, reset to idle frame
    if (!isMoving) {
        currentFrame = 0;
        PlayerSprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
    }
}

void Player::setSelectedSlot(int idx) {
    if (idx >= 0 && idx < hotbar.size())
        selectedSlot = idx;
}
int Player::getSelectedSlotIndex() const {
    return selectedSlot;
}

void Player::render(sf::RenderWindow& window) {
    window.draw(PlayerSprite);
    Tool* tool = hotbar[selectedSlot];
    if (tool && tool->getType() != Tool::Type::None) {
        sf::Vector2f toolPos = position + sf::Vector2f(0, 0); // смещение подбери по вкусу
        tool->render(window, toolPos, 1.5f); // инструменты мельче персонажа
    }
}

sf::Vector2f Player::getPosition() const {
    return position;
}

Tool* Player::getSelectedTool() const {

    if (selectedSlot >= 0 && selectedSlot < hotbar.size())
        return hotbar[selectedSlot];
    return nullptr;
}

