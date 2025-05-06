#include "Inventory.hpp"

#include <iostream>

Inventory::Inventory(int slots, float slotSize, const sf::Vector2f& position)
    : totalSlots(slots), slotSize(slotSize), position(position), selectedSlot(0) {
    for (int i = 0; i < totalSlots; ++i) {
        sf::RectangleShape slot(sf::Vector2f(slotSize, slotSize));
        slot.setPosition({position.x + i * (slotSize + 5.0f), position.y});
        slot.setFillColor(sf::Color::Transparent);
        slot.setOutlineColor(sf::Color::White);
        slot.setOutlineThickness(2.0f);
        this->slots.push_back(slot);
    }
    // std::cout << "totalSlots is ready" << std::endl;
}

void Inventory::handleInput(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent) {
            int scancodeValue = static_cast<int>(keyEvent->scancode); // Приведение scancode к int
            int num1Value = static_cast<int>(sf::Keyboard::Scan::Num1);
            int num9Value = static_cast<int>(sf::Keyboard::Scan::Num9);
            // std::cout << "scancodeValue: " << scancodeValue << std::endl;
            if (scancodeValue >= num1Value && scancodeValue <= num9Value) {
                selectedSlot = scancodeValue - num1Value; // Вычисление индекса слота
                if (selectedSlot >= totalSlots) selectedSlot = totalSlots - 1;
            }
        }
    }
}

void Inventory::updatePosition(const sf::View& cameraView) {
    sf::Vector2f viewCenter = cameraView.getCenter();
    sf::Vector2f viewSize = cameraView.getSize();
    // Расчет новой позиции инвентаря
    float inventoryWidth = totalSlots * (slotSize + 5.0f) - 5.0f; // Общая ширина инвентаря
    float xPosition = viewCenter.x - inventoryWidth / 2.0f; // Центрирование по горизонтали
    float yPosition = viewCenter.y + viewSize.y / 2.0f - slotSize - 20.0f; // Снизу на 20 пикселей

    // Обновление позиции слотов
    for (int i = 0; i < totalSlots; ++i) {
        slots[i].setPosition({xPosition + i * (slotSize + 5.0f), yPosition});
    }
}

void Inventory::render(sf::RenderWindow& window) {
    for (size_t i = 0; i < slots.size(); ++i) {
        if (i == selectedSlot) {
            slots[i].setOutlineColor(sf::Color::Yellow);
        } else {
            slots[i].setOutlineColor(sf::Color::White);
        }
        window.draw(slots[i]);
    }
}

int Inventory::getSelectedSlot() const {
    return selectedSlot;
}