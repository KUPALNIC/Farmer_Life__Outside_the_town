#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "/home/kupalnic/CLionProjects/Farmer Life: Outside the town/src/Objects/Tool.hpp"
#include "World.hpp"

class Player {
public:
    Player(World& worldRef);

    // void handleInput(sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    World& world;
    sf::Vector2f position;
    sf::Texture forwardTexture;
    sf::Texture backwardTexture;
    sf::Sprite PlayerSprite;
    float currentFrame;
    bool facingLeft;

    std::array<Tool*, 9> hotbar{};
    int selectedSlot;
};