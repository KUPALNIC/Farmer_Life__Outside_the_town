#include "Game.hpp"
#include <iostream>
#include <cmath>

constexpr float TRADER_OPEN_DIST = 90.f;
constexpr float TRADER_CLOSE_DIST = 180.f;
const std::string TRADER_HELLO = "Best deals in the village!";
const std::string TRADER_GOODBYE = "See you soon, farmer!";

Game::Game()
    : inventory(9, 64.f, {650,1000}), trader({80.f, 80.f})
{
    font.openFromFile("../assets/fonts/Delius/Delius-Regular.ttf");
    traderTexture.loadFromFile("../assets/textures/trader.png");
    coinTexture.loadFromFile("../assets/textures/coin.png");
    cropTexture.loadFromFile("../assets/textures/crop.png");
    tradeWindow.setNotifyCallback([this](const std::string& msg) { notifyUser(msg); });
    reset();
}

Game::~Game() {}

void Game::reset() {
    // std::cout << "reset: start" << std::endl;
    world.emplace();
    // std::cout << "reset: world ok" << std::endl;
    player.emplace(*world);
    // std::cout << "reset: player ok" << std::endl;
    camera.emplace();
    // std::cout << "reset: camera ok" << std::endl;
    inventory = Inventory(9, 64.f, {650,1000});
    trader = Trader({80.f, 80.f});
    tradeWindow = TradeWindow();
    tradeWindow.setNotifyCallback([this](const std::string& msg) { notifyUser(msg); });
    notificationText.clear();
    notificationClock.restart();
    traderPhrase.clear();
    traderPhraseDuration = 0.0f;
}

void Game::notifyUser(const std::string& text) {
    notificationText = text;
    notificationClock.restart();
}

void Game::showTraderPhrase(const std::string& phrase, float duration) {
    traderPhrase = phrase;
    traderPhraseClock.restart();
    traderPhraseDuration = duration;
}

void Game::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    if (!player || !camera) return;
    camera->handleInput(event);
    inventory.handleInput(event);

    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();
        if (key->code == sf::Keyboard::Key::E) {
            sf::Vector2f p = player->getPosition();
            sf::Vector2f t = trader.getPosition();
            float dist = std::hypot(p.x-t.x, p.y-t.y);
            if (dist < TRADER_OPEN_DIST && !tradeWindow.isOpen()) {
                tradeWindow.open();
                showTraderPhrase(TRADER_HELLO, 2.0f);
            }
        }
    }
    tradeWindow.handleInput(event, inventory);

    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Right) {
            sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            removeGridX = static_cast<int>(worldPos.x) / world->getCellSize();
            removeGridY = static_cast<int>(worldPos.y) / world->getCellSize();
            isRemoving = true;
            removeClock.restart();
            world->removeBed(removeGridX, removeGridY);

        }
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i pixelPos(mouseEvent->position.x, mouseEvent->position.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            int gridX = static_cast<int>(worldPos.x) / world->getCellSize();
            int gridY = static_cast<int>(worldPos.y) / world->getCellSize();
            if (!world->isBiomeOpened(gridX, gridY)) return;
            int selectedSlot = inventory.getSelectedSlot();
            if (selectedSlot >= 0 && selectedSlot <= 2) {
                Tool* tool = player->getSelectedTool();
                if (!tool) return;
                Tool::Type tt = tool->getType();
                //Hoe
                if (tt == Tool::Type::Hoe) {
                    if (world->getCellType(gridX, gridY) == CellType::EMPTY) {
                        const int BED_COST = 10;
                        if (inventory.getCoins() >= BED_COST) {
                            inventory.spendCoins(BED_COST);
                            world->interactWithCell(gridX, gridY, CellType::BED);
                            world->createBed(gridX, gridY);
                        } else {
                            notifyUser("Not enough coins");
                        }
                    }
                    return;
                }
                //Watering can
                if (tt == Tool::Type::WateringCan) {
                    if (world->getCellType(gridX, gridY) == CellType::BED)
                        world->waterBed(gridX, gridY);
                    return;
                }
                // Топор — сбор урожая
                if (tt == Tool::Type::Axe) {
                    if (world->getCellType(gridX, gridY) == CellType::BED) {
                        Bed* bed = world->getBedAt(gridX, gridY);
                        if (bed && bed->isReadyToHarvest()) {
                            CropType harvested = bed->harvest();
                            if (harvested != CropType::None) {
                                inventory.addCrop(harvested, 2); // +2 культуры в инвентарь
                            }
                        }
                    }
                    world->dryBed(gridX, gridY);
                    return;
                }
            }
            if (selectedSlot >= 3 && selectedSlot <= 6) {
                CropType selectedSeed = inventory.getCropInSlot(selectedSlot);
                if (selectedSeed == CropType::None) return;
                // Можно сажать только если ячейка — грядка, она политая и без культуры
                if (world->getCellType(gridX, gridY) == CellType::BED) {
                    Bed* bed = world->getBedAt(gridX, gridY);
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
    if (!player || !camera || !world) return;
    const float deltaTime = 1.0f / 60.0f;
    player->update(deltaTime);
    world->update(deltaTime);
    camera->update(deltaTime, player->getPosition());
    player->setSelectedSlot(inventory.getSelectedSlot());
    world->updateBeds(deltaTime);

    if (tradeWindow.isOpen()) {
        sf::Vector2f p = player->getPosition();
        sf::Vector2f t = trader.getPosition();
        float dist = std::hypot(p.x-t.x, p.y-t.y);
        if (dist > TRADER_CLOSE_DIST) {
            tradeWindow.close();
            showTraderPhrase(TRADER_GOODBYE, 2.0f);
        }
    }
}

void Game::render(sf::RenderWindow& window) {
    if (!player || !camera || !world) return;
    camera->apply(window);
    world->render(window);
    world->renderBeds(window, cropTexture);
    player->render(window);

    trader.render(window, traderTexture);

    sf::Vector2f t = trader.getPosition();
    bool showHello = false;
    sf::Vector2f p = player->getPosition();
    float dist = std::hypot(p.x-t.x, p.y-t.y);
    if (dist < TRADER_OPEN_DIST && !tradeWindow.isOpen()) {
        showHello = true;
    }
    bool showPhrase = !traderPhrase.empty() && traderPhraseClock.getElapsedTime().asSeconds() < traderPhraseDuration;
    std::string phraseToDraw;
    if (showPhrase) {
        phraseToDraw = traderPhrase;
    } else if (showHello) {
        phraseToDraw = TRADER_HELLO;
    }
    if (!phraseToDraw.empty()) {
        sf::Text helloText(font, phraseToDraw, 20);
        helloText.setFillColor(sf::Color::Yellow);
        helloText.setOutlineColor(sf::Color::Black);
        helloText.setOutlineThickness(2.f);
        helloText.setPosition({t.x + 32 - helloText.getLocalBounds().size.x / 2.f, t.y - 38});
        window.draw(helloText);
    }
    if (!showPhrase && !traderPhrase.empty()) {
        traderPhrase.clear();
    }

    inventory.renderCoins(window, coinTexture);
    inventory.render(window, player->getHotbar(), cropTexture);

    tradeWindow.render(window, inventory, cropTexture, font);

    if (!notificationText.empty() && notificationClock.getElapsedTime().asSeconds() < 2.0f) {
        sf::View original = window.getView();
        window.setView(window.getDefaultView());
        sf::Text notif(font, notificationText, 20);
        notif.setFillColor(sf::Color::Red);
        notif.setOutlineColor(sf::Color::Black);
        notif.setOutlineThickness(2.f);
        notif.setPosition({window.getSize().x - 260.f, 50.f});
        window.draw(notif);
        window.setView(original);
    } else if (notificationClock.getElapsedTime().asSeconds() >= 2.0f) {
        notificationText.clear();
    }
}
