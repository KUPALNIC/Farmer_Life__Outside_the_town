#include "Game.hpp"
#include <iostream>

Game::Game() {
    // Загрузка шрифта для UI
    if (!font.openFromFile("assets/fonts/default.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
}

Game::~Game() {
    // Очистка ресурсов, если необходимо
}

void Game::handleInput(const sf::Event& event) {
    player.handleInput(event);
    camera.handleInput(event);
}

void Game::update() {
    const float deltaTime = 1.0f / 60.0f; // Предположим, фиксированный FPS
    player.update(deltaTime);
    world.update(deltaTime);
    camera.update(deltaTime, player.getPosition());
}

void Game::render(sf::RenderWindow& window) {
    camera.apply(window);
    world.render(window);
    player.render(window);
}