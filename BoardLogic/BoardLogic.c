#include "BoardLogic.h"

enum GameState CheckHorizontalWin(enum GameState board[NUM_TILES][NUM_TILES]) {
    enum GameState potential_winner = START;
    for (int i = 0; i < NUM_TILES; i++) {
        potential_winner = board[i][0];
        if (potential_winner == START) {
            continue;
        }

        for (int j = 0; j < NUM_TILES - 1; j++) {
            if (board[i][j] != board[i][j + 1]) {
                potential_winner = START;
                break;
            }
        }

        if (potential_winner != START) {
            break;
        }
    }

    return potential_winner;
}

enum GameState CheckVerticalWin(enum GameState board[NUM_TILES][NUM_TILES]) {
    enum GameState potential_winner = START;
    for (int j = 0; j < NUM_TILES; j++) {
        potential_winner = board[0][j];
        if (potential_winner == START) {
            continue;
        }

        for (int i = 0; i < NUM_TILES - 1; i++) {
            if (board[i][j] != board[i + 1][j]) {
                potential_winner = START;
                break;
            }
        }

        if (potential_winner != START) {
            break;
        }
    }

    return potential_winner;
}

enum GameState CheckDiagonalWin(enum GameState board[NUM_TILES][NUM_TILES]) {
    enum GameState potential_winner = board[0][0];
    for (int i = 0; i < NUM_TILES - 1; i++) {
        if (board[i][i] != board[i + 1][i + 1]) {
            potential_winner = START;
            break;
        }
    }

    if (potential_winner != START) {
        return potential_winner;
    }

    potential_winner = board[0][NUM_TILES - 1];
    for (int i = 0; i < NUM_TILES - 1; i++) {
        if (board[NUM_TILES - (i + 1)][i] != board[NUM_TILES - (i + 2)][i + 1]) {
            potential_winner = START;
            break;
        }
    }

    return potential_winner;
}


enum GameState CheckWin(enum GameState board[NUM_TILES][NUM_TILES])
{
    enum GameState winner = START;

    if ((winner = CheckHorizontalWin(board)) != START) {
        return winner + TURN_WIN_OFFSET;
    }

    if ((winner = CheckVerticalWin(board)) != START) {
        return winner + TURN_WIN_OFFSET;
    }

    if ((winner = CheckDiagonalWin(board)) != START) {
        return winner + TURN_WIN_OFFSET;
    }

    return winner;
}

bool CheckTie(enum GameState board[NUM_TILES][NUM_TILES]) {
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            if (board[i][j] == START) {
                return false;
            }
        }
    }

    return true;
}

int rate_position(enum GameState board[NUM_TILES][NUM_TILES], enum GameState turn)
{
    enum GameState winner = CheckWin(board);
    if (winner == X_WIN) {
        return 100;
    }
    else if (winner == O_WIN) {
        return -100;
    }

    enum GameState next_turn = turn == X_TURN ? O_TURN : X_TURN;
    int possible_moves = 0;
    int scores = 0;
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            if (board[i][j] != START) {
                continue;
            }

            possible_moves += 1;
            board[i][j] = next_turn;
            scores += rate_position(board, next_turn);
            board[i][j] = START;
        }
    }

    if (possible_moves == 0) {
        return 0;
    }

    //printf("Scores: %d\n", scores);
    //printf("Possible Moves: %d\n", possible_moves);
    return scores / possible_moves;
}

void ResetBoard(enum GameState board[NUM_TILES][NUM_TILES]) {
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            board[i][j] = START;
        }
    }
}