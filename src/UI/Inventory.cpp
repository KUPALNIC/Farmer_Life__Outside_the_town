#include "Inventory.hpp"
#include "../Objects/Tool.hpp"

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

void Inventory::updatePosition(const sf::Vector2u& windowSize) {
    // Центрируем инвентарь по горизонтали и фиксируем его снизу
    float inventoryWidth = totalSlots * (slotSize + 5.0f) - 5.0f; // Общая ширина инвентаря
    float xPosition = (windowSize.x - inventoryWidth) / 2.0f; // Центрирование по горизонтали
    float yPosition = windowSize.y - slotSize - 20.0f;       // Отступ от нижнего края

    // Обновляем позиции слотов в соответствии с новыми координатами
    for (int i = 0; i < totalSlots; ++i) {
        slots[i].setPosition({xPosition + i * (slotSize + 5.0f), yPosition});
    }
}

void Inventory::render(sf::RenderWindow& window, const std::array<Tool*, 9>& hotbar) {
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    for (size_t i = 0; i < slots.size(); ++i) {
        if (i == selectedSlot) {
            slots[i].setOutlineColor(sf::Color::Yellow);
        } else {
            slots[i].setOutlineColor(sf::Color::White);
        }
        window.draw(slots[i]);

        if (hotbar[i]) {
            sf::Sprite icon = hotbar[i]->getIconSprite();
            sf::Vector2f rectPos = slots[i].getPosition();
            float slotCenterX = rectPos.x + slots[i].getSize().x / 2.0f;
            float slotCenterY = rectPos.y + slots[i].getSize().y / 2.0f;
            icon.setOrigin({8.f, 8.f}); // 16x16
            icon.setPosition({slotCenterX, slotCenterY});
            icon.setScale({3.f, 3.f}); //
            window.draw(icon);
        }
    }
    window.setView(originalView);
}

int Inventory::getSelectedSlot() const {
    return selectedSlot;
}