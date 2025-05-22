#include "Goal.h"
#include "Player.h"
#include <iostream>

static const std::vector<std::string> goalTile = {
    "$ $",
    " $ ",
    "$ $"
};


const std::vector<std::string>& Goal::getRealTile() const {
    return goalTile;
}

void Goal::is_touched(Player& player) {
    player.reachGoal();
    std::cout << "You reached the goal!" << std::endl;
}
