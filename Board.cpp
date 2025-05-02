#include <vector>
#include <ctime>
#include <cstdlib>
#include "Tile.cpp"

class Board {
private:
   int rows, cols, mineCount, unrevealedSafe;
   std::vector<std::vector<Tile>> tiles;
   std::vector<Tile*> mineTiles;

public:
   Board(int c, int r, int m) : cols(c), rows(r), mineCount(m) {
       tiles.resize(rows, std::vector<Tile>(cols, Tile(0, 0)));
       reset();
   }
   void reset() {
       tiles.clear();
       tiles.resize(rows, std::vector<Tile>(cols, Tile(0, 0)));
       mineTiles.clear();
       unrevealedSafe = rows * cols - mineCount;
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               tiles[y][x] = Tile(x, y);
           }
       }
       std::vector<pair<int, int>> positions;
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               positions.emplace_back(x, y);
           }
       }
       srand(time(0));
       for (int i = positions.size() - 1; i > 0; --i) {
           int j = rand() % (i + 1);
           swap(positions[i], positions[j]);
       }
       for (int i = 0; i < mineCount; ++i) {
           int x = positions[i].first;
           int y = positions[i].second;
           tiles[y][x].setMine(true);
           mineTiles.push_back(&tiles[y][x]);
       }
       updateAdjacents();
   }

   void draw(sf::RenderWindow& window, bool debugMode, bool isPaused, sf::Texture* tileRevealed, sf::Texture* mineTex, sf::Texture* flagTex) const {
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               tiles[y][x].draw(window, debugMode, isPaused, tileRevealed, mineTex, flagTex);
           }
       }
   }

   Tile* getTile(int x, int y) {
       if (x >= 0 && x < cols && y >= 0 && y < rows) {
           return &tiles[y][x];
       }
       return nullptr;
   }

  void updateAdjacents() {
       int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
       int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               std::vector<Tile*> neighbors;
               int count = 0;
               for (int i = 0; i < 8; ++i) {
                   int nx = x + dx[i], ny = y + dy[i];
                   if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                       Tile* neighbor = &tiles[ny][nx];
                       neighbors.push_back(neighbor);
                       if (neighbor->hasMine()) {
                           count++;
                       }
                   }
               }
               tiles[y][x].setNeighbors(neighbors);
               tiles[y][x].setAdjacentMines(count);
           }
       }
   }

   void revealTile(int x, int y) {
       Tile* tile = getTile(x, y);
       if (!tile || tile->isRevealedTile() || tile->isFlagged()) {
           return;
       }
       tile->reveal();
       if (!tile->hasMine()) {
           unrevealedSafe--;
       }
       if (tile->getAdjacentMineCount() == 0 && !tile->hasMine()) {
           revealAdjacentZeros(tile);
       }
   }

   void revealAdjacentZeros(Tile* tile) {
       for (Tile* neighbor : tile->getNeighbors()) {
           if (!neighbor->isRevealedTile() && !neighbor->isFlagged() && !neighbor->hasMine()) {
               neighbor->reveal();
               unrevealedSafe--;
               if (neighbor->getAdjacentMineCount() == 0) {
                   revealAdjacentZeros(neighbor);
               }
           }
       }
   }

   void toggleFlag(int x, int y) {
       Tile* tile = getTile(x, y);
       if (tile && !tile->isRevealedTile()) {
           tile->toggleFlag();
       }
   }

   bool checkVictory() const {
       return unrevealedSafe == 0;
   }
   void revealAllMines() {
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               if (tiles[y][x].hasMine()) {
                   tiles[y][x].reveal();
               }
           }
       }
   }
   void flagAllMines() {
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               if (tiles[y][x].hasMine()) {
                   tiles[y][x].forceFlag();
               }
           }
       }
   }
  int getFlagCount() const {
         int count = 0;
         for (int y = 0; y < rows; ++y) {
             for (int x = 0; x < cols; ++x) {
                 if (tiles[y][x].isFlagged()) {
                     count++;
                 }
             }
         }
         return count;
     }
};
