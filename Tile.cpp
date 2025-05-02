#include "Tile.h"

Tile::Tile(int xPos, int yPos)
    : x(xPos), y(yPos) {
    baseSprite.setPosition(x * 32.f, y * 32.f);
    overlaySprite.setPosition(x * 32.f, y * 32.f);
}

void Tile::setMine(bool mine) {
    isMine = mine;
}

void Tile::setTexture(sf::Texture& base, sf::Texture* overlay) {
    baseSprite.setTexture(base);
    if (overlay) {
        overlaySprite.setTexture(*overlay);
    }
}

void Tile::draw(sf::RenderWindow& window, bool debugMode, bool isPaused,
                sf::Texture* tileRevealedTex, sf::Texture* mineTex, sf::Texture* flagTex) const {
    if (isPaused && tileRevealedTex) {
        sf::Sprite flat(*tileRevealedTex);
        flat.setPosition(baseSprite.getPosition());
        window.draw(flat);
        return;
    }
    window.draw(baseSprite);
    if (isRevealed && isMine) {
        if (isFlagged && flagTex) {
            sf::Sprite flag(*flagTex);
            flag.setPosition(baseSprite.getPosition());
            window.draw(flag);
        }
        if (mineTex) {
            sf::Sprite mine(*mineTex);
            mine.setPosition(baseSprite.getPosition());
            window.draw(mine);
        }
    } else if (isRevealed && adjacentMines > 0) {
        window.draw(overlaySprite);
    } else if (isFlagged && !isRevealed && flagTex) {
        sf::Sprite flag(*flagTex);
        flag.setPosition(baseSprite.getPosition());
        window.draw(flag);
    } else if (debugMode && isMine && !isRevealed && mineTex) {
        sf::Sprite mine(*mineTex);
        mine.setPosition(baseSprite.getPosition());
        window.draw(mine);
    }
}

void Tile::reveal() {
    if (!isRevealed) {
        isRevealed = true;
    }
}

void Tile::toggleFlag() {
    if (!isRevealed) {
        isFlagged = !isFlagged;
    }
}

void Tile::forceFlag() {
    if (!isRevealed) {
        isFlagged = true;
    }
}

bool Tile::contains(int mouseX, int mouseY) const {
    return baseSprite.getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

void Tile::setNeighbors(const std::vector<Tile*>& adjacent) {
    neighbors = adjacent;
}

void Tile::setAdjacentMines(int count) {
    adjacentMines = count;
}

bool Tile::hasMine() const { return isMine; }

bool Tile::isRevealedTile() const { return isRevealed; }

bool Tile::isFlaggedTile() const { return isFlagged; }

int Tile::getAdjacentMineCount() const { return adjacentMines; }

std::vector<Tile*> Tile::get"));
