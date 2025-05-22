#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <memory>
#include "Player.h"

class Block {
protected:
    bool visible = false;
    bool discovered = false;

public:
    Block() = default;
    virtual ~Block() = default;

    // 根據狀態回傳圖樣（用於畫面顯示）
    virtual const std::vector<std::string>& getTile() const;
    // 真正圖樣（fog-of-war 顯示用）
    virtual const std::vector<std::string>& getRealTile() const = 0;

    // 被玩家觸碰時的反應（純虛函式）
    virtual void is_touched(Player& player) = 0;

    // 是否可見
    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }
    

    // 是否已探索過
    
    bool isDiscovered() const { return discovered; }
    void setDiscovered(bool d) { discovered = d; }

    
};

using BlockPtr = std::shared_ptr<Block>;

#endif
