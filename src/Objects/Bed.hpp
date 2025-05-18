#pragma once

#include <SFML/Graphics.hpp>

enum class CropType { None, Wheat, Carrot, Potato, Tomato };

class Bed {
public:
    Bed() = default;

    bool isWatered() const { return watered; }
    bool isOccupied() const { return occupied; }
    CropType getCropType() const { return crop; }
    int getGrowthStage() const { return growthStage; }
    bool isReadyToHarvest() const { return growthStage == maxGrowthStage && crop != CropType::None; }

    bool plantCrop(CropType type);
    void water();
    void update(float dt);
    CropType harvest();
    void remove();
    void render(sf::RenderWindow& window, sf::Texture& cropTexture, sf::Vector2f pos) const;
private:
    int x;
    int y;
    bool watered = false;
    bool occupied = false;
    CropType crop = CropType::None;
    int growthStage = 0;
    float growthTimer = 0.0f;
    static constexpr float stageTime = 5.0f;
    static constexpr int maxGrowthStage = 5;
};