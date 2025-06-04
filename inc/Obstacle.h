#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Block.h"
#include <vector>
#include <string>
#include <utility>
#include <memory>

class Maze;
class Player;

class Obstacle : public Block {
private:
    struct SharedData {
        int hp;
        std::vector<std::pair<int, int>> positions;
        Maze* maze;
    };

    std::shared_ptr<SharedData> data;
    //std::shared_ptr 是可以讓多個 std::shared_ptr 共享一份記憶體，
    //並且在最後一個 std::shared_ptr 生命週期結束時時自動釋放記憶體

    // 私有建構子，外部無法直接 new
    Obstacle(std::shared_ptr<SharedData> shared);

    static const std::vector<std::string> tile;

public:
    // 工廠函式，建立一組障礙 (會自行建立 SharedData 並加入所有位置)
    static std::shared_ptr<Obstacle> create(Maze* maze, 
        const std::vector<std::pair<int, int>>& positions);

    // 接口
    void is_touched(Player& player) override;
    const std::vector<std::string>& getRealTile() const override;
};

#endif
