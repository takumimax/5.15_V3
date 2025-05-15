#include "Block.h"

Block::Block() {
    tileMap['1'] = {
        "***",
        "***",
        "***"
    };
    tileMap['0'] = {
        "   ",
        "   ",
        "   "
    };
    tileMap['2'] = {
        "&$&",
        "$&$",
        "#$#"
    };
}

const std::vector<std::string>& Block::getTile(char tileType) const {
    auto it = tileMap.find(tileType);
    if (it != tileMap.end()) {
        return it->second;
    }
    static const std::vector<std::string> unknown = {
        "???",
        "???",
        "???"
    };
    return unknown;
}

std::vector<std::string> Block::getPlayerTile() const {
    return {
        " O ",
        "/|/",
        "/ \\",
    };
}
