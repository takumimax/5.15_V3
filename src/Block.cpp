#include "Block.h"

const std::vector<std::string>& Block::getTile() const {
    static const std::vector<std::string> unknown = {
        "   ",
        "   ",
        "   "
    };

    static const std::vector<std::string> fog = {
        " . ",
        " . ",
        " . "
    };

    if (visible) {
        return getRealTile(); // subclass 提供實際樣式
    } else if (discovered) {
        return getRealTile();
    } else {
        return unknown;
    }
}


bool Block::isVisible() const { return visible; }
void Block::setVisible(bool v) { visible = v; }


// 是否已探索過

bool Block::isDiscovered() const { return discovered; }
void Block::setDiscovered(bool d) { discovered = d; }