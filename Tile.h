#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Tile {
private:
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    sf::Sprite baseSprite;
    sf::Sprite overlaySprite;
    std::vector<Tile*> neighbors;
    int adjacentMines = 0;
    int x, y;

public:
    Tile(int xPos = 0, int yPos = 0);
    void setMine(bool mine);
    void setTexture(sf::Texture& base, sf::Texture* overlay = nullptr);
    void draw(sf::RenderWindow& window, bool debugMode, bool isPaused,
              sf::Texture* tileRevealedTex, sf::Texture* mineTex, sf::Texture* flagTex) const;
    void reveal();
    void toggleFlag();
    void forceFlag();
    bool contains(int mouseX, int mouseY) const;
    void setNeighbors(const std::vector<Tile*>& adjacent);
    void setAdjacentMines(int count);
    bool hasMine() const;
    bool isRevealedTile() const;
    bool isFlaggedTile() const;
    int getAdjacentMineCount() const;
    std::vector<Tile*> getNeighbors() const;
};
