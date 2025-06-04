#ifndef WALL_H
#define WALL_H

#include "Block.h"

class Wall : public Block {
public:
    Wall() = default;
    virtual ~Wall() = default;

    const std::vector<std::string>& getRealTile() const override;
    void is_touched(Player& player) override;
};

#endif
