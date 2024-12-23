#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
void clearScreen(HANDLE hConsole, COORD topLeft, DWORD screenSize) ;

void SetColor(int textColor, int backgroundColor, bool brightBackground = false) ;

void printTitle();

void printSelector(char **levelNameList, const int sizes[],int levelCount, int select);

int levelSelect(char **levelNameList, const int sizes[],int levelCount);

void loose(int mines[][20], int height, int width);

void win(int mines[][20], int flags[][20], int height, int width);

void displayMap(HANDLE hConsole, COORD topLeft, int height, int width, int cursorY, int cursorX, char board[][40], const int mine[][20], const int flag[][20], const int mineCount[][20]) ;

void createMineCountArray(const int mine[][20], int mineCount[][20], int height, int width) ;

void dig(int mines[][20], const int flags[][20], const int mineCount[][20], int y, int x, int height, int width);

void placeMinesRandomly(int mines[][20], int height, int width, int totalMines);

void initializeVisualBoard(char board[][40], int height, int width);

void settingsPrint(int select);

bool settingsFunction(int *totalMines, int *height, int select, int input);

void settingsMenu(int *totalMines, int *height);
int checkGameState(const int mines[][20], const int flags[][20], int height, int width, int minesCount);



