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
    mainMenuBg.loadFromFile("../assets/menu/main_bg.jpg"); // ваша картинка

    GameState state = GameState::MainMenu;
    Game game;
    MainMenu mainMenu(font, mainMenuBg, window);
    PauseMenu pauseMenu(font, window);

    mainMenu.setCallbacks(
        // onNewGame
        [&]() {
            game = Game();
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

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else {
                game.handleInput(*event, window);
            }
            if (state == GameState::MainMenu) {
                mainMenu.handleEvent(event, window);
            } else if (state == GameState::Playing) {
                // Открыть паузу по Esc
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                int scancodeValue = static_cast<int>(keyEvent->scancode);
                int escapeValue = static_cast<int>(sf::Keyboard::Scancode::Escape);
                if (event->is<sf::Event::KeyPressed> &&  scancodeValue == escapeValue) {
                    state = GameState::Paused;
                    pauseMenu.show();
                } else {
                    game.handleInput(event, window);
                }
            } else if (state == GameState::Paused) {
                pauseMenu.handleEvent(event, window);
            }
        }

        game.update();
        window.clear();
        game.render(window);
        window.display();
    }

    return 0;
}