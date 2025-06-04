#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <limits>
#include <cstdio>      // popen, pclose
#include <vector>
#include <string>
#include <algorithm>
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

// 讀取 maps 資料夾所有 .txt 地圖檔案，回傳檔名清單
std::vector<std::string> getMapFiles(const std::string& folder) {
    std::vector<std::string> files;
    std::string cmd = "ls " + folder + "/*.txt 2> /dev/null";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "無法讀取資料夾: " << folder << std::endl;
        return files;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string filepath(buffer);
        if (!filepath.empty() && filepath.back() == '\n')
            filepath.pop_back();
        files.push_back(filepath);
    }
    pclose(pipe);

    std::sort(files.begin(), files.end());
    return files;
}

int main(int argc, char* argv[]) {
    std::string mapFolder = "maps";

    // 決定地圖清單來源
    std::vector<std::string> mapFiles;
    if (argc >= 2) {
        std::string arg1(argv[1]);
        if (arg1.find(".txt") != std::string::npos) {
            mapFiles.push_back(arg1);
        } else {
            mapFiles = getMapFiles(arg1);
            if (mapFiles.empty()) {
                std::cerr << "資料夾 " << arg1 << " 無任何 .txt 地圖檔案\n";
                return 1;
            }
        }
    } else {
        mapFiles = getMapFiles(mapFolder);
        if (mapFiles.empty()) {
            std::cerr << "資料夾 " << mapFolder << " 無任何 .txt 地圖檔案\n";
            return 1;
        }
    }

    setNonBlockingInput();
    std::cout << "\033[?25l";     // 隱藏游標
    std::cout << "\033[?1049h";   // 啟用替代 buffer

    for (size_t level = 0; level < mapFiles.size(); ++level) {
    
        // 設定該關卡需要的鑰匙數 = 關卡編號 + 1（第1關要1把鑰匙）
        Maze::setTotalKeys(level + 1);
        
        const auto& mapFile = mapFiles[level];
        Maze maze;
        Player player;

        std::cout << "\n=== 載入地圖: " << mapFile << " ===\n";

        

        if (!maze.loadMap(mapFile, player)) {
            std::cerr << "載入失敗，跳過此關\n";
            continue;
        }

        maze.updateVisibility(player);
        maze.draw(player);
        player.drawStatus(maze.getHeight() * 3);

        bool levelComplete = false;

        while (true) {
            char key;
            if (read(STDIN_FILENO, &key, 1) > 0) {
                if (key == 'e') {
                    resetInput();
                    std::cout << "\033[?25h";
                    std::cout << "\033[?1049l";
                    std::cout << "\n遊戲提早結束！\n";
                    return 0;
                }

                bool moved = player.move(key, maze);

                if (moved && player.hasReachedGoal()) {
                    maze.draw(player);
                    player.drawStatus(maze.getHeight() * 3);
                    levelComplete = true;
                    break;
                }

                if (player.isDead()) {
                    maze.draw(player);
                    player.drawStatus(maze.getHeight() * 3);
                    break;
                }

                if (moved) {
                    maze.draw(player);
                    player.drawStatus(maze.getHeight() * 3);
                    std::cout.flush();
                }
            }
            usleep(10000);
        }

        if (player.isDead()) {
            std::cout << "\n你已死亡，遊戲結束！\n";
            break;
        } else if (levelComplete) {
            std::cout << "\n恭喜通過關卡！準備進入下一關...\n";
            usleep(1000000);
        }
    }

    resetInput();
    std::cout << "\033[?25h";
    std::cout << "\033[?1049l";

    std::cout << "\n所有關卡結束，感謝遊玩！\n";
    std::cout << "請按 Enter 結束...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
