#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Player.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "/home/kupalnic/CLionProjects/Farmer Life: Outside the town/src/UI/Inventory.hpp"
#include <SFML/System/Clock.hpp>

class Game {
public:
    Game();
    ~Game();
    void handleInput(const sf::Event& event, const sf::RenderWindow& window);
    void update();
    void render(sf::RenderWindow& window);

private:
    Player player;
    Camera camera;
    World world;
    Inventory inventory;
    sf::Font font;
    bool isRemoving = false;           // флаг: сейчас держим ПКМ
    sf::Clock removeClock;             // часы для замера длительности удержания
    int removeGridX = -1, removeGridY = -1; // координаты ячейки, на которой началось удержание
};