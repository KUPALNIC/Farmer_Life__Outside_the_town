#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.hpp"

class MainMenu {
public:
    MainMenu(const sf::Font& font, const sf::Texture& bgTexture, sf::RenderWindow& window);

    void show();
    void hide();
    bool isActive() const;

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    void setCallbacks(std::function<void()> onNewGame, std::function<void()> onExit);

private:
    Menu menu;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Text title;
    bool active = false;
};