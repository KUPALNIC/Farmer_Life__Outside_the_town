#include <SFML/Graphics.hpp>
#include "Game/Game.hpp"
#include "UI/MainMenu.hpp"
#include "UI/PauseMenu.hpp"

enum class GameState {
    MainMenu, Playing, Paused, Exit
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Farmer life: Outside the town");
    window.setFramerateLimit(60);
    sf::Font font;
    font.openFromFile("../assets/fonts/Cherry_Bomb_One/CherryBombOne-Regular.ttf");

    sf::Texture mainMenuBg;
    mainMenuBg.loadFromFile("../assets/textures/sky.png");

    GameState state = GameState::MainMenu;
    Game game;
    MainMenu mainMenu(font, mainMenuBg, window);
    PauseMenu pauseMenu(font, window);

    mainMenu.setCallbacks(
        // onNewGame
        [&]() {
            game.reset();
            state = GameState::Playing;
            mainMenu.hide();
        },
        // onExit
        [&]() {
            state = GameState::Exit;
        }
    );
    pauseMenu.setCallbacks(
        // onResume
        [&]() {
            state = GameState::Playing;
            pauseMenu.hide();
        },
        // onMainMenu
        [&]() {
            state = GameState::MainMenu;
            pauseMenu.hide();
            mainMenu.show();
        },
        // onExit
        [&]() {
            state = GameState::Exit;
        }
    );

    mainMenu.show();

    while (window.isOpen() && state != GameState::Exit) {
        while (auto eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
            } else {
                game.handleInput(event, window);
                if (state == GameState::MainMenu) {
                    mainMenu.handleEvent(event, window);
                } else if (state == GameState::Playing) {
                auto keyEvent = event.getIf<sf::Event::KeyPressed>();
                    if (keyEvent) {
                        int scancodeValue = static_cast<int>(keyEvent->scancode);
                        int escapeValue = static_cast<int>(sf::Keyboard::Scancode::Escape);
                        if (keyEvent &&  scancodeValue == escapeValue) {
                            state = GameState::Paused;
                            pauseMenu.show();
                        }
                    }
                    else {
                        game.handleInput(event, window);
                    }
                } else if (state == GameState::Paused) {
                    pauseMenu.handleEvent(event, window);
                }
            }

        }

        // game.update();
        window.clear();
        if (state == GameState::MainMenu) {
            mainMenu.render(window);
        } else if (state == GameState::Playing) {
            game.update();
            game.render(window);
        } else if (state == GameState::Paused) {
            game.render(window);
            pauseMenu.render(window);
        }
        window.display();
    }

    return 0;
}