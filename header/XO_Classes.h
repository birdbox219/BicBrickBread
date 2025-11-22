/**
 * @file XO_Classes.h
 * @brief Defines the X-O (Tic-Tac-Toe) specific classes that extend the generic board game framework.
 *
 * This file provides:
 * - `X_O_Board`: A specialized board class for the Tic-Tac-Toe game.
 * - `XO_UI`: A user interface class tailored to X-O game setup and player interaction.
 */

#ifndef XO_CLASSES_H
#define XO_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

/**
 * @class X_O_Board
 * @brief Represents the Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Tic-Tac-Toe (X-O) game, including
 * move updates, win/draw detection, and display functions.
 *
 * @see Board
 */
class X_O_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    /**
     * @brief Default constructor that initializes a 3x3 X-O board.
     */
    X_O_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has won the game.
     * @param player Pointer to the player being checked.
     * @return true if the player has a winning line, false otherwise.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in X-O logic).
     */
    bool is_lose(Player<char>*) { return false; };

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all cells are filled and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);
};

// Implementation of X_O_Board
X_O_Board::X_O_Board() : Board<char>(3, 3) {
    rows = 3;
    columns = 3;
    board = vector<vector<char>>(rows, vector<char>(columns));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != blank_symbol) {
        return false;
    }
    board[x][y] = move->get_symbol();
    n_moves++;
    return true;
}

bool X_O_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == s && board[i][1] == s && board[i][2] == s) ||
            (board[0][i] == s && board[1][i] == s && board[2][i] == s)) {
            return true;
        }
    }
    // Check diagonals
    if ((board[0][0] == s && board[1][1] == s && board[2][2] == s) ||
        (board[0][2] == s && board[1][1] == s && board[2][0] == s)) {
        return true;
    }
    return false;
}

bool X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @class XO_UI
 * @brief User Interface class for the X-O (Tic-Tac-Toe) game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O-specific functionality for player setup and move input.
 *
 * @see UI
 */
class XO_UI : public UI<char> {
public:
    /**
     * @brief Constructs an XO_UI object.
     *
     * Initializes the base `UI<char>` class with the welcome message "FCAI X-O".
     */
    XO_UI() : UI<char>("Welcome to FCAI X-O Game", 5) {}

    /**
     * @brief Destructor for XO_UI.
     */
    ~XO_UI() {};

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol ('X' or 'O') assigned to the player.
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) {
        return new Player<char>(name, symbol, type); // Simplified
    }

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    virtual Move<char>* get_move(Player<char>* player) {
        int x, y;
        cout << "Enter move for " << player->get_name() << " (x y): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
};

#endif // XO_CLASSES_H
