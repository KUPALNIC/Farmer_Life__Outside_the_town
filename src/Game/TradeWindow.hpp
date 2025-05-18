#pragma once
#include <SFML/Graphics.hpp>
#include "../UI/Inventory.hpp"
#include <functional>

class TradeWindow {
public:
    TradeWindow();
    void open();
    void close();
    bool isOpen() const;

    void handleInput(const sf::Event& event, Inventory& inventory);
    void render(sf::RenderWindow& window, Inventory& inventory, sf::Texture& cropTexture, sf::Font& font);
    void setNotifyCallback(std::function<void(const std::string&)> cb) { notifyCallback = cb; }

private:
    bool openFlag = false;
    int selectedItem = 0;
    enum Mode { Buy, Sell } mode = Buy;
    std::function<void(const std::string&)> notifyCallback;
};