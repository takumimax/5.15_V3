#include "Player.h"
#include "Maze.h"
#include <iostream>
#include "Obstacle.h"
#include <memory>  // 確保有包含這個標頭


static const std::vector<std::string> playerTile = {
    " o ",
    "/|/",
    "/ \\"
};

void Player::setPosition(int startX, int startY) {
    x = startX;
    y = startY;
}

void Player::getPosition(int& outX, int& outY) const {
    outX = x;
    outY = y;
}

bool Player::move(char input, Maze& maze) {
    int newX = x, newY = y;
    switch (input) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        default: return false;
    }

    if (newX < 0 || newX >= maze.getWidth() || newY < 0 || newY >= maze.getHeight())
        return false;

    auto target = maze.getBlock(newX, newY);
    if (!target)
        return false;

    if (maze.isWall(newX, newY))
        return false;


    // 如果是障礙物，先攻擊並阻止移動
    auto obstacle = std::dynamic_pointer_cast<Obstacle>(target);
    if (obstacle) {
        obstacle->is_touched(*this);
        // 阻止移動，直到障礙物被破壞
        return false;
    }

    // 執行格子上的效果
    target->is_touched(*this);

    x = newX;
    y = newY;
    
    if(maze.isKey(newX, newY))
    {
        maze.replaceWithEmpty(newX,newY);
    }

    // 更新可見範圍
    maze.updateVisibility(*this);

    return true;
}



const std::vector<std::string>& Player::getTile() const 
{
    return playerTile;
}

bool Player::isDead() const {
    return health <= 0;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    // 你可以在這裡加入死亡時要做的額外處理，例如輸出訊息等
    // if (isDead()) { std::cout << "Player died!\n"; }
}

void Player::drawStatus(int mapHeightLines) const {
    // 游標移動到狀態行（地圖的行數 * 3行 + 1）
    std::cout << "\033[" << (mapHeightLines + 1) << ";1H";

    // 清除該行
    std::cout << "\033[2K";

    // 輸出狀態資訊
    std::cout << "Health: " << getHealth() << "  Atk: " << getAtk() << 
    "  Keys: " << getKeyCount() << "/" << getTotalKeys()<< std::flush;
}

int Player::getHealth() const { return health; }

void Player::setHealth(int h){ health=h;}

int Player::getAtk() const { return atk; }

void Player::reachGoal(){ reachedGoal = true; }
bool Player::hasReachedGoal() const { return reachedGoal; }

void Player::collide() 
{ atk--; }

void Player::collectKey() {
    if (keysCollected < totalKeys) keysCollected++;
}

int Player::getKeyCount() const {
    return keysCollected;
}

int Player::getTotalKeys() const {
    return totalKeys;
}

