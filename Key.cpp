#include "Key.h"
#include "Player.h"
#include "Empty.h"

const std::vector<std::string> Key::tile = {
    " O ",
    " |=",
    " |="
};

const std::vector<std::string>& Key::getRealTile() const {
    return tile;
}

void Key::is_touched(Player& player) {
    player.collectKey();  // 玩家增加鑰匙數量
}
