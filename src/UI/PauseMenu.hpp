#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.hpp"

class PauseMenu {
public:
    PauseMenu(const sf::Font& font, sf::RenderWindow& window);

    void show();
    void hide();
    bool isActive() const;

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    void setCallbacks(std::function<void()> onResume, std::function<void()> onMainMenu, std::function<void()> onExit);

private:
    Menu menu;
    sf::Text title;
    bool active = false;
};