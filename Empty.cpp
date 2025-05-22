#include "Empty.h"

static const std::vector<std::string> emptyTile = {
    "   ",
    "   ",
    "   "
};

const std::vector<std::string>& Empty::getRealTile() const {
    return emptyTile;
}

void Empty::is_touched(Player&) {
    // 空地不做任何事
}
