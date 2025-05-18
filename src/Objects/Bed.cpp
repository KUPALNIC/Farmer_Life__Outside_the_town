#include "Bed.hpp"

// Bed::Bed() {}

bool Bed::plantCrop(CropType type) {
    if (occupied || !watered || type == CropType::None)
        return false;
    crop = type;
    growthStage = 1;
    growthTimer = 0.0f;
    occupied = true;
    watered = false; // после посадки сбрасываем полив
    return true;
}

void Bed::water() { watered = true; }

void Bed::update(float dt) {
    if (!occupied || crop == CropType::None || growthStage == 0 || growthStage >= maxGrowthStage) return;
    growthTimer += dt;
    if (growthTimer >= stageTime) {
        growthTimer -= stageTime;
        ++growthStage;
        if (growthStage > maxGrowthStage) growthStage = maxGrowthStage;
    }
}

CropType Bed::harvest() {
    if (isReadyToHarvest()) {
        CropType harvested = crop;
        remove();
        return harvested;
    }
    return CropType::None;
}

void Bed::remove() {
    watered = false;
    occupied = false;
    crop = CropType::None;
    growthStage = 0;
    growthTimer = 0.0f;
}

void Bed::render(sf::RenderWindow& window, sf::Texture& cropTexture, sf::Vector2f pos) const{
    if (occupied && crop != CropType::None && growthStage > 0) {
        int cropRow = static_cast<int>(crop) - 1; // CropType::Wheat==1
        if (cropRow >= 0) {
            sf::Sprite cropSprite(cropTexture);
            cropSprite.setTextureRect(sf::IntRect({
                growthStage * 32, cropRow * 32}, {32, 32}
            ));
            cropSprite.setPosition(pos);
            window.draw(cropSprite);
        }
    }
}