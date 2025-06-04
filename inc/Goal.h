#ifndef GOAL_H
#define GOAL_H

#include "Block.h"

class Goal : public Block {

public:
    Goal() = default;
    virtual ~Goal() = default;

    const std::vector<std::string>& getRealTile() const override;
    void is_touched(Player& player) override;
};

#endif
