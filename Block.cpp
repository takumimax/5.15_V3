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
