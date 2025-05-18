#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

class Menu {
public:
    Menu(const sf::Font& font);

    void setPosition(float x, float y);
    void addButton(const std::string& text, std::function<void()> callback);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    void setActive(bool active);
    bool isActive() const;



private:
    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        std::function<void()> callback;
        Button(const sf::Font& font) : text(font, "", 28) {}
    };

    std::vector<Button> buttons;
    int selected = 0;
    float startX = 0, startY = 0;
    bool active = false;
    const sf::Font& font;
};