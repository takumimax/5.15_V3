#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>
class Maze;

class Player {
private:
    int x = 0, y = 0;
    int health = 10;
    int atk = 10;

    bool reachedGoal = false;

public:
    void setPosition(int startX, int startY);
    void getPosition(int& outX, int& outY) const;
    bool move(char input, Maze& maze);
    const std::vector<std::string>& getTile() const ;

     // 新增：檢查玩家是否死亡
    bool isDead() const;

    // 範例：扣血函式，會自動判斷死亡狀態
    void takeDamage(int amount);

    int getHealth() const;
    void setHealth(int h);

    int getAtk() const;
    void collide() ;

    // 新增：在指定地圖下方畫狀態
    void drawStatus(int mapHeightLines) const;

    void reachGoal() ;
    bool hasReachedGoal() const;
};

#endif