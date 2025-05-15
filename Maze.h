#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include "Block.h"

class Player; // Forward declaration

class Maze {
private:
    std::vector<std::string> mapData;
    int height = 0, width = 0;

public:
    bool loadMap(const std::string& filename, Player& player);
    void draw(const Player& player, const Block& block) const;
    bool isWall(int x, int y) const;
    bool isGoal(int x, int y) const;
    char getTile(int x, int y) const;
    int getHeight() const;
    int getWidth() const;
};

#endif