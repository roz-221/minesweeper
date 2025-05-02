#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
#include <chrono>
#include "Board.cpp"

void updateLeaderboard(const std::string& name, int time, int width, int height);
void viewLeaderboard(int width, int height);

class GameWindow {
private:
   int cols, rows, mines;
   bool isGameOver = false;
   bool isVictory = false;
   bool debugMode = false;
   bool isPaused = false;
   bool hasBegun = false;

   sf::RenderWindow window;
   Board board;
   std::map<std::string, sf::Texture> textures;
   sf::Texture digitsTexture;

   sf::Sprite smile;
   sf::Sprite pauseButton;
   sf::Sprite leaderboardButton;
   sf::Sprite debugButton;

   time_point<steady_clock> startTime;
   time_point<steady_clock> pauseStart;
   time_point<steady_clock> leaderboardStart;
   int pausedSeconds = 0;
   int finalTime = 0;
   bool isViewingLeaderboard = false;
   std::string playerName;

   void handleClick(sf::Event::MouseButtonEvent mouse) {
       int x = mouse.x;
       int y = mouse.y;

       if (smile.getGlobalBounds().contains(x, y)) {
           board.reset();
           isGameOver = false;
           isVictory = false;
           debugMode = false;
           isPaused = false;
           hasBegun = false;
           pausedSeconds = 0;
           finalTime = 0;
           smile.setTexture(textures["face_happy"]);
           pauseButton.setTexture(textures["pause"]);
           return;
       }

       if (pauseButton.getGlobalBounds().contains(x, y) && !isGameOver) {
           isPaused = !isPaused;
           if (isPaused) {
               pauseStart = steady_clock::now();
               pauseButton.setTexture(textures["play"]);
           } else {
               pausedSeconds += duration_cast<seconds>(steady_clock::now() - pauseStart).count();
               pauseButton.setTexture(textures["pause"]);
           }
           return;
       }

       if (debugButton.getGlobalBounds().contains(x, y) && !isGameOver && !isPaused) {
           debugMode = !debugMode;
           return;
       }

       if (leaderboardButton.getGlobalBounds().contains(x, y)) {
           int width = cols * 16;
           int height = rows * 16 + 50;

           int timeInSeconds = 0;
           if (hasBegun) {
               if (isPaused) {
                   timeInSeconds = pausedSeconds;
               } else {
                   timeInSeconds = duration_cast<seconds>(steady_clock::now() - startTime).count() - pausedSeconds;
               }
           }

           leaderboardStart = steady_clock::now();
           isViewingLeaderboard = true;

           updateTileTexture();
           window.clear(sf::Color::White);
           board.draw(window, debugMode, isPaused || isViewingLeaderboard,&textures["tile_revealed"], &textures["mine"], &textures["flag"]);
            window.draw(pauseButton);
           window.draw(smile);
           window.draw(debugButton);
           window.draw(leaderboardButton);
           drawMineCounter();
           drawTimer();
           window.display();
           viewLeaderboard(width, height);
           if (!isPaused) {
               pausedSeconds += duration_cast<seconds>(steady_clock::now() - leaderboardStart).count();
           }
           isViewingLeaderboard = false;
           return;
       }

       if (isPaused || isGameOver) {
         return;
       }
     
       int tilex = x / 32;
       int tiley = y / 32;
       Tile* tile = board.getTile(tilex, tiley);
       if (!tile) return;
     
       if (!hasBegun) {
           hasBegun = true;
           startTime = steady_clock::now();
       }

       if (mouse.button == sf::Mouse::Left) {
           board.revealTile(tilex, tiley);
           if (tile->hasMine() && tile->isRevealedTile()) {
               isGameOver = true;
               smile.setTexture(textures["face_lose"]);
               board.revealAllMines();
               finalTime = duration_cast<seconds>(steady_clock::now() - startTime).count() - pausedSeconds;
           } 
           else if (board.checkVictory()) {
               isGameOver = true;
               isVictory = true;
               smile.setTexture(textures["face_win"]);
               board.flagAllMines();
               finalTime = duration_cast<seconds>(steady_clock::now() - startTime).count() - pausedSeconds;

               updateTileTexture();
               window.clear(sf::Color::White);
               board.draw(window, debugMode, isPaused || isViewingLeaderboard,
                          &textures["tile_revealed"], &textures["mine"], &textures["flag"]);
               window.draw(smile);
               window.draw(debugButton);
               window.draw(pauseButton);
               window.draw(leaderboardButton);
               drawMineCounter();
               drawTimer();
               window.display();

               updateLeaderboard(playerName, finalTime, cols * 16, rows * 16 + 50);
           }
       } else if (mouse.button == sf::Mouse::Right) {
           board.toggleFlag(tilex, tiley);
       }
   }

void loadTextures() {
       std::string names[] = {
           "tile_hidden", "tile_revealed", "flag", "mine","number_1", "number_2", "number_3", "number_4",
           "number_5", "number_6", "number_7", "number_8","face_happy", "face_win", "face_lose",
           "pause", "play", "leaderboard", "debug" };
     
       for (std::string name : names) {
           sf::Texture texture;
           if (!texture.loadFromFile("images/" + name + ".png")) {
               cerr << "Failed to load " << name << ".png" << endl;
               exit(1);
           }
           textures[name] = texture;
       }

       if (!digitsTexture.loadFromFile("images/digits.png")) {
           cerr << "Failed to load digits.png" << endl;
           exit(1);
       }

       debugButton.setTexture(textures["debug"]);
       debugButton.setPosition((cols * 32) - 304, 32 * (rows + 0.5f));
     
       smile.setTexture(textures["face_happy"]);
       smile.setPosition((cols / 2.0f * 32) - 32, 32 * (rows + 0.5f));

      leaderboardButton.setTexture(textures["leaderboard"]);
       leaderboardButton.setPosition((cols * 32) - 176, 32 * (rows + 0.5f));
     
       pauseButton.setTexture(textures["pause"]);
       pauseButton.setPosition((cols * 32) - 240, 32 * (rows + 0.5f));
       
   }

   void updateTileTexture() {
       for (int y = 0; y < rows; ++y) {
           for (int x = 0; x < cols; ++x) {
               Tile* tile = board.getTile(x, y);
               if (tile->isRevealedTile()) {
                   tile->setTexture(textures["tile_revealed"]);
                   if (tile->hasMine()) {
                       tile->setTexture(textures["tile_revealed"], &textures["mine"]);
                   } 
                   else if (tile->getAdjacentMineCount() > 0) {
                       std::string num = "number_" + to_std::string(tile->getAdjacentMineCount());
                       tile->setTexture(textures["tile_revealed"], &textures[num]);
                   }
               } 
               else {
                   tile->setTexture(textures["tile_hidden"]);
                   if (tile->isFlaggedTile()) {
                       tile->setTexture(textures["tile_hidden"], &textures["flag"]);
                   } 
                   else if (debugMode && tile->hasMine()) {
                       tile->setTexture(textures["tile_hidden"], &textures["mine"]);
                   }
               }
           }
       }
   }

   void drawMineCounter() {
       int flagsLeft = mines - board.getFlagCount();
       int xStart = 33;
       int yStart = 32 * (rows + 0.5f) + 16;

       std::string digits = to_std::string(abs(flagsLeft));
       while (digits.length() < 3) digits = "0" + digits;

       int digitIndex = 0;
       if (flagsLeft < 0) {
           sf::Sprite dash(digitsTexture, sf::IntRect(10 * 21, 0, 21, 32));
           dash.setPosition(xStart, yStart);
           window.draw(dash);
           digitIndex++;
       }
       for (char c : digits) {
           int d = c - '0';
           sf::Sprite digit(digitsTexture, sf::IntRect(d * 21, 0, 21, 32));
           digit.setPosition(xStart + digitIndex * 21, yStart);
           window.draw(digit);
           digitIndex++;
       }
   }

   void handleKeyPress(sf::Keyboard::Key key) {
       if (key == sf::Keyboard::D && !isGameOver && !isPaused) {
           debugMode = !debugMode;
       }
    }

   void drawTimer() {
       int xMin = (cols * 32) - 97;
       int xSec = (cols * 32) - 54;
       int y = 32 * (rows + 0.5f) + 16;
       int totalSec = 0;
       if (isGameOver) {
           totalSec = finalTime;
       } else if (hasBegun) {
           if (isPaused) {
               totalSec = duration_cast<seconds>(pauseStart - startTime).count() - pausedSeconds;
           } else {
               totalSec = duration_cast<seconds>(steady_clock::now() - startTime).count() - pausedSeconds;
           }
       }
       totalSec = std::max(0, totalSec);

       int minutes = totalSec / 60;
       int seconds = totalSec % 60;
       std::string m = (minutes < 10 ? "0" : "") + to_std::string(minutes);
       std::string s = (seconds < 10 ? "0" : "") + to_std::string(seconds);
       sf::Sprite min1(digitsTexture, sf::IntRect((m[0] - '0') * 21, 0, 21, 32));
       sf::Sprite min2(digitsTexture, sf::IntRect((m[1] - '0') * 21, 0, 21, 32));
       sf::Sprite sec1(digitsTexture, sf::IntRect((s[0] - '0') * 21, 0, 21, 32));
       sf::Sprite sec2(digitsTexture, sf::IntRect((s[1] - '0') * 21, 0, 21, 32));

       min1.setPosition(xMin, y);
       min2.setPosition(xMin + 21, y);
       sec1.setPosition(xSec, y);
       sec2.setPosition(xSec + 21, y);

       window.draw(min1);
       window.draw(min2);
       window.draw(sec1);
       window.draw(sec2);
   }

public:
   GameWindow(int c, int r, int m, const std::string& name)
       : cols(c), rows(r), mines(m), board(c, r, m),
         window(sf::VideoMode(c * 32, r * 32 + 100), "Minesweeper", sf::Stileyle::Close),
         playerName(name) {
       loadTextures();
   }
   void run() {
       while (window.isOpen()) {
           sf::Event event;
           while (window.pollEvent(event)) {
               if (event.tileype == sf::Event::Closed) window.close();
               if (event.tileype == sf::Event::MouseButtonPressed)
                   handleClick(event.mouseButton);
               if (event.tileype == sf::Event::KeyPressed)
                   handleKeyPress(event.key.code);
           }
           updateTileTexture();

           window.clear(sf::Color::White);
           board.draw(window, debugMode, isPaused || isViewingLeaderboard,
                      &textures["tile_revealed"], &textures["mine"], &textures["flag"]);
           window.draw(smile);
           window.draw(debugButton);
           window.draw(pauseButton);
           window.draw(leaderboardButton);
           drawMineCounter();
           drawTimer();
           window.display();
       }
   }
};

void launchGame(int cols, int rows, int mines, const std::string& playerName) {
   GameWindow game(cols, rows, mines, playerName);
   game.run();
}
