#include "Inventory.hpp"

#include <iostream>

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
    sf::Texture toolsTexture;
    toolsTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/assets/textures/tools.png");
    hotbar[0] = new Tool(ToolType::AXE, toolsTexture, sf::IntRect({0,0},{16,16}));
    hotbar[1] = new Tool(ToolType::PLOW, toolsTexture, sf::IntRect({16,0},{16,16}));
    hotbar[2] = new Tool(ToolType::WATERING_CAN, toolsTexture, sf::IntRect({32,0},{16,16}));
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

// Tool::Type getCurrentTool() {
//     return
// }

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
        // Нарисовать иконку инструмента если есть
        if (hotbar[i]) {
            // Загрузка текстуры должна быть централизована, тут предполагаем что у Tool есть спрайт
            sf::Sprite toolSprite;
            // Например, можно добавить в Tool метод getIconSprite()
            if (hotbar[i]->getType() == Tool::Type::Axe ||
                hotbar[i]->getType() == Tool::Type::Hoe ||
                hotbar[i]->getType() == Tool::Type::WateringCan) {
                toolSprite = hotbar[i]->getIconSprite();
                // Центрируем по прямоугольнику
                sf::Vector2f rectPos = slots[i].getPosition();
                float slotCenterX = rectPos.x + slots[i].getSize().x / 2.0f;
                float slotCenterY = rectPos.y + slots[i].getSize().y / 2.0f;
                toolSprite.setPosition({slotCenterX - 8, slotCenterY - 8});
                toolSprite.setScale({2.f, 2.f});
                window.draw(toolSprite);
                }
        }
    }
    window.setView(originalView);
}

int Inventory::getSelectedSlot() const {
    return selectedSlot;
}