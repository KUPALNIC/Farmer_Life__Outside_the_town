#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Camera.hpp"
#include "World.hpp"

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

    sf::Font font;
};