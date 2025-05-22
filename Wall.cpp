#include "Wall.h"

static const std::vector<std::string> wallTile = {
    "***",
    "***",
    "***"
};


const std::vector<std::string>& Wall::getRealTile() const {
    return wallTile;
}

void Wall::is_touched(Player&) {
    // 不可穿越，實際邏輯已在 Maze 判斷
}
