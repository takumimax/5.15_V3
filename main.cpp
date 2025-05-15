#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <limits>     // 為了 std::numeric_limits
#include "Maze.h"
#include "Player.h"
#include "Block.h"

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
    Block block;

    if (!maze.loadMap(argv[1], player)) {
        std::cerr << "讀取地圖失敗\n";
        return 1;
    }

    setNonBlockingInput();
    std::cout << "\033[?25l";     // 隱藏游標
    std::cout << "\033[?1049h";   // 啟用替代 buffer
    maze.draw(player, block);

    while (true) {
        char key;
        if (read(STDIN_FILENO, &key, 1) > 0) {
            if (key == 'e') break;

            int oldX, oldY;
            player.getPosition(oldX, oldY);

            bool goal = player.move(key, maze);
            if (goal) break;

            maze.draw(player, block);
            std::cout.flush();
        }
        usleep(10000);
    }

    // 先恢復輸入模式
    resetInput();

    std::cout << "\033[?25h";     // 顯示游標
    std::cout << "\033[?1049l";   // 回到主 buffer

    // 輸出結束訊息
    std::cout << "\n遊戲結束，請按 Enter...";

    // 清除 std::cin 緩衝區，避免殘留輸入卡住
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 等待使用者按下 Enter
    std::cin.get();

    return 0;
}
