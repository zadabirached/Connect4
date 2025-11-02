#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "helper.h"

void clear_screen(void) { }

void print_board(char board[ROWS][COLS]) {
    puts("");
    printf("  1 2 3 4 5 6 7\n");
    for (int i = 0; i < ROWS; i++) {
        printf("  ");
        for (int j = 0; j < COLS; j++) printf("%c ", board[i][j]);
        puts("");
    }
    fflush(stdout);
}

int drop_checker(char board[ROWS][COLS], int col, char token) {
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '.') { board[i][col] = token; return 1; }
    }
    return 0;
}

int check_direction(char board[ROWS][COLS], int r, int c, int dr, int dc, char token) {
    for (int i = 0; i < 4; i++) {
        int nr = r + dr * i, nc = c + dc * i;
        if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) return 0;
        if (board[nr][nc] != token) return 0;
    }
    return 1;
}

int is_winner(char board[ROWS][COLS], char token) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (board[r][c] == token &&
                (check_direction(board,r,c,0,1,token)||
                 check_direction(board,r,c,1,0,token)||
                 check_direction(board,r,c,1,1,token)||
                 check_direction(board,r,c,-1,1,token)))
                return 1;
    return 0;
}

int is_draw(char board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++) if (board[0][c] == '.') return 0;
    return 1;
}

int read_column_choice(char player) {
    char line[128];
    for (;;) {
        printf("\nPlayer %c, choose a column (1-7): ", player);
        if (!fgets(line, sizeof(line), stdin)) return -1;
        size_t len = strlen(line);
        if (len && line[len - 1] == '\n') line[len - 1] = '\0';
        errno = 0;
        char *endp = NULL;
        long val = strtol(line, &endp, 10);
        if (endp == line || errno == ERANGE || *endp != '\0') { printf("Invalid input. Please enter a number from 1 to 7.\n"); continue; }
        if (val < 1 || val > 7) { printf("Invalid input. Please enter a number from 1 to 7.\n"); continue; }
        return (int)val;
    }
}

static int first_free_row(char board[ROWS][COLS], int col0) {
    for (int r = ROWS - 1; r >= 0; r--) if (board[r][col0] == '.') return r;
    return -1;
}

static int wins_if_drop_here(char board[ROWS][COLS], int col0, char token) {
    int r = first_free_row(board, col0);
    if (r < 0) return 0;
    board[r][col0] = token;
    int ok = 0;
    int dirs[4][2] = {{0,1},{1,0},{1,1},{-1,1}};
    for (int d = 0; d < 4 && !ok; d++) {
        int dr = dirs[d][0], dc = dirs[d][1];
        for (int k = -3; k <= 0 && !ok; k++) {
            int sr = r + k*dr, sc = col0 + k*dc;
            if (check_direction(board, sr, sc, dr, dc, token)) ok = 1;
        }
    }
    board[r][col0] = '.';
    return ok;
}

int bot_choose_random_col(char board[ROWS][COLS]) {
    int candidates[COLS], n = 0;
    for (int c = 0; c < COLS; c++) if (board[0][c] == '.') candidates[n++] = c;
    if (n == 0) return -1;
    int pick = rand() % n;
    return candidates[pick] + 1;
}

int bot_choose_medium_col(char board[ROWS][COLS], char bot_token, char human_token) {
    for (int c = 0; c < COLS; c++) if (board[0][c] == '.') if (wins_if_drop_here(board, c, bot_token)) return c + 1;
    for (int c = 0; c < COLS; c++) if (board[0][c] == '.') if (wins_if_drop_here(board, c, human_token)) return c + 1;
    return bot_choose_random_col(board);
}
