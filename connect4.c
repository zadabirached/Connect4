#include <stdio.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 7

void clear_screen() { printf("\033[2J\033[H"); }

void print_board(char board[ROWS][COLS]) {
    clear_screen();
    printf("\n   1 2 3 4 5 6 7\n");
    for (int i = 0; i < ROWS; i++) {
        printf("   ");
        for (int j = 0; j < COLS; j++) printf("%c ", board[i][j]);
        printf("\n");
    }
}

int drop_checker(char board[ROWS][COLS], int col, char token) {
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '.') { board[i][col] = token; return 1; }
    }
    return 0; // column full
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
                (check_direction(board,r,c,0,1,token)   ||
                 check_direction(board,r,c,1,0,token)   ||
                 check_direction(board,r,c,1,1,token)   ||
                 check_direction(board,r,c,-1,1,token)))
                return 1;
    return 0;
}

int is_draw(char board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++) if (board[0][c] == '.') return 0;
    return 1;
}

int main() {
    char board[ROWS][COLS];
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) board[i][j] = '.';

    int col; char player = 'A';
    while (1) {
        print_board(board);
        printf("\nPlayer %c, choose a column (1-7): ", player);
        if (scanf("%d", &col) != 1) { printf("Invalid input.\n"); return 0; }
        if (col < 1 || col > 7) continue;
        if (!drop_checker(board, col - 1, player)) continue;

        if (is_winner(board, player)) { print_board(board); printf("\nPlayer %c WINS!\n", player); break; }
        if (is_draw(board))           { print_board(board); printf("\nGame is a DRAW.\n"); break; }

        player = (player == 'A') ? 'B' : 'A';
    }
    return 0;
}
