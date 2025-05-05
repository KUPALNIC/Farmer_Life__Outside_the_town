#include <SFML/Graphics.hpp>
#include "Game/Game.hpp"

int main() {
    // Создаем окно приложения
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Harvest Valley");
    window.setFramerateLimit(60);

    // Инициализация игры
    Game game;

    // Основной игровой цикл
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        // Обновляем состояние игры
        game.update();

        // Отрисовка
        window.clear();
        game.render(window);
        window.display();
    }

    return 0;
}