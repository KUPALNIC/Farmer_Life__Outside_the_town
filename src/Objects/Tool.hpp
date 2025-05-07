#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Tool {
public:
    enum class Type { Hoe, WateringCan, Axe }; // Убрана лопата

    Tool(Type type);

    void use(sf::Vector2i position, bool isLeftClick, bool isRightClick);
    bool isBroken() const;

    std::string getName() const;

private:
    Type toolType;
    int durability; // Прочность инструмента
};