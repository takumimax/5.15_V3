#ifndef PLAYER_H
#define PLAYER_H

class Maze;

class Player {
private:
    int x = 0, y = 0;

public:
    void setPosition(int startX, int startY);
    void getPosition(int& outX, int& outY) const;
    bool move(char input, const Maze& maze);
};

#endif