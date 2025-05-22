#ifndef EMPTY_H
#define EMPTY_H

#include "Block.h"

class Empty : public Block {
public:
    Empty() = default;
    virtual ~Empty() = default;

    const std::vector<std::string>& getRealTile() const override;
    void is_touched(Player& player) override;
};

#endif
