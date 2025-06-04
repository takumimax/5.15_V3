#ifndef KEY_H
#define KEY_H

#include "Block.h"
#include <vector>
#include <string>

class Key : public Block {
private:
    static const std::vector<std::string> tile;

public:
    Key() = default;
    virtual ~Key() = default;

    const std::vector<std::string>& getRealTile() const override;
    void is_touched(Player& player) override;
};

#endif
