#include <jo/jo.h>

static char board[3][3];
static int currentPlayer = 1; // 1 for player 'X', -1 for player 'O'
static int selectedX = 0, selectedY = 0, winner;
static jo_font *my_font;

/* Initialize the TicTacToe board */
void initialize_board(void) {
    currentPlayer=1;
    winner=0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            board[y][x] = ' ';
        }
    }
}

/* Draw the TicTacToe board */
void draw_board(void) {
    jo_set_printf_color_index(JO_COLOR_INDEX_Blue);
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            jo_printf(10 + x * 3, 10 + y * 2, "[%c]", board[y][x]);
        }
    }

    // Highlight the selected cell
    jo_set_printf_color_index(JO_COLOR_INDEX_Red);
    jo_printf(10 + selectedX * 3, 10 + selectedY * 2, "[");
    jo_printf(12 + selectedX * 3, 10 + selectedY * 2, "]");
}

/* Check for a win or draw */
int check_winner(void) {
    for (int i = 0; i < 3; i++) {
        // Check rows and columns
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // Check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    // Check for draw
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == ' ') {
                return 0; // Still playing
            }
        }
    }

    return 'D'; // Draw
}

/* Handle player move */
void place_marker(void) {
    if (board[selectedY][selectedX] == ' ') {
        board[selectedY][selectedX] = currentPlayer == 1 ? 'X' : 'O';
        currentPlayer = -currentPlayer; // Switch player
    }
}

/* Handle player input for navigation and selecting cells */
void handle_input(void) {
    if (jo_is_pad1_key_down(JO_KEY_LEFT)) {
        selectedX=selectedX==0?2:selectedX-1;
    }
    else if (jo_is_pad1_key_down(JO_KEY_RIGHT)) {
        selectedX=selectedX==2?0:selectedX+1;
    }
    if (jo_is_pad1_key_down(JO_KEY_UP)) {
        selectedY=selectedY==0?2:selectedY-1;
    }
    else if (jo_is_pad1_key_down(JO_KEY_DOWN)) {
        selectedY=selectedY==2?0:selectedY+1;
    }

    if (jo_is_pad1_key_pressed(JO_KEY_A) && !winner) {
        place_marker();
    }
    if(jo_is_pad1_key_pressed(JO_KEY_START) && winner) {
        initialize_board();
    }
}

/* Main drawing function */
void my_draw(void) {
    handle_input();
    draw_board();

    winner = check_winner();
    if (winner == 'X' || winner == 'O') {
        jo_font_printf_centered(my_font, 10, 20, 1.0f, "PLAYER %c WINS!", winner);
    } else if (winner == 'D') {
        jo_font_printf_centered(my_font, 10, 20, 1.0f, "IT'S A DRAW!");
    }
}

/* Main entry point */
void jo_main(void) {
    jo_core_init(JO_COLOR_White);
    initialize_board();
    my_font = jo_font_load(JO_ROOT_DIR, "FONT.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");
    jo_core_add_callback(my_draw);
    jo_core_run();
}
