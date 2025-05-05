#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Tool.hpp"

class Player {
public:
    Player();

    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;

private:
    sf::Vector2f position;
    sf::CircleShape sprite; // Временный спрайт игрока

    std::array<Tool*, 9> hotbar; // Слоты инвентаря
    int selectedSlot; // Текущий выбранный слот
};