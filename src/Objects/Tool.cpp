#include "Tool.hpp"

Tool::Tool(Type type) : toolType(type), durability(100) {}

void Tool::use() {
    if (durability > 0) {
        durability -= 10; // Уменьшение прочности при использовании
    }
}

bool Tool::isBroken() const {
    return durability <= 0;
}

std::string Tool::getName() const {
    switch (toolType) {
        case Type::Shovel: return "Shovel";
        case Type::Axe: return "Axe";
        case Type::Hoe: return "Hoe";
        default: return "Unknown Tool";
    }
}