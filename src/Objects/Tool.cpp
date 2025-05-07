#include "Tool.hpp"
#include "Crop.hpp" // Для взаимодействия с грядками

Tool::Tool(Type type) : toolType(type), durability(100) {}

void Tool::use(sf::Vector2i position, bool isLeftClick, bool isRightClick) {
    if (durability <= 0) return;

    // Логика для плуга
    if (toolType == Type::Hoe && isLeftClick) {
        // Создать грядку на позиции
        // TODO: добавить вызов функции создания грядки
    } else if (toolType == Type::Hoe && isRightClick) {
        // Удаление грядки при удержании
        // TODO: добавить логику удаления
    }

    // Логика для лейки
    if (toolType == Type::WateringCan) {
        // Полив (например, увеличение стадии роста Crop)
        // TODO: добавить логику полива
    }

    durability -= 10; // Уменьшение прочности
}

bool Tool::isBroken() const {
    return durability <= 0;
}

std::string Tool::getName() const {
    switch (toolType) {
        case Type::Hoe: return "Hoe";
        case Type::WateringCan: return "Watering Can";
        case Type::Axe: return "Axe";
        default: return "Unknown Tool";
    }
}