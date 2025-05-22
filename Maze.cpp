#include "Maze.h"
#include "Player.h"
#include "Wall.h"
#include "Goal.h"
#include "Empty.h"
#include "Obstacle.h"
#include "Key.h"
#include <queue>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

int Maze::totalKeys = 3; 

bool Maze::loadMap(const std::string& filename, Player& player) {
    std::ifstream file(filename);
    if (!file) return false;

    mapData.clear();
    blocks.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty() && (mapData.empty() || line.size() == mapData[0].size())) {
            mapData.push_back(line);
        } else {
            std::cerr << "Invalid line length in map file.\n";
            return false;
        }
    }

    if (mapData.empty()) return false;

    height = static_cast<int>(mapData.size());
    width = static_cast<int>(mapData[0].size());
    blocks.resize(height, std::vector<std::shared_ptr<Block>>(width, nullptr));

    bool playerSet = false;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char ch = mapData[y][x];
            if (ch == '3' && !visited[y][x]) {
                std::vector<std::pair<int, int>> groupPositions;
                std::queue<std::pair<int, int>> q;
                q.push(std::make_pair(x,y));
                visited[y][x] = true;

                while (!q.empty()) {
                    auto p = q.front();
                    q.pop();
                    int cx = p.first;
                    int cy = p.second;

                    groupPositions.emplace_back(cx, cy);

                    const int dx[] = {1, -1, 0, 0};
                    const int dy[] = {0, 0, 1, -1};
                    for (int dir = 0; dir < 4; ++dir) {
                        int nx = cx + dx[dir], ny = cy + dy[dir];
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (!visited[ny][nx] && mapData[ny][nx] == '3') {
                                visited[ny][nx] = true;
                                q.push({nx, ny});
                            }
                        }
                    }
                }

                // 產生 shared_ptr Obstacle
                auto sharedObstacle = Obstacle::create(this, groupPositions);

                for (auto& pos : groupPositions) {
                    int gx = pos.first, gy = pos.second;
                    blocks[gy][gx] = sharedObstacle; // 所有位置指向同一 shared_ptr
                }

            } else if (ch == '0') {
                blocks[y][x] = std::make_shared<Empty>();
                if (!playerSet) {
                    player.setPosition(x, y);
                    playerSet = true;
                }
            } else if (ch == '1') {
                blocks[y][x] = std::make_shared<Wall>();
            } else if (ch == '2') {
                blocks[y][x] = std::make_shared<Goal>();
            } else if (ch != '3') {
                std::cerr << "Unknown character in map: '" << ch << "' at (" << x << ", " << y << ")\n";
                blocks[y][x] = std::make_shared<Empty>();
            }
        }
    }

    if (playerSet) {
        updateVisibility(player);
    }

    // ---- 隨機放置 3 把 Key ----
    std::vector<std::pair<int, int>> emptyPositions;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (std::dynamic_pointer_cast<Empty>(getBlock(x, y))) {
                emptyPositions.emplace_back(x, y);
            }
        }
    }

    // 確保有足夠空位
    if (emptyPositions.size() < 3) {
        std::cerr << "Error: Not enough empty space to place keys!" << std::endl;
        return false;
    }

    // 隨機打亂並選前3個位置
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(emptyPositions.begin(), emptyPositions.end(), gen);

    for (int i = 0; i < 3; ++i) {
        int keyX = emptyPositions[i].first;
        int keyY = emptyPositions[i].second;
        replaceBlockAt(keyX, keyY, std::make_shared<Key>());
        mapData[keyY][keyX]='k';
    }


    //return
    return playerSet;
}

void Maze::draw(const Player& player) const {
    std::cout << "\033[2J\033[H"; // 清螢幕，游標移左上

    int px, py;
    player.getPosition(px, py);

    for (int y = 0; y < height; ++y) {
        for (int line = 0; line < 3; ++line) {
            for (int x = 0; x < width; ++x) {
                if (x == px && y == py) {
                    std::cout << player.getTile()[line];
                } else {
                    if (blocks[y][x]) {
                        std::cout << blocks[y][x]->getTile()[line];
                    } else {
                        std::cout << "   ";
                    }
                }
            }
            std::cout << '\n';
        }
    }
}

void Maze::updateVisibility(const Player& player) {
    int px, py;
    player.getPosition(px, py);

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = px + dx;
            int ny = py + dy;
            if (inBounds(nx, ny) && blocks[ny][nx]) {
                blocks[ny][nx]->setVisible(true);
                blocks[ny][nx]->setDiscovered(true);
            }
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!(x >= px - 1 && x <= px + 1 && y >= py - 1 && y <= py + 1)) {
                if (blocks[y][x]) blocks[y][x]->setVisible(false);
            }
        }
    }
}

void Maze::getBlockPosition(const std::shared_ptr<Block>& block, int& outX, int& outY) const {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (blocks[y][x] == block) {
                outX = x;
                outY = y;
                return;
            }
    outX = -1;
    outY = -1;
}

void Maze::replaceBlockAt(int x, int y, std::shared_ptr<Block> newBlock) {
    if (inBounds(x, y)) {
        blocks[y][x] = newBlock;  // shared_ptr自動釋放舊物件
    }
}

void Maze::setMapCharAt(int x, int y, char ch) {
    if (inBounds(x, y)) {
        mapData[y][x] = ch;
    }
}

void Maze::replaceWithEmpty(int x, int y) {
    if (inBounds(x, y)) {
        blocks[y][x] = std::make_shared<Empty>();
    }
    mapData[y][x]='0';
}

bool Maze::isWall(int x, int y) const {
    return inBounds(x, y) && mapData[y][x] == '1';
}

bool Maze::isGoal(int x, int y) const {
    return inBounds(x, y) && mapData[y][x] == '2';
}

bool Maze::isKey(int x,int y) const {
    return  inBounds(x, y) && mapData[y][x] =='k';
}

char Maze::getNumChar(int x, int y) const {
    return inBounds(x, y) ? mapData[y][x] : ' ';
}

int Maze::getHeight() const {
    return height;
}

int Maze::getWidth() const {
    return width;
}

std::shared_ptr<Block> Maze::getBlock(int x, int y) const {
    return (inBounds(x, y)) ? blocks[y][x] : nullptr;
}

bool Maze::inBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Maze::setTotalKeys(int k)
{
    totalKeys=k;
}

int Maze::getTotalKeys() {
    return totalKeys;
}