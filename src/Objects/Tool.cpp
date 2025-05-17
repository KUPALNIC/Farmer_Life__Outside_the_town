#include "Tool.hpp"

Tool::Tool(Type type, const sf::Texture& tex, sf::IntRect rect)
    : toolType(type), durability(100)
{
    iconSprite.setTexture(tex);
    iconSprite.setTextureRect(rect);
}

sf::Sprite Tool::getIconSprite() const {
    return iconSprite;
}