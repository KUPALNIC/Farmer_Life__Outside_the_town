#include "Game.hpp"
#include <iostream>

Game::Game(): inventory(9, 32.f, {400,300}) {
    if (!font.openFromFile("assets/fonts/default.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
}

Game::~Game() {}

void Game::handleInput(const sf::Event& event) {
    player.handleInput(event);
    camera.handleInput(event);
    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
            const sf::Vector2i mousePosition = sf::Mouse::getPosition();
            sf::RenderWindow m_window;
            const sf::Vector2f worldPosition = m_window.mapPixelToCoords(mousePosition);
            int gridX = static_cast<int>(worldPosition.x) / world.getCellSize();
            int gridY = static_cast<int>(worldPosition.y) / world.getCellSize();
            world.interactWithCell(gridX, gridY, CellType::BED); // Пример: установка грядки
        }
    }
}

void Game::update() {
    const float deltaTime = 1.0f / 60.0f;
    player.update(deltaTime);
    world.update(deltaTime);
    camera.update(deltaTime, player.getPosition());
}

void Game::render(sf::RenderWindow& window) {
    camera.apply(window);
    world.render(window);
    player.render(window);
}