// main.cpp
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <limits>
#include "Maze.h"
#include "Player.h"

void setNonBlockingInput() {
    termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void resetInput() {
    termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "使用方式: " << argv[0] << " inputfile\n";
        return 1;
    }

    Maze maze;
    Player player;

    if (!maze.loadMap(argv[1], player)) {
        std::cerr << "讀取地圖失敗\n";
        return 1;
    }

    setNonBlockingInput();
    std::cout << "\033[?25l";     // 隱藏游標
    std::cout << "\033[?1049h";   // 啟用替代 buffer
    maze.draw(player);
    player.drawStatus(maze.getHeight() * 3);

    while (true) {
        char key;
        if (read(STDIN_FILENO, &key, 1) > 0) {
            if (key == 'e') break;

            bool moved = player.move(key, maze);

            // 移動成功後再檢查是否抵達終點
            if (moved && player.hasReachedGoal()) break;

            // 新增死亡判斷
            if (player.isDead()) {
                maze.draw(player);
                player.drawStatus(maze.getHeight() * 3);
                
                break;
            }


            player.drawStatus(maze.getHeight() * 3);
            // 只有移動成功才需要重畫
            if (moved) {
                maze.draw(player);
                player.drawStatus(maze.getHeight() * 3);
                std::cout.flush();
            }
        }
        usleep(10000);
    }

    resetInput();

    std::cout << "\033[?25h";     // 顯示游標
    std::cout << "\033[?1049l";   // 回到主 buffer

    if (player.isDead()) std::cout << "\n你已死亡，遊戲結束！\n";

    std::cout << "\n遊戲結束，請按 Enter...";

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
