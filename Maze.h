#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <memory>  // shared_ptr
#include "Block.h"

class Player;

class Maze {
private:
    std::vector<std::string> mapData;                
    std::vector<std::vector<std::shared_ptr<Block>>> blocks; // 改成 shared_ptr
    int height = 0, width = 0;
    static int totalKeys;
    

public:
    ~Maze() = default;  // shared_ptr 自動清理

    bool loadMap(const std::string& filename, Player& player);
    void draw(const Player& player) const;

    bool isWall(int x, int y) const;
    bool isGoal(int x, int y) const;
    bool isKey(int x,int y) const;
    char getNumChar(int x, int y) const;

    int getHeight() const;
    int getWidth() const;

    // 改成回傳 shared_ptr
    std::shared_ptr<Block> getBlock(int x, int y) const;

    void updateVisibility(const Player& player);

    void getBlockPosition(const std::shared_ptr<Block>& block, int& outX, int& outY) const;

    void replaceWithEmpty(int x, int y);

    bool inBounds(int x, int y) const;

    // 改用 shared_ptr
    void replaceBlockAt(int x, int y, std::shared_ptr<Block> newBlock);

    void setMapCharAt(int x, int y, char ch);

    static void setTotalKeys(int);

    static int getTotalKeys() ;
};

#endif
