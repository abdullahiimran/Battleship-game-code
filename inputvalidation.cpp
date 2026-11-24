#include "raylib.h"

char boards[2][9][9];
char shipMark[] = { 'X', 'Y' };
int currentPlayer = 0;
int shipSizes[3] = { 2, 3, 4 }, shipsPlaced[2] = { 0, 0 };
bool firstCellSelected = false, allShipsPlaced = false;
int firstRow, firstColumn;

void DrawGrid(int startX, int startY, int cellSize);
bool insideGrid(int startX, int startY, int cellSize, int& row, int& column);
void initializeBoards(char borads[2][9][9]);
bool placementValidation(int player, int row1, int column1, int row2, int column2, int size);
void placeShip(int player, int row1, int col1, int row2, int col2);
void handleShipPlacement(int startX, int startY, int cellSize);
void DrawShips(int player, int startX, int startY, int cellSize);

int main()
{
    InitWindow(800, 600, "Battleship");
    initializeBoards(boards);
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (!allShipsPlaced)
        {
            handleShipPlacement(100, 100, 40);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGrid(100, 100, 40);
        if (currentPlayer == 0)
            DrawShips(0, 100, 100, 40);
        else
            DrawShips(1, 100, 100, 40);
        if (currentPlayer == 0)
            DrawText("PLAYER 1 - PLACE YOUR SHIPS", 100, 50, 25, BLUE);
        else
            DrawText("PLAYER 2 - PLACE YOUR SHIPS", 100, 50, 25, RED);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void initializeBoards(char boards[2][9][9])
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++)
                boards[i][j][k] = '-';
        }
    }
}

void DrawGrid(int startX, int startY, int cellSize)
{
    for (int i = 0; i <= 9; i++)
    {
        DrawLine(startX, startY + i * cellSize,
            startX + 9 * cellSize, startY + i * cellSize,
            BLACK);

        DrawLine(startX + i * cellSize, startY,
            startX + i * cellSize, startY + 9 * cellSize,
            BLACK);
    }
}

bool insideGrid(int startX, int startY, int cellSize, int& row, int& column)
{
    int mouseX = GetMouseX(),
        mouseY = GetMouseY();
    if ((mouseX >= startX && mouseX < startX + 9 * cellSize) && (mouseY >= startY && mouseY < startY + 9 * cellSize))
    {
        column = (mouseX - startX) / cellSize;
        row = (mouseY - startY) / cellSize;
        return true;
    }
    return false;
}

bool placementValidation(int player, int row1, int col1, int row2, int col2, int requiredSize)
{
    if (row1 < 0 || row1 >= 9 || row2 < 0 || row2 >= 9 || col1 < 0 || col1 >= 9 || col2 < 0 || col2 >= 9)
        return false;
    if (row1 != row2 && col1 != col2)
        return false;
    int length;
    if (row1 == row2)
    {
        if (col1 > col2)
            length = col1 - col2 + 1;
        else
            length = col2 - col1 + 1;
    }
    else
    {
        if (row1 > row2)
            length = row1 - row2 + 1;
        else
            length = row2 - row1 + 1;
    }
    if (length != requiredSize)
        return false;
    if (row1 == row2)
    {
        int start = col1 < col2 ? col1 : col2;
        int end = col1 > col2 ? col1 : col2;

        for (int j = start; j <= end; j++)
        {
            if (boards[player][row1][j] != '-')
                return false;
        }
    }
    else
    {
        int start = row1 < row2 ? row1 : row2;
        int end = row1 > row2 ? row1 : row2;
        for (int i = start; i <= end; i++)
        {
            if (boards[player][i][col1] != '-')
                return false;
        }
    }
    return true;
}

void placeShip(int player, int row1, int col1, int row2, int col2)
{
    if (row1 == row2)
    {
        int start = col1 < col2 ? col1 : col2;
        int end = col1 > col2 ? col1 : col2;

        for (int j = start; j <= end; j++)
        {
            boards[player][row1][j] = shipMark[player];
        }
    }
    else
    {
        int start = row1 < row2 ? row1 : row2;
        int end = row1 > row2 ? row1 : row2;

        for (int i = start; i <= end; i++)
        {
            boards[player][i][col1] = shipMark[player];
        }
    }
    TraceLog(LOG_INFO, "Player %d board:", player + 1);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            TraceLog(LOG_INFO, "%c", boards[player][i][j]);
        }
    }
}

void handleShipPlacement(int startX, int startY, int cellSize)
{
    int row, col;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (insideGrid(startX, startY, cellSize, row, col))
        {
            if (firstCellSelected == false)
            {
                firstRow = row;
                firstColumn = col;
                firstCellSelected = true;
            }
            else
            {
                int requiredSize = shipSizes[shipsPlaced[currentPlayer]];
                if (placementValidation(currentPlayer,
                    firstRow, firstColumn, row, col, requiredSize))
                {
                    placeShip(currentPlayer,
                        firstRow, firstColumn, row, col);
                    shipsPlaced[currentPlayer]++;
                    if (shipsPlaced[0] == 3 && shipsPlaced[1] == 3)
                        allShipsPlaced = true;
                    else if (shipsPlaced[currentPlayer] == 3)
                        currentPlayer = 1 - currentPlayer;
                }
                firstCellSelected = false;
            }
        }
    }
}

void DrawShips(int player, int startX, int startY, int cellSize)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (boards[player][i][j] == shipMark[player])
            {
                DrawRectangle(
                    startX + j * cellSize + 2,
                    startY + i * cellSize + 2,
                    cellSize - 4,
                    cellSize - 4,
                    BLUE
                );
            }
        }
    }
}