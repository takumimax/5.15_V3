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
    bool isVisible() const ;
    void setVisible(bool v) ;
    

    // 是否已探索過
    
    bool isDiscovered() const ;
    void setDiscovered(bool d) ;

    
};


#endif
