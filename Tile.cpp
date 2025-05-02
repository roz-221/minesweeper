#include <SFML/Graphics.hpp>
#include <vector>

class Tile {
private:
   bool isMine = false;
    bool isFlagged = false;
   bool isRevealed = false;
   sf::Sprite base;
   std::vector<Tile*> neighbors;
   sf::Sprite overlaySprite;
   int adjacentMines = 0;
   int x, y;

public:
   Tile (int x, int y) : x(x), y(y){
       base.setPosition(x * 32, y * 32);
       overlaySprite.setPosition(x * 32, y * 32);
   }

   void setMine(bool mine) { 
       isMine = mine; 
   }
   void setFlag() {
       if (!isRevealed) isFlagged = true;
   }

   void setTexture(sf::Texture& baseTexture, sf::Texture* overlay = nullptr) {
       base.setTexture(baseTexture);
       if (overlay) {
           overlaySprite.setTexture(*overlay);
       }
   }

   void draw(sf::RenderWindow& window, bool debug, bool isPaused, sf::Texture* tileRevealedTex, sf::Texture* mineTex, sf::Texture* flagTex) const {
       if (isPaused && tileRevealedTex != nullptr) {
           sf::Sprite flat(*tileRevealedTex);
           flat.setPosition(base.getPosition());
           window.draw(flat);
           return;
       }
    
       window.draw(base);

       if (isRevealed && isMine) {
           if (isFlagged && flagTex) {
               sf::Sprite flag(*flagTex);
               flag.setPosition(base.getPosition());
               window.draw(flag);
           }
           if (mineTex) {
               sf::Sprite mine(*mineTex);
               mine.setPosition(base.getPosition());
               window.draw(mine);
           }
       }
       else if (isRevealed && adjacentMines > 0) {
           window.draw(overlaySprite);
       }
       else if (debug && isMine && !isRevealed && mineTex) {
           sf::Sprite mine(*mineTex);
           mine.setPosition(base.getPosition());
           window.draw(mine);
       }
       else if (isFlagged && !isRevealed && flagTex) {
           sf::Sprite flag(*flagTex);
           flag.setPosition(base.getPosition());
           window.draw(flag);
       }
   }
   void reveal() {
       if (!isRevealed) {
           isRevealed = true;
       }
   }

   void setNeighbors(const std::vector<Tile*>& adjacent) { neighbors = adjacent; }

   void toggleFlag() { if (!isRevealed) isFlagged = !isFlagged; }

   bool contains(int mouseX, int mouseY) const {
       return base.getGlobalBounds().contains(mouseX, mouseY);
   }

   void setAdjacentMines(int count) { adjacentMines = count; }

   bool hasMine() const { return isMine; }
   bool isRevealedTile() const { return isRevealed; }
   bool isFlagged() const { return isFlagged; }
   int getAdjacentMineCount() const { return adjacentMines; }
   std::vector<Tile*> getNeighbors() const { return neighbors; }
};
