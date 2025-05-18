#include "Game.hpp"
#include <iostream>

constexpr int BED_COST = 10;

Game::Game(): player(world),
      inventory(9, 64.f, {650,1000}), trader({80.f, 80.f}){
    if (!font.openFromFile("../assets/fonts/Delius/Delius-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    font.openFromFile("../assets/fonts/Delius/Delius-Regular.ttf");
    traderTexture.loadFromFile("../assets/textures/trader.png");
    coinTexture.loadFromFile("../assets/textures/coin.png");
    cropTexture.loadFromFile("../assets/textures/crop.png");
    tradeWindow.setNotifyCallback([this](const std::string& msg) { notifyUser(msg); });
}

Game::~Game() {}

void Game::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    camera.handleInput(event);
    inventory.handleInput(event);
    // Проверка открытия окна торговли
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();
        if (key->code == sf::Keyboard::Key::E) {
            // Проверка дистанции до торговца
            sf::Vector2f p = player.getPosition();
            sf::Vector2f t = trader.getPosition();
            float dist = std::hypot(p.x-t.x, p.y-t.y);
            if (dist < 90.f && !tradeWindow.isOpen()) {
                tradeWindow.open();
            }
        }
    }
    tradeWindow.handleInput(event, inventory);

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
                        if (inventory.getCoins() >= BED_COST) {
                            inventory.spendCoins(BED_COST);
                            world.interactWithCell(gridX, gridY, CellType::BED);
                            world.createBed(gridX, gridY);
                        } else {
                            notificationText = "Not enough coins";
                            notificationClock.restart();
                        }
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
                    world.dryBed(gridX, gridY);
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
    if (isRemoving && removeClock.getElapsedTime().asSeconds() >2.0f) {
        world.interactWithCell(removeGridX, removeGridY, CellType::EMPTY);
        isRemoving = false; // сброс
    }
    player.setSelectedSlot(inventory.getSelectedSlot());
    world.updateBeds(deltaTime);
}

sf::Texture cropTexture;

void Game::render(sf::RenderWindow& window) {
    camera.apply(window);
    world.render(window);
    world.renderBeds(window, cropTexture);
    trader.render(window, traderTexture);
    player.render(window);
    if (!cropTexture.loadFromFile("../assets/textures/crop.png")) {
        std::cerr << "Failed to load crop.png!" << std::endl;
    }
    cropTexture.loadFromFile("../assets/textures/crop.png");
    inventory.render(window, player.getHotbar(), cropTexture);


    // HUD: монеты и инвентарь всегда поверх
    inventory.renderCoins(window, coinTexture);
    if (!notificationText.empty() && notificationClock.getElapsedTime().asSeconds() < 2.0f) {
        sf::View original = window.getView();
        window.setView(window.getDefaultView());
        sf::Text notif(font, notificationText, 20);
        notif.setFillColor(sf::Color::Red);
        notif.setOutlineColor(sf::Color::Black);
        notif.setOutlineThickness(2.f);
        // notif.setScale({3.f, 3.f});
        notif.setPosition({window.getSize().x - 200.f, 50.f});
        window.draw(notif);
        window.setView(original);
    } else if (notificationClock.getElapsedTime().asSeconds() >= 2.0f) {
        notificationText.clear();
    }
    inventory.render(window, player.getHotbar(), cropTexture);

    // Окно торговли поверх всего
    tradeWindow.render(window, inventory, cropTexture, font);

}


void Game::notifyUser(const std::string& text) {
    notificationText = text;
    notificationClock.restart();
}