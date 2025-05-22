#include "Obstacle.h"
#include "Maze.h"
#include "Player.h"
#include "Empty.h"


const std::vector<std::string> Obstacle::tile = {
    "@@@",
    "@#@",
    "@@@"
};

// 私有建構子
Obstacle::Obstacle(std::shared_ptr<SharedData> shared) : data(shared) {}

// 靜態工廠函式
std::shared_ptr<Obstacle> Obstacle::create(Maze* maze, 
    const std::vector<std::pair<int,int>>& positions) 
{
    auto shared = std::make_shared<SharedData>();
    shared->hp = 2 ; // 初始血量
    shared->maze = maze;
    shared->positions = positions;  // 一次傳入所有位置

    // 改成 new 並由 shared_ptr 管理，避免 private 建構子衝突
    return std::shared_ptr<Obstacle>(new Obstacle(shared));
}

void Obstacle::is_touched(Player& player) {
    data->hp -= 1;
    player.collide();

    if (data->hp <= 0) {
        // 血量耗盡，全部換成空地
        for (auto& pos : data->positions) {
            data->maze->replaceBlockAt(pos.first, pos.second, std::make_shared<Empty>());
        }
    }
}

const std::vector<std::string>& Obstacle::getRealTile() const {
    return tile;
}
