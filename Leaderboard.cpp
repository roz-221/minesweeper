#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

struct Entry {
   std::string name;
   int time;
   bool isCurrent;
   Entry() = default;
   Entry(const std::string& n, int t, bool c = false) : name(n), time(t), isCurrent(c) {}
};

void writeLeaderboard(const string& path, const vector<Entry>& leaders) {
   std::ofstream file(path, ios::trunc);
   int count = 0;
   for (const auto& entry : leaders) {
       file << entry.name << " " << entry.time << endl;
       if (++count == 5) break;
   }
}

std::vector<Entry> readLeaderboard(const string& path) {
   std::ifstream file(path);
   std::vector<Entry> leaders;
   string name;
   int score;
   while (file >> name >> score) {
       leaders.push_back(Entry(name, score));
   }
   return leaders;
}

string formatTime(int seconds) {
   int min = seconds / 60;
   int sec = seconds % 60;
   std::string minStr = (min < 10 ? "0" : "") + to_string(min);
   std::string secStr = (sec < 10 ? "0" : "") + to_string(sec);
   return minStr + ":" + secStr;
}

void showLeaderboardWindow(const std::vector<Entry>& leaders, int width, int height) {
   sf::RenderWindow window(sf::VideoMode(width, height), "Leaderboard", sf::Style::Close);
   sf::Font font;
   if (!font.loadFromFile("files/font.ttf")) {
       cerr << "Failed to load font.ttf" << endl;
       return;
   }
  
  std::string fullText;
   for (int i = 0; i < leaders.size() && i < 5; ++i) {
       std::string score = formatTime(leaders[i].time);
       std::string name = leaders[i].name + (leaders[i].isCurrent ? "*" : "");
       std::string entry = to_string(i + 1) + ".\t" + score + "\t" + name;
       fullText += entry;
       if (i < 4) fullText += "\n\n";
   }
  
   sf::Text title("LEADERBOARD", font, 20);
   title.setStyle(sf::Text::Bold | sf::Text::Underlined);
   title.setFillColor(sf::Color::White);
   sf::FloatRect titleBounds = title.getLocalBounds();
   title.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
   title.setPosition(width / 2.0f, height / 2.0f - 120);
   
   sf::Text entryText(fullText, font, 18);
   entryText.setStyle(sf::Text::Bold);
   entryText.setFillColor(sf::Color::White);
   sf::FloatRect entryBounds = entryText.getLocalBounds();
   entryText.setOrigin(entryBounds.left + entryBounds.width / 2, entryBounds.top + entryBounds.height / 2);
   entryText.setPosition(width / 2.0f, height / 2.0f + 20);

   while (window.isOpen()) {
       sf::Event event;
       while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed) {
               window.close();
           }
       }
       window.clear(sf::Color::Blue);
       window.draw(entryText);
       window.draw(title);
       window.display();
   }
}

void updateLeaderboard(const std::string& playerName, int timeSec, int width, int height) {
   std:string path = "files/leaderboard.txt";
   std::vector<Entry> leaders = readLeaderboard(path);
   leaders.push_back(Entry(playerName, timeSec));

   std::sort(leaders.begin(), leaders.end(), [](const Entry& a, const Entry& b) {
       return a.time < b.time;
   });

   for (auto& entry : leaders) {
     entry.isCurrent = false;
   }

   for (int i = 0; i < leaders.size() && i < 5; ++i) {
       if (leaders[i].name == playerName && leaders[i].time == timeSec) {
           leaders[i].isCurrent = true;
           break;
       }
   }
   writeLeaderboard(path, leaders);
   showLeaderboardWindow(leaders, width, height);
}

void viewLeaderboard(int width, int height) {
   string path = "files/leaderboard.txt";
   std::vector<Entry> leaders = readLeaderboard(path);
   showLeaderboardWindow(leaders, width, height);
}
