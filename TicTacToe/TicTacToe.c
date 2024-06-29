// TicTacToe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>

#include "raylib.h"

#define NUM_TILES 3

enum TileState {
    EMPTY = 0,
    X_PLAYED,
    O_PLAYED,
};

enum GameState {
    X_TURN = 0,
    O_TURN,
    X_WIN,
    O_WIN,
};

const int SCREEN_WIDTH = 360;
const int SCREEN_HEIGHT = 470;

int OUTLINE_WIDTH = 3;

int TILE_SIZE = 100;
int TILE_MARGIN = 10;

int SCORE_START_Y = 10;
int SCORE_SIZE_Y = 100;
int BOARD_START_Y = 120;
int TILE_START_X = 20;
int TILE_START_Y = 130;

#define BACKGROUND   (Color) { 192, 192, 192, 255 }
#define DARK_OUTLINE (Color) { 128, 128, 128, 255 }

enum TileState board[NUM_TILES][NUM_TILES] = { 0 };

void ResetBoard(enum TileState board[NUM_TILES][NUM_TILES]) {
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            board[i][j] = 0;
        }
    }
}

enum GameState CheckWin(enum TileState board[NUM_TILES][NUM_TILES]) {
    enum TileState winner = EMPTY;

    // Horizontal
    for (int i = 0; i < NUM_TILES; i++) {
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            printf("Horizontal Winner!\n");
            winner = board[i][0];
        }
    }

    // Vertical
    for (int j = 0; j < NUM_TILES; j++) {
        if (board[0][j] != EMPTY && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
            printf("Vertical Winner!\n");
            winner = board[0][j];
        }
    }

    // Diagonal
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        printf("Diagonal Winner!\n");
        winner = board[0][0];
    }
    else if (board[2][0] != EMPTY && board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
        printf("Diagonal Winnner!\n");
        winner = board[2][0];
    }

    switch (winner) {
    case X_PLAYED:
        return X_WIN;
    case O_PLAYED:
        return O_WIN;
    default:
        return X_TURN;
    }
}

void DrawOutline(Vector2 start, Vector2 size, bool invert) {
    Color color1 = WHITE;
    Color color2 = DARK_OUTLINE;
    if (invert) {
        color1 = DARK_OUTLINE;
        color2 = WHITE;
    }

    // Left
    DrawRectangle(start.x, start.y, OUTLINE_WIDTH, size.y - OUTLINE_WIDTH, color1);
    DrawTriangle((Vector2){ start.x + OUTLINE_WIDTH, start.y + size.y - OUTLINE_WIDTH }, (Vector2){ start.x, start.y + size.y - OUTLINE_WIDTH}, (Vector2){ start.x, start.y + size.y }, color1);

    // Top
    DrawRectangle(start.x, start.y, size.x - OUTLINE_WIDTH, OUTLINE_WIDTH, color1);
    DrawTriangle((Vector2){ start.x + size.x, start.y }, (Vector2) { start.x + size.x - OUTLINE_WIDTH, start.y }, (Vector2){ start.x + size.x - OUTLINE_WIDTH, start.y + OUTLINE_WIDTH }, color1);

    // Bottom
    DrawRectangle(start.x + OUTLINE_WIDTH, start.y + size.y - OUTLINE_WIDTH, size.x - OUTLINE_WIDTH, OUTLINE_WIDTH, color2);
    DrawTriangle((Vector2){ start.x, start.y + size.y }, (Vector2){ start.x + OUTLINE_WIDTH, start.y + size.y },  (Vector2){ start.x + OUTLINE_WIDTH, start.y + size.y - OUTLINE_WIDTH}, color2);

    // Right
    DrawRectangle(start.x + size.x - OUTLINE_WIDTH, start.y + OUTLINE_WIDTH, OUTLINE_WIDTH, size.y - OUTLINE_WIDTH, color2);
    DrawTriangle((Vector2){ start.x + size.x - OUTLINE_WIDTH, start.y + OUTLINE_WIDTH }, (Vector2){ start.x + size.x, start.y + OUTLINE_WIDTH}, (Vector2){ start.x + size.x, start.y }, color2);
}

void DrawScore() {
    DrawOutline((Vector2) { 10, 10 }, (Vector2) { SCREEN_WIDTH - 20, 100 }, true);
}

void DrawBoard() {
    int board_size = TILE_MARGIN * (NUM_TILES + 1) + (TILE_SIZE * NUM_TILES);
    DrawOutline((Vector2) { 10, BOARD_START_Y}, (Vector2) { SCREEN_WIDTH - 20, board_size }, true);
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            int x_pos = (TILE_MARGIN * j) + (TILE_SIZE * j) + TILE_START_X;
            int y_pos = (TILE_MARGIN * i) + (TILE_SIZE * i) + TILE_START_Y;
            Color color;
            switch (board[i][j]) {
            case X_PLAYED:
                DrawLineEx((Vector2) { x_pos + 15, y_pos + 15 }, (Vector2) { x_pos + TILE_SIZE - 15, y_pos + TILE_SIZE - 15 }, 10, RED);
                DrawLineEx((Vector2) { x_pos + 15, y_pos + TILE_SIZE - 15 }, (Vector2) { x_pos + TILE_SIZE - 15, y_pos + 15}, 10, RED);
                break;
            case O_PLAYED:
                DrawCircle(x_pos + (TILE_SIZE / 2), y_pos + (TILE_SIZE / 2), (TILE_SIZE /2 ) - 10, BLUE);
                DrawCircle(x_pos + (TILE_SIZE / 2), y_pos + (TILE_SIZE / 2), (TILE_SIZE / 2) - 20, BACKGROUND);
                break;
            }
            DrawOutline((Vector2) { x_pos, y_pos }, (Vector2) { TILE_SIZE, TILE_SIZE }, false);
        }
    }
}

Vector2 CheckCollision(Vector2 mouse_pos) {
    Vector2 collision = { -1, -1 };
    for (int i = 0; i < NUM_TILES; i++) {
        int y_pos = (TILE_MARGIN * i) + (TILE_SIZE * i) + TILE_START_Y;
        if (mouse_pos.y < y_pos || mouse_pos.y > y_pos + TILE_SIZE) {
            continue;
        }

        for (int j = 0; j < NUM_TILES; j++) {
            int x_pos = (TILE_MARGIN * j) + (TILE_SIZE * j) + TILE_START_X;
            if (mouse_pos.x >= x_pos && mouse_pos.x <= x_pos + TILE_SIZE) {
                return (Vector2){j, i};
            }
        }
    }

    return collision;
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic-Tac-Toe");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        Vector2 mouse_pos = { 0 };
        if (IsGestureDetected(GESTURE_TAP)) {
            mouse_pos = GetMousePosition();
            Vector2 collision = CheckCollision(mouse_pos);
            
            if (collision.x != -1 && collision.y != -1) {
                board[(int)collision.y][(int)collision.x] += 1;

                if (board[(int)collision.y][(int)collision.x] == 3) {
                    board[(int)collision.y][(int)collision.x] = 0;
                }
            }
        }

        enum GameState win = CheckWin(board);
        if (win == X_WIN || win == O_WIN) {
            printf("Someone Won!\n");
            ResetBoard(board);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BACKGROUND);

        DrawOutline((Vector2) { 0, 0 }, (Vector2) { SCREEN_WIDTH, SCREEN_HEIGHT }, false);
        DrawScore();
        DrawBoard();
        EndDrawing();
        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
