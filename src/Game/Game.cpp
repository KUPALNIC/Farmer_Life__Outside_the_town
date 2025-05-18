#include "Game.hpp"
#include <iostream>

Game::Game(): player(world),
      inventory(9, 64.f, {650,1000}){
    if (!font.openFromFile("../assets/fonts/Delius/Delius-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
}

Game::~Game() {}

void Game::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    camera.handleInput(event);
    inventory.handleInput(event);

    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Right) {
            sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            removeGridX = static_cast<int>(worldPos.x) / world.getCellSize();
            removeGridY = static_cast<int>(worldPos.y) / world.getCellSize();
            isRemoving = true;
            removeClock.restart();
            world.removeBed(removeGridX, removeGridY);
        }

        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            int gridX = static_cast<int>(worldPos.x) / world.getCellSize();
            int gridY = static_cast<int>(worldPos.y) / world.getCellSize();

            // Проверяем — открыт ли биом и допустимы ли координаты
            if (!world.isBiomeOpened(gridX, gridY)) return;

            // Получаем выбранный слот и что в нем лежит
            int selectedSlot = inventory.getSelectedSlot();

            // 1–3 — инструменты
            if (selectedSlot >= 0 && selectedSlot <= 2) {
                Tool* tool = player.getSelectedTool();
                if (!tool) return;
                Tool::Type tt = tool->getType();

                // Мотыга — создаём грядку
                if (tt == Tool::Type::Hoe) {
                    if (world.getCellType(gridX, gridY) == CellType::EMPTY)
                        world.interactWithCell(gridX, gridY, CellType::BED);
                        world.createBed(gridX, gridY);
                    return;
                }
                // Лейка — поливаем грядку
                if (tt == Tool::Type::WateringCan) {
                    if (world.getCellType(gridX, gridY) == CellType::BED)
                        world.waterBed(gridX, gridY);

                    return;
                }
                // Топор — сбор урожая
                if (tt == Tool::Type::Axe) {
                    if (world.getCellType(gridX, gridY) == CellType::BED) {
                        Bed* bed = world.getBedAt(gridX, gridY);
                        if (bed && bed->isReadyToHarvest()) {
                            CropType harvested = bed->harvest();
                            if (harvested != CropType::None) {
                                inventory.addCrop(harvested, 2); // +2 культуры в инвентарь
                            }
                        }
                    }
                    return;
                }
            }

            if (selectedSlot >= 3 && selectedSlot <= 6) {
                CropType selectedSeed = inventory.getCropInSlot(selectedSlot);
                if (selectedSeed == CropType::None) return;
                // Можно сажать только если ячейка — грядка, она политая и без культуры
                if (world.getCellType(gridX, gridY) == CellType::BED) {
                    Bed* bed = world.getBedAt(gridX, gridY);
                    if (bed && !bed->isOccupied() && bed->isWatered()) {
                        if (inventory.getCropCount(selectedSeed) > 0 && bed->plantCrop(selectedSeed)) {
                            inventory.removeCrop(selectedSeed, 1);
                        }
                    }
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
    sf::Texture cropTexture;
    cropTexture.loadFromFile("/home/kupalnic/CLionProjects/Farmer Life: Outside the town/cmake-build-debug/assets/textures/crop.png");
    inventory.render(window, player.getHotbar(), cropTexture);
}