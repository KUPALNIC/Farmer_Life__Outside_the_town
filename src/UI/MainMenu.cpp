#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Font& font, const sf::Texture& bgTexture, sf::RenderWindow& window)
    : menu(font), bgSprite(bgTexture), title(font) {
    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(
        {
            float(window.getSize().x) / bgTexture.getSize().x,
            float(window.getSize().y) / bgTexture.getSize().y
        }
    );
    title.setFont(font);
    title.setString("Farmer Life: Outside the Town");
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color(80, 60, 30));
    title.setOutlineThickness(4.f);
    title.setPosition({window.getSize().x / 2.f - title.getLocalBounds().size.x / 2.f, 80});
    menu.setPosition(window.getSize().x / 2.f - 160, 250);
}

void MainMenu::setCallbacks(std::function<void()> onNewGame, std::function<void()> onExit) {
    menu.addButton("New Game", onNewGame);
    menu.addButton("Exit", onExit);
}

void MainMenu::show() { active = true; menu.setActive(true);}
void MainMenu::hide() { active = false; menu.setActive(false);}
bool MainMenu::isActive() const { return active; }

void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!active) return;
    menu.handleEvent(event, window);
}

void MainMenu::render(sf::RenderWindow& window) {
    if (!active) return;
    window.setView(window.getDefaultView());
    window.draw(bgSprite);
    window.draw(title);
    menu.render(window);
}