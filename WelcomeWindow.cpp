#include "WelcomeWindow.h"
#include <iostream>

static void setText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f,
                   bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}

static std::string formatName(const std::string& input) {
    std::string result = input;
    if (!result.empty()) {
        result[0] = static_cast<char>(std::toupper(result[0]));
        for (std::size_t i = 1; i < result.length(); ++i) {
            result[i] = static_cast<char>(std::tolower(result[i]));
        }
    }
    return result;
}

std::string runWelcomeWindow(int width, int height) {
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    window.clear(sf::Color::Blue);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Failed to load font.ttf" << std::endl;
        return std::string();
    }

    sf::Text title("WELCOME TO MINESWEEPER!", font, 24);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(title, width / 2.0f, height / 2.0f - 150);

    sf::Text prompt("Enter your name:", font, 20);
    prompt.setFillColor(sf::Color::White);
    prompt.setStyle(sf::Text::Bold);
    setText(prompt, width / 2.0f, height / 2.0f - 75);

    std::string playerName;
    sf::Text nameText("", font, 18);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, width / 2.0f, height / 2.0f - 45);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return std::string();
            }
            if (event.type == sf::Event::TextEntered) {
                char ch = static_cast<char>(event.text.unicode);
                if (std::isalpha(ch) && playerName.length() < 10) {
                    playerName += ch;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace && !playerName.empty()) {
                    playerName.pop_back();
                }
                if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    window.close();
                    return formatName(playerName);
                }
            }
        }

        nameText.setString(formatName(playerName) + "|");
        setText(nameText, width / 2.0f, height / 2.0f - 45);

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(prompt);
        window.draw(nameText);
        window.display();
    }
    return std::string();
}
