#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

std::string runWelcomeWindow(int width, int height);
void launchGame(int cols, int rows, int mines, const std::string& playerName);

int main() {
    std::ifstream config("files/config.cfg");
    int cols = 25;
    int rows = 16;
    int mines = 50;
    
    if (config.good()) {
        config >> cols >> rows >> mines;
    }

    int width = cols * 32;
    int height = rows * 32 + 100;

    std::string name = runWelcomeWindow(width, height);
    if (name.empty()) {
        return 0;
    }

    launchGame(cols, rows, mines, playerName);
    return 0;
}
