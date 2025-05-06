#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "/home/kupalnic/CLionProjects/Farmer Life: Outside the town/src/UI/Inventory.hpp"

class Game {
public:
    Game();
    ~Game();

    void handleInput(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);

private:
    Player player;
    Camera camera;
    World world;
    Inventory inventory;
    sf::Font font;
};