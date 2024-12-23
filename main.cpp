#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include"header.h"

int main() {
    char play[] = "Play";
    char settings[] = "Settings";
    char quit[] = "Quit";
    char *levelNameList[] = {play, settings, quit};
    int sizes[] = {4, 8, 4};

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};

    int select = 0;
    bool toMenu = false;
    bool quitGame = false;
    int height = 20;
    int width = 20;
    int totalMines = 20;

    while(1){
        toMenu = false;
        switch(select = levelSelect(levelNameList, sizes, 3)){
            case 1:
                break;
            case 2:
                settingsMenu(&totalMines, &height);
                printf("%d", totalMines);
                continue;
            case 3:
                quitGame = true;
                break;
        }
        if(quitGame == true){
            break;
        }

        while(toMenu == false){
            int gameState = 0;
            int cursorX = height/2;
            int cursorY = width/2;

            int mines[20][20] = {0};
            int flags[20][20] = {0};
            int mineCount[20][20] = {0};
            char board[20][40];

            placeMinesRandomly(mines, height, width, totalMines);
            system("cls");

            initializeVisualBoard(board, height, width);

            createMineCountArray(mines, mineCount, height, width);

            displayMap(hConsole, topLeft, height, width, cursorY, cursorX, board, mines, flags, mineCount);
            while (gameState == 0) {
                if (_kbhit()) {
                    char key = _getch();
                    if (key == 27) { // ESC ��h�X
                        break;
                    }

                    // �ھګ����s��m
                    switch (key) {
                        case 'w': // �V�W
                        case 'W':
                            if (cursorY > 0) cursorY--;
                            break;
                        case 's': // �V�U
                        case 'S':
                            if (cursorY < height - 1) cursorY++;
                            break;
                        case 'a': // �V��
                        case 'A':
                            if (cursorX > 0) cursorX-=2;
                            break;
                        case 'd': // �V�k
                        case 'D':
                            if (cursorX < width*2 - 2) cursorX+=2; // ��h�ؼe��
                            break;
                        case 'f':
                        case 'F':
                            if(flags[cursorY][cursorX/2] == 0 && (mines[cursorY][cursorX/2] == 0 || mines[cursorY][cursorX/2] == -1)){
                                flags[cursorY][cursorX/2] = 1;
                            }
                            else{
                                flags[cursorY][cursorX/2] = 0;
                            }
                            break;
                        case ' ':
                            dig(mines, flags, mineCount, cursorY, cursorX/2, height, width);
                            gameState = checkGameState(mines, flags, height, width, totalMines);
                    }
                    displayMap(hConsole, topLeft, height, width, cursorY, cursorX, board, mines, flags, mineCount);
                }
                Sleep(20);
            }
            switch(gameState){
                case 1:
                    win(mines, flags, height, width);
                    displayMap(hConsole, topLeft, height, width, cursorY, cursorX, board, mines, flags, mineCount);
                    printf("\n\n�AĹ�F!\n");
                    toMenu = true;
                    system("pause");
                    break;
                case -1:
                    loose(mines, height, width);
                    displayMap(hConsole, topLeft, height, width, cursorY, cursorX, board, mines, flags, mineCount);
                    printf("\n\n�A���a�p�F!\n");
                    toMenu = true;
                    system("pause");
                    break;
                case 0:
                    printf("\n�A�Q�n�h�X��?(��enter��h�X�A���䥦�����)");
                        char key = _getch();
                        if (key == 13) { // enter ��h�X
                            toMenu = true;
                        }
                    break;
            }
        }
    }
}
