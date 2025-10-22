#include <stdio.h>
#include "helper.h"

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    char board[ROWS][COLS];
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = '.';

    char player = 'A';

    while (1) {
        print_board(board);
        int col = read_column_choice(player);
        if (col == -1) {
            printf("\nGoodbye!\n");
            break;
        }
        if (!drop_checker(board, col - 1, player)) {
            printf("Column %d is full. Pick a different column.\n", col);
            continue;
        }
        if (is_winner(board, player)) {
            print_board(board);
            printf("\nPlayer %c WINS!\n", player);
            break;
        }
        if (is_draw(board)) {
            print_board(board);
            printf("\nGame is a DRAW.\n");
            break;
        }
        player = (player == 'A') ? 'B' : 'A';
    }
    return 0;
}
