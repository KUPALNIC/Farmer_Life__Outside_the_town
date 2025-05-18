#include "TradeWindow.hpp"

static const int cropPricesBuy[]  = { 3, 4, 5, 6 };
static const int cropPricesSell[] = { 2, 3, 4, 5 };

TradeWindow::TradeWindow() {}

void TradeWindow::open() { openFlag = true; }
void TradeWindow::close() { openFlag = false; }
bool TradeWindow::isOpen() const { return openFlag; }

void TradeWindow::handleInput(const sf::Event& event, Inventory& inventory) {
    if (!openFlag) return;
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();
        if (key->code == sf::Keyboard::Key::Escape) { close(); }
        if (key->code == sf::Keyboard::Key::Up)    { if (selectedItem>0) --selectedItem; }
        if (key->code == sf::Keyboard::Key::Down)  { if (selectedItem<3) ++selectedItem; }
        if (key->code == sf::Keyboard::Key::Tab)   { mode = (mode == Buy) ? Sell : Buy; }
        if (key->code == sf::Keyboard::Key::Enter) {
            CropType type = (CropType)(selectedItem + 1);
            if (mode == Buy) {
                int price = cropPricesBuy[selectedItem];
                if (inventory.getCoins() >= price) {
                    inventory.spendCoins(price);
                    inventory.addCrop(type, 1);
                }else {
                    if (notifyCallback) notifyCallback("Not enough coins");
                }
            } else {
                int price = cropPricesSell[selectedItem];
                if (inventory.getCropCount(type) > 0) {
                    inventory.removeCrop(type, 1);
                    inventory.addCoins(price);
                }
            }
        }
    }
}

void TradeWindow::render(sf::RenderWindow& window, Inventory& inventory, sf::Texture& cropTexture, sf::Font& font) {
    if (!openFlag) return;
    sf::View original = window.getView();
    window.setView(window.getDefaultView());

    sf::RectangleShape bg({340, 220});
    bg.setFillColor(sf::Color(30, 30, 30, 230));
    bg.setOutlineColor(sf::Color::White);
    bg.setOutlineThickness(3.f);
    bg.setPosition({static_cast<float>(window.getSize().x/2-170), static_cast<float>(window.getSize().y/2-110)});
    window.draw(bg);

    static const char* cropNames[] = { "Wheat", "Carrot", "Potato", "Tomato" };
    for (int i = 0; i < 4; ++i) {
        float y = window.getSize().y/2-80 + i*42;
        sf::Text txt(font, std::string(cropNames[i]));
        txt.setPosition({static_cast<float>(window.getSize().x/2-120), y});
        txt.setCharacterSize(22);
        txt.setFillColor(i==selectedItem?sf::Color::Yellow:sf::Color::White);
        window.draw(txt);

        // Иконка культуры
        sf::Sprite icon(cropTexture);
        icon.setTextureRect(sf::IntRect({0, i*32}, {32,32}));
        icon.setScale({0.7f, 0.7f});
        icon.setPosition({static_cast<float>(window.getSize().x/2-150), y-6});
        window.draw(icon);

        // Цена
        int price = (mode==Buy)?cropPricesBuy[i]:cropPricesSell[i];
        sf::Text priceText(font);
        priceText.setFont(font);
        priceText.setString((mode==Buy?"Buy: ":"Sell: ") + std::to_string(price));
        priceText.setCharacterSize(20);
        priceText.setPosition({static_cast<float>(window.getSize().x/2+40), y});
        window.draw(priceText);

        // Количество у игрока
        sf::Text qtyText(font);
        qtyText.setFont(font);
        qtyText.setString("x"+std::to_string(inventory.getCropCount((CropType)(i+1))));
        qtyText.setCharacterSize(20);
        qtyText.setPosition({static_cast<float>(window.getSize().x/2+120), y});
        window.draw(qtyText);
    }

    sf::Text hint(font);
    hint.setFont(font);
    hint.setString("[Enter] Buy/Sell, [Tab] Mode, [Esc] Exit");
    hint.setCharacterSize(16);
    hint.setFillColor(sf::Color::Cyan);
    hint.setPosition({static_cast<float>(window.getSize().x/2-150), static_cast<float>(window.getSize().y/2+90)});
    window.draw(hint);

    window.setView(original);
}