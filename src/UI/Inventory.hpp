#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Objects/Tool.hpp" // подключаем Tool
#include "../Objects/Bed.hpp"

class Inventory {
public:
    Inventory(int slots, float slotSize, const sf::Vector2f& position);

    void handleInput(const sf::Event& event);
    void render(sf::RenderWindow& window, const std::array<Tool*, 9>& hotbar, sf::Texture& cropTexture);
    void updatePosition(const sf::Vector2u& windowSize);
    int getSelectedSlot() const;
    CropType getCropInSlot(int slot) const;
    // Работа с культурами
    void addCrop(CropType type, int amount = 1);
    bool removeCrop(CropType type, int amount = 1);
    int getCropCount(CropType type) const;
    std::map<CropType, int> getAllCrops() const;


private:
    int selectedSlot;
    int totalSlots;
    float slotSize;
    sf::Vector2f position;
    std::vector<sf::RectangleShape> slots;

    // Культуры
    std::map<CropType, int> cropItems;
    std::vector<CropType> cropOrder; // порядок появления культур

    // Приватный метод для рендера культур
    void renderCrops(sf::RenderWindow& window, sf::Texture& cropTexture);
};