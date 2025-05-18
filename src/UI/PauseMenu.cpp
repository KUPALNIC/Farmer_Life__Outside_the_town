#include "PauseMenu.hpp"

PauseMenu::PauseMenu(const sf::Font& font, sf::RenderWindow& window)
    : menu(font), title(font) {
    title.setFont(font);
    title.setString("Paused");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color(30, 30, 60));
    title.setOutlineThickness(4.f);
    title.setPosition({window.getSize().x / 2.f - title.getLocalBounds().size.x, 100});
    menu.setPosition(window.getSize().x / 2.f - 160, 240);
}

void PauseMenu::setCallbacks(std::function<void()> onResume, std::function<void()> onMainMenu, std::function<void()> onExit) {
    menu.addButton("Resume", onResume);
    menu.addButton("Main Menu", onMainMenu);
    menu.addButton("Exit", onExit);
}

void PauseMenu::show() { active = true; menu.setActive(true);}
void PauseMenu::hide() { active = false; menu.setActive(false);}
bool PauseMenu::isActive() const { return active; }

void PauseMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!active) return;
    menu.handleEvent(event, window);
}

void PauseMenu::render(sf::RenderWindow& window) {
    if (!active) return;
    window.setView(window.getDefaultView());
    window.draw(title);
    menu.render(window);
}