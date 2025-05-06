#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Inventory {
public:
    Inventory(int slots, float slotSize, const sf::Vector2f& position);

    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void updatePosition(const sf::Vector2u& windowSize);

    int getSelectedSlot() const;

private:
    int selectedSlot;                       // Текущий выбранный слот
    int totalSlots;                         // Общее количество слотов
    float slotSize;                         // Размер одного слота
    sf::Vector2f position;                  // Начальная позиция панели
    std::vector<sf::RectangleShape> slots;  // Прямоугольники для слотов
};