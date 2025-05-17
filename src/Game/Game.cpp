#include "Game.hpp"
#include <iostream>

Game::Game(): player(world),
      inventory(9, 64.f, {650,1000}) {
    if (!font.openFromFile("assets/fonts/Delius/Delius-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
}

Game::~Game() {}

void Game::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    // player.handleInput(event);
    camera.handleInput(event);
    inventory.handleInput(event);
    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        // if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Middle) {
        //     // получаем координаты клика внутри окна
        //     sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
        //     // используем то же окно, которое рендерим
        //     sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        //     int gridX = static_cast<int>(worldPos.x) / world.getCellSize();
        //     int gridY = static_cast<int>(worldPos.y) / world.getCellSize();
        //     if (world.getCellType(gridX, gridY) == CellType::EMPTY) {
        //         world.interactWithCell(gridX, gridY, CellType::TREE);
        //     }
        // }
        if (mouseEvent->button == sf::Mouse::Button::Right) {
            sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            removeGridX = static_cast<int>(worldPos.x) / world.getCellSize();
            removeGridY = static_cast<int>(worldPos.y) / world.getCellSize();
            isRemoving = true;
            removeClock.restart();
        }
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            int gridX = static_cast<int>(worldPos.x) / world.getCellSize();
            int gridY = static_cast<int>(worldPos.y) / world.getCellSize();
            Tool* tool = player.getSelectedTool();
            if (tool) {
                Tool::Type tt = tool->getType();
                if (tt == Tool::Type::Hoe) {
                    if (world.getCellType(gridX, gridY) == CellType::EMPTY)
                        world.interactWithCell(gridX, gridY, CellType::BED);
                }
                if (tt == Tool::Type::WateringCan) {
                    if (world.getCellType(gridX, gridY) == CellType::BED)
                        world.waterBed(gridX, gridY); // Новая функция, смотри ниже
                }
            }
        }
    }
}

void Game::update() {
    const float deltaTime = 1.0f / 60.0f;
    player.update(deltaTime);
    world.update(deltaTime);
    camera.update(deltaTime, player.getPosition());
    if (isRemoving && removeClock.getElapsedTime().asSeconds() >= 2.0f) {
        world.interactWithCell(removeGridX, removeGridY, CellType::EMPTY);
        isRemoving = false; // сброс
    }
    player.setSelectedSlot(inventory.getSelectedSlot());
}

void Game::render(sf::RenderWindow& window) {
    camera.apply(window);
    world.render(window);
    player.render(window);
    inventory.render(window, player.getHotbar());
}