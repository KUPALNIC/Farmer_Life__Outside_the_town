#include "Menu.hpp"

Menu::Menu(const sf::Font& font) : font(font) {}

void Menu::setPosition(float x, float y) {
    startX = x;
    startY = y;
}

void Menu::addButton(const std::string& text, std::function<void()> callback) {
    Button btn(font);
    btn.shape.setSize({320, 52});
    btn.shape.setFillColor(sf::Color(70, 70, 70, 220));
    btn.shape.setOutlineColor(sf::Color::White);
    btn.shape.setOutlineThickness(2.f);

    btn.text.setFont(font);
    btn.text.setString(text);
    btn.text.setCharacterSize(28);
    btn.text.setFillColor(sf::Color::White);
    btn.callback = callback;

    buttons.push_back(btn);

    // Расставить кнопки по вертикали
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].shape.setPosition({startX, startY + i * 70});
        buttons[i].text.setPosition({startX+32, startY + i*70 + 8});
    }
}

void Menu::handleEvent(const sf::Event& event, const sf::RenderWindow&) {
    if (!active) return;
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();
        if (key->code == sf::Keyboard::Key::Up) {
            if (selected > 0) --selected;
        }
        if (key->code == sf::Keyboard::Key::Down) {
            if (selected < (int)buttons.size()-1) ++selected;
        }
        if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space) {
            if (selected >= 0 && selected < (int)buttons.size()) {
                buttons[selected].callback();
            }
        }
    }
    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(mouse->position.x, mouse->position.y);
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].shape.getGlobalBounds().contains(mousePos)) {
                    buttons[i].callback();
                }
            }
        }
    }
    if (event.is<sf::Event::MouseMoved>()) {
        auto mouse = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f mousePos(mouse->position.x, mouse->position.y);
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].shape.getGlobalBounds().contains(mousePos)) {
                selected = (int)i;
            }
        }
    }
}

void Menu::render(sf::RenderWindow& window) {
    if (!active) return;
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].shape.setFillColor(i == selected ? sf::Color(120,120,40,230) : sf::Color(70,70,70,220));
        window.draw(buttons[i].shape);
        window.draw(buttons[i].text);
    }
}

void Menu::setActive(bool a) { active = a; }
bool Menu::isActive() const { return active; }