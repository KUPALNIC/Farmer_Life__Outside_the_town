#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Tool {
public:
    enum class Type { Hoe, WateringCan, Axe, None };

    Tool(Type type, sf::Texture& icons, const sf::IntRect& rect);

    void use(sf::Vector2i position, bool isLeftClick, bool isRightClick);
    bool isBroken() const;
    std::string getName() const;
    Type getType() const { return toolType; }
    const sf::Sprite& getIconSprite() const { return iconSprite; }
    void render(sf::RenderWindow& window, sf::Vector2f pos, float scale = 1.0f) const;

private:
    Type toolType;
    int durability;
    sf::Texture iconTexture;
    sf::Sprite iconSprite;
};