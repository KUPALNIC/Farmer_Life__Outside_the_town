#include "Tool.hpp"
sf::Texture toolsTexture;

Tool::Tool(Type type, sf::Texture& icons, const sf::IntRect& rect)
    : toolType(type), durability(100), iconSprite(iconTexture) {
    iconSprite.setTexture(icons);
    iconSprite.setTextureRect(rect);
}

void Tool::render(sf::RenderWindow& window, sf::Vector2f pos, float scale) const {
    sf::Sprite sprite = iconSprite;
    sprite.setScale({scale, scale});
    sprite.setPosition(pos);
    window.draw(sprite);
}
