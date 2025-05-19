#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Player.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "/home/kupalnic/CLionProjects/Farmer Life: Outside the town/src/UI/Inventory.hpp"
#include <SFML/System/Clock.hpp>
#include "Trader.hpp"
#include "TradeWindow.hpp"
#include <optional>

class Game {
public:
    Game();
    ~Game();

    void reset();

    void handleInput(const sf::Event& event, const sf::RenderWindow& window);
    void update();
    void render(sf::RenderWindow& window);
    void notifyUser(const std::string& text);

    void showTraderPhrase(const std::string &phrase, float duration);

private:
    std::optional<World> world;
    std::optional<Player> player;
    std::optional<Camera> camera;
    Inventory inventory;
    sf::Font font;
    bool isRemoving = false;
    sf::Clock removeClock;
    int removeGridX = -1, removeGridY = -1;
    Trader trader;
    TradeWindow tradeWindow;
    sf::Texture traderTexture;
    sf::Texture coinTexture;
    sf::Texture cropTexture;
    std::string notificationText;
    sf::Clock notificationClock;
    std::string traderPhrase;
    sf::Clock traderPhraseClock;
    float traderPhraseDuration = 0.0f;

};