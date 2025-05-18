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
    addCrop(CropType::Wheat, 1);
    addCrop(CropType::Carrot, 1);
    addCrop(CropType::Potato, 1);
    addCrop(CropType::Tomato, 1);
}
void Inventory::addCrop(CropType type, int amount) {
    if (type != CropType::None)
        cropItems[type] += amount;
}

bool Inventory::removeCrop(CropType type, int amount) {
    if (type == CropType::None) return false;
    if (cropItems[type] >= amount) {
        cropItems[type] -= amount;
        return true;
    }
    return false;
}

int Inventory::getCropCount(CropType type) const {
    auto it = cropItems.find(type);
    return it != cropItems.end() ? it->second : 0;
}

std::map<CropType, int> Inventory::getAllCrops() const {
    return cropItems;
}

void Inventory::renderCrops(sf::RenderWindow& window, sf::Texture& cropTexture) {
    int slotIndex = 3; // культуры начинаются с 4 слота (индекс 3)
    for (const auto& [type, count] : cropItems) {
        if (type == CropType::None || count <= 0) continue;
        if (slotIndex >= 7) break; // только 4 слота под культуры

        sf::Vector2f slotPos = slots[slotIndex].getPosition();
        int cropRow = static_cast<int>(type) - 1;
        sf::Sprite icon(cropTexture);
        icon.setTexture(cropTexture);
        icon.setTextureRect(sf::IntRect({0, cropRow * 32}, {32, 32}));
        icon.setScale({slotSize / 32.f, slotSize / 32.f});
        icon.setPosition(slotPos);
        window.draw(icon);

        // Количество
        static sf::Font font;
        static bool isFontLoaded = false;
        if (!isFontLoaded) {
            font.openFromFile("../assets/fonts/Delius/Delius-Regular.ttf");
            isFontLoaded = true;
        }
        sf::Text txt(font, std::to_string(count), 15);
        txt.setFillColor(sf::Color::White);
        txt.setOutlineColor(sf::Color::Black);
        txt.setOutlineThickness(2.f);
        txt.setPosition({slotPos.x + slotSize - 18, slotPos.y + 2});
        window.draw(txt);

        ++slotIndex;
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

void Inventory::render(sf::RenderWindow& window, const std::array<Tool*, 9>& hotbar, sf::Texture& cropTexture) {
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());
    renderCrops(window, cropTexture);
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

// CropType getCropInSlot(int slot) {
//     switch (slot) {
//         case 3: return CropType::Wheat;
//         case 4: return CropType::Carrot;
//         case 5: return CropType::Potato;
//         case 6: return CropType::Tomato;
//         default: return CropType::None;
//     }
// }