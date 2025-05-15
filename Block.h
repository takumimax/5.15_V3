#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <unordered_map>

class Block {
private:
    std::unordered_map<char, std::vector<std::string>> tileMap;

public:
    Block();

    const std::vector<std::string>& getTile(char tileType) const;
    std::vector<std::string> getPlayerTile() const;
};

#endif