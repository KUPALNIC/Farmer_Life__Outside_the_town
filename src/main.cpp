#include <SFML/Graphics.hpp>
#include "Game/Game.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Harvest Valley");
    window.setFramerateLimit(60);

    Game game;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else {
                game.handleInput(*event, window);
            }
        }

        game.update();
        window.clear();
        game.render(window);
        window.display();
    }

    return 0;
}