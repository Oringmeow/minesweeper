#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
void clearScreen(HANDLE hConsole, COORD topLeft, DWORD screenSize) {
    DWORD charsWritten;
    FillConsoleOutputCharacter(hConsole, ' ', screenSize, topLeft, &charsWritten);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screenSize, topLeft, &charsWritten);
    SetConsoleCursorPosition(hConsole, topLeft);
}
void SetColor(int textColor, int backgroundColor, bool brightBackground = false) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 背景顏色左移 4 位，合併文本顏色
    int colorAttribute = textColor | (backgroundColor << 4);

    // 如果需要亮背景，加入 BACKGROUND_INTENSITY 屬性
    if (brightBackground) {
        colorAttribute |= BACKGROUND_INTENSITY;
    }

    SetConsoleTextAttribute(hConsole, colorAttribute);
}
void printTitle(){
    SetColor(3, 0);
    std::cout << R"(
+============================================================+)";
    SetColor(11, 0);
    std::cout << R"(
| __  __ _            ____                                   |
||  \/  (_)_ __   ___/ ___|_      _____  ___ _ __   ___ _ __ |
|| |\/| | | '_ \ / _ \___ \ \ /\ / / _ \/ _ \ '_ \ / _ \ '__||
|| |  | | | | | |  __/___) \ V  V /  __/  __/ |_) |  __/ |   |
||_|  |_|_|_| |_|\___|____/ \_/\_/ \___|\___| .__/ \___|_|   |
|                                           |_|              |)";
    SetColor(3, 0);
    std::cout << R"(
+============================================================+)";
    SetColor(7, 0);
    printf("\n              用WASD控制選取,F插旗子,空白鍵挖開");
    printf("\n \n");
}
void printSelector(char **levelNameList, const int sizes[],int levelCount, int select){
    printTitle();
    for(int a = 1;a<=levelCount;a++){
        for(int i = 0;i<(62/2) - (sizes[a-1]/2);i++){
            printf(" ");
        }
        if(select == a){
            SetColor(7,2);
        }
        for(int b = 0; b<sizes[a-1];b++){
            printf("%c", levelNameList[a-1][b]);
        }
        SetColor(7,0);
        printf("\n \n");
    }
}
int levelSelect(char **levelNameList, const int sizes[],int levelCount){
    bool selected = false;
    int select = 1;

    system("cls");
    printSelector(levelNameList, sizes, levelCount, select);

    while(selected == false){
        if (_kbhit()) {
            char key = _getch();
            switch(key){
                case 72:
                case 'w':
                case 'W':
                    if(select > 1){
                        select-=1;
                    }
                    break;
                case 80:
                case 's':
                case 'S':
                    if(select < (levelCount)){
                        select+=1;
                    }
                    break;
                case 13:
                    return select;
                    break;
            }
            system("cls");
            printSelector(levelNameList, sizes, levelCount, select);
        }
    }
}
void loose(int mines[][20], int height, int width){
    for(int a = 0;a<height;a++){
        for(int b = 0;b<width;b++){
            if(mines[a][b] == -1 || mines[a][b] == -2){
                mines[a][b] = -2;
            }
        }
    }
}
void win(int mines[][20], int flags[][20], int height, int width){
    for(int a = 0;a<height;a++){
        for(int b = 0;b<width;b++){
            if(mines[a][b] == -1){
                flags[a][b] = 1;
            }
            else if(mines[a][b] == 0){
                mines[a][b] = 1;
            }
        }
    }
}
int checkGameState(const int mines[][20], const int flags[][20], int height, int width, int minesCount){
    int openCount = 0;
    for(int a = 0;a<height;a++){
        for(int b = 0;b<width;b++){
            if(mines[a][b] == -2){
                return -1;
            }
            else if(mines[a][b] == 1){
                openCount ++;
            }
        }
    }
    if(openCount == ((height*width) - minesCount)){
        return 1;
    }
    else{
        return 0;
    }
}

void displayMap(HANDLE hConsole, COORD topLeft, int height, int width, int cursorY, int cursorX, char board[][40], const int mine[][20], const int flag[][20], const int mineCount[][20]) {
    DWORD screenSize = height * (width * 2);
    clearScreen(hConsole, topLeft, screenSize);

    bool insertBlank = false;
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width * 2; b++) {
            if (mine[a][b / 2] == 0 || mine[a][b / 2] == -1) {
                SetColor(0, 0, true);
                if (flag[a][b / 2] == 1) {
                    board[a][b] = '/';
                } else {
                    board[a][b] = ' ';
                }
            } else if (mine[a][b / 2] == 1) {
                char num = '0' + mineCount[a][b / 2];
                if (insertBlank == true) {
                    insertBlank = false;
                    board[a][b] = ' ';
                } else if (mineCount[a][b / 2] != 0) {
                    insertBlank = true;
                    board[a][b] = num;
                    SetColor(7, mineCount[a][b / 2]);
                } else {
                    SetColor(0, 0);
                }
            } else if (mine[a][b / 2] == -2) {
                if (insertBlank == true) {
                    insertBlank = false;
                    board[a][b] = ' ';
                } else {
                    insertBlank = true;
                    board[a][b] = '*';
                }
                SetColor(7, 4);
            }

            if ((a == cursorY && b == cursorX) || (a == cursorY && b == cursorX + 1)) {
                SetColor(7, 15);
            }
            printf("%c", board[a][b]);
        }
        printf("\n");
    }
    printf("\n\n");
    SetColor(7, 0);
    printf("按Esc退出 ");

}
int countSurrounding(const int mine[][20], int y, int x, int height, int width, int target) {
    int count = 0;

    // 遍歷周圍 8 個格子
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // 跳過自己本身的格子
            if (i == 0 && j == 0) continue;

            int newY = y + i;
            int newX = x + j;

            // 檢查邊界
            if (newY >= 0 && newY < height && newX >= 0 && newX < width) {
                if (mine[newY][newX] == target) { // -1 表示地雷
                    count++;
                }
            }
        }
    }
    return count;
}


// 計算並輸出地雷數量陣列
void createMineCountArray(const int mine[][20], int mineCount[][20], int height, int width) {
    // 計算每個格子的地雷數量
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            mineCount[row][col] = countSurrounding(mine, row, col, height, width, -1);
        }
    }
}
void dig(int mines[][20], const int flags[][20], const int mineCount[][20], int y, int x, int height, int width){
    if(flags[y][x] == 0 && mines[y][x] == 0){
        mines[y][x] = 1;
        if(mineCount[y][x] == 0){
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    // 跳過自己本身的格子
                    if (i == 0 && j == 0) continue;

                    int newY = y + i;
                    int newX = x + j;

                    // 檢查邊界
                    if (newY >= 0 && newY < height && newX >= 0 && newX < width) {
                        dig(mines, flags, mineCount, newY, newX, height, width);
                    }
                }
            }
        }
    }
    else if(mines[y][x] == 1 && mineCount[y][x] != 0){
        if(countSurrounding(flags, y, x, height, width, 1) == mineCount[y][x]){
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    // 跳過自己本身的格子
                    if (i == 0 && j == 0) continue;

                    int newY = y + i;
                    int newX = x + j;

                    // 檢查邊界
                    if (newY >= 0 && newY < height && newX >= 0 && newX < width && flags[newY][newX] == 0 && (mines[newY][newX] == -1 || mines[newY][newX] == 0)) {
                        dig(mines, flags, mineCount, newY, newX, height, width);
                    }
                }
            }
        }


    }
    else if(flags[y][x] == 0 && mines[y][x] == -1){
        mines[y][x] = -2;
    }
}
void placeMinesRandomly(int mines[][20], int height, int width, int totalMines){
    srand(time(0));
    int placedMines = 0;
    while (placedMines < totalMines) {
        int x = rand() % width;
        int y = rand() % height;
        if (mines[y][x] == 0) {
            mines[y][x] = -1;
            placedMines++;
        }
    }
}
void initializeVisualBoard(char board[][40], int height, int width){
    for(int a = 0;a<height;a++){
        for(int b = 0;b<width*2;b++){
            board[a][b] = ' ';
        }
    }
}
void settingsPrint(int select){
    if(select == 1){
        SetColor(7, 2);
    }else{
        SetColor(7, 0);
    }
    printf("地雷數量");
    SetColor(7, 0);
}
bool settingsFunction(int *totalMines, int *height, int select, int input){
     switch(select){
        case 1:
            if(input > 30){
                printf("地雷數量過多! 請重新輸入!\n");
                if(_kbhit()){
                    return false;
                }
            }else{
                *totalMines = input;
                return true;
            }
            break;
    }
}
void settingsMenu(int *totalMines, int *height){
    system("cls");
    int select = 1;
    bool quit = false;
    settingsPrint(select);
    while(quit == false){
        if (_kbhit()) {
            system("cls");
            settingsPrint(select);
            char key = _getch();
            switch(key){
                case 13:
                    while(1){
                        int input = 0;
                        system("cls");
                        printf("請輸入...>");
                        scanf("%d", &input);
                        if(settingsFunction(totalMines, height, select,input) == true){
                            printf("success!\n");
                            system("pause");
                            system("cls");
                            settingsPrint(select);
                            break;
                        }else{
                            system("pause");
                            system("cls");
                            settingsPrint(select);
                        }
                    }
                    break;
                case 27:
                    quit = true;
                    break;
            }
        }
    }
}
