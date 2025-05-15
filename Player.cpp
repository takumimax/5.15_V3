#include "Player.h"
#include "Maze.h"

void Player::setPosition(int startX, int startY) {
    x = startX;
    y = startY;
}

void Player::getPosition(int& outX, int& outY) const {
    outX = x;
    outY = y;
}

bool Player::move(char input, const Maze& maze) {
    int newX = x, newY = y;
    switch (input) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
    }

    if (newX < 0 || newX >= maze.getWidth() || newY < 0 || newY >= maze.getHeight())
        return false;

    if (maze.isWall(newX, newY))
        return false;

    x = newX;
    y = newY;

    return maze.isGoal(x, y);
}