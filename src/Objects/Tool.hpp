#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Tool {
public:
    enum class Type { Hoe, WateringCan, Axe };

    Tool(Type type, const sf::Texture& tex, sf::IntRect rect);

    void use(sf::Vector2i position, bool isLeftClick, bool isRightClick);
    bool isBroken() const;
    std::string getName() const;

    Type getType() const { return toolType; }
    sf::Sprite getIconSprite() const; // Новый метод

private:
    Type toolType;
    int durability;
    sf::Sprite iconSprite;
};