#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "../Objects/Tool.hpp" // подключаем Tool

class Inventory {
public:
    Inventory(int slots, float slotSize, const sf::Vector2f& position);

    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window, const std::array<Tool*, 9>& hotbar);
    void updatePosition(const sf::Vector2u& windowSize);
    int getSelectedSlot() const;

private:
    int selectedSlot;
    int totalSlots;
    float slotSize;
    sf::Vector2f position;
    std::vector<sf::RectangleShape> slots;
};