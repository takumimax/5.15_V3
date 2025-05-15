#include "Maze.h"
#include "Player.h"
#include <fstream>
#include <iostream>

bool Maze::loadMap(const std::string& filename, Player& player) {
    std::ifstream file(filename);
    if (!file) return false;

    mapData.clear();
    std::string line;
    while (std::getline(file, line)) {
        mapData.push_back(line);
    }

    height = mapData.size();
    width = mapData.empty() ? 0 : mapData[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mapData[y][x] == '0') {
                player.setPosition(x, y);
                return true;
            }
        }
    }
    return false;
}

void Maze::draw(const Player& player, const Block& block) const {
    std::cout << "\033[2J\033[H";
    int px, py;
    player.getPosition(px, py);

    for (int y = 0; y < height; ++y) {
        for (int line = 0; line < 3; ++line) {
            for (int x = 0; x < width; ++x) {
                if (x == px && y == py) {
                    std::cout << block.getPlayerTile()[line];
                } else {
                    std::cout << block.getTile(mapData[y][x])[line];
                }
            }
            std::cout << '\n';
        }
    }
}