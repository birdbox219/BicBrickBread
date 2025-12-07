#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include <queue>
#include "XO_inf.h"

using namespace std;

//--------------------------------------- XO_inf_Board Implementation

/**
 * @brief Constructs an empty XO_inf_Board.
 *
 * @details
 * Initializes a 3x3 board with all cells set to the blank symbol.
 */
XO_inf_Board::XO_inf_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Apply or undo a move on the board.
 *
 * @param move Pointer to the move to apply. If `move->get_symbol() == 0`, it
 *        represents an undo operation.
 * @return True if the move was successfully applied, false if invalid.
 *
 * @details
 * - For normal moves (`mark != 0`):
 *   - Validates cell coordinates and occupancy.
 *   - Updates the board and history.
 *   - Increments move counter `cnt` modulo 3.
 *   - Every third move, oldest move is pushed to `undoHistory`.
 * - For undo moves (`mark == 0`):
 *   - Decrements move counter and resets cell.
 *   - Restores move from `undoHistory` if needed.
 *   - Updates `cnt` modulo 3.
 */
bool XO_inf_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns || (board[x][y] != blank_symbol && mark != 0))
        return false;

    if (mark == 0) { // Undo move
        --n_moves;
        board[x][y] = blank_symbol;
        history.pop_back();
        if(cnt == 0 && !undoHistory.empty()) {
            history.push_front(undoHistory.top());
            undoHistory.pop();
        }
        cnt = (cnt - 1 + 3) % 3;
        return true;
    }

    // Apply normal move
    board[x][y] = mark;
    ++n_moves;
    history.push_back({x, y});
    ++cnt;
    cnt %= 3;
    if (cnt == 0 && !history.empty()) {
        undoHistory.push(history.front());
        history.pop_front();
    }

    return true;
}

/**
 * @brief Checks if the given player has won.
 *
 * @param player Pointer to the player to check.
 * @return True if the player has a winning line; false otherwise.
 */
bool XO_inf_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Checks if the game is a draw.
 *
 * @param player Pointer to the player (ignored in this context).
 * @return True if all cells are filled and no player has won.
 */
bool XO_inf_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

/**
 * @brief Checks if the game is over.
 *
 * @param player Pointer to the player to check for.
 * @return True if the game ended via win or draw.
 */
bool XO_inf_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_inf_AI Implementation

/**
 * @brief Evaluate the board from AI perspective.
 *
 * @param b Pointer to the board.
 * @param player Pointer to the AI player.
 * @return Numeric score: 10 if win, -10 if lose, 0 otherwise.
 */
float XO_inf_AI::evaluate(Board<char>* b, Player<char>* player)
{
    auto* board = dynamic_cast<XO_inf_Board*>(b);
    if (!board || !player) throw invalid_argument("Invalid board or player in evaluate()");

    if (board->is_win(player)) return 10.0f;
    if (board->is_lose(player)) return -10.0f;
    return 0.0f;
}

/**
 * @brief Minimax algorithm with alpha-beta pruning.
 *
 * @param aiTurn True if it's AI's turn.
 * @param player Pointer to the AI player.
 * @param alpha Alpha value for pruning.
 * @param beta Beta value for pruning.
 * @param blankCell Symbol representing an empty cell.
 * @param depth Maximum recursion depth.
 * @return Best minimax score from the current board position.
 */
float XO_inf_AI::minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth)
{
    auto* board = dynamic_cast<XO_inf_Board*>(player->get_board_ptr());
    if (!board || !player) throw invalid_argument("Invalid board or player in minimax()");

    float score = evaluate(board, player);
    if (score == 10 || score == -10 || board->is_draw(player) || depth == 0)
        return score;

    char ai = player->get_symbol();
    char opp = (ai == 'X') ? 'O' : 'X';
    char turn = aiTurn ? ai : opp;
    float best = aiTurn ? -INF : INF;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board->get_cell(i, j) == blankCell)
            {
                Move<char> move(i, j, turn);
                board->update_board(&move);

                float val = minimax(!aiTurn, player, alpha, beta, blankCell, depth - 1);

                move = Move<char>(i, j, 0); // Undo move
                board->update_board(&move);

                if (aiTurn) { best = max(best, val); alpha = max(alpha, val); }
                else        { best = min(best, val); beta  = min(beta, val); }

                if (beta <= alpha) return best;
            }

    return best;
}

/**
 * @brief Determine the best move for AI.
 *
 * @param player Pointer to the AI player.
 * @param blankCell Symbol representing empty cells.
 * @param depth Maximum search depth (default 6).
 * @return Pointer to the optimal Move<char>.
 *
 * @throws runtime_error if no valid moves are available.
 */
Move<char>* XO_inf_AI::bestMove(Player<char>* player, char blankCell, int depth)
{
    auto* board = dynamic_cast<XO_inf_Board*>(player->get_board_ptr());
    if (!board || !player) throw invalid_argument("Invalid board or player in bestMove()");

    char ai = player->get_symbol();
    float bestVal = -INF;
    int bestR = -1, bestC = -1;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board->get_cell(i, j) == blankCell)
            {
                Move<char> move(i, j, ai);
                board->update_board(&move);

                float val = minimax(false, player, -INF, INF, blankCell, depth - 1);

                move = Move<char>(i, j, 0); // Undo move
                board->update_board(&move);

                if (val > bestVal) { bestVal = val; bestR = i; bestC = j; }
            }

    if (bestR == -1 || bestC == -1)
        throw runtime_error("No valid moves left for AI");

    return new Move<char>(bestR, bestC, ai);
}

//--------------------------------------- XO_inf_UI Implementation

/**
 * @brief Constructs the XO_inf UI controller.
 */
XO_inf_UI::XO_inf_UI() : Custom_UI<char>("Infinite XO", 3) {}

/**
 * @brief Create a new player for the game.
 *
 * @param name Player's name.
 * @param symbol Player's symbol (X or O).
 * @param type Player type (HUMAN or COMPUTER).
 * @return Pointer to the created Player<char>.
 */
Player<char>* XO_inf_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Get the next move from a player.
 *
 * @param player Pointer to the active player.
 * @return Pointer to a Move<char> representing the player's move.
 *
 * @details
 * - For HUMAN players, reads coordinates from stdin.
 * - For COMPUTER players, randomly selects an empty cell.
 * - For AI players, uses the XO_inf_AI class to calculate the best move based
 *   on minimax and board evaluation.
 */
/**
 * @brief Get the next move from a player.
 *
 * @param player Pointer to the active player.
 * @return Pointer to a Move<char> representing the player's move.
 *
 * @details
 * - For HUMAN players, reads coordinates from stdin and validates them.
 *   Prompts repeatedly until valid input is given.
 * - For COMPUTER players, randomly selects an empty cell.
 * - For AI players, uses the XO_inf_AI class to calculate the best move based
 *   on minimax and board evaluation.
 */
Move<char>* XO_inf_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        while (true) {
            try {
                cout << "\nPlease enter your move x and y (0 to 2): ";
                if (!(cin >> x >> y)) {
                    cin.clear(); // clear error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
                    throw invalid_argument("Invalid input: not a number");
                }

                // Check bounds
                if (x < 0 || x >= player->get_board_ptr()->get_rows() ||
                    y < 0 || y >= player->get_board_ptr()->get_columns()) {
                    throw out_of_range("Move coordinates out of bounds");
                }

                break; // valid input, exit loop
            }
            catch (const exception& e) {
                cerr << "Error: " << e.what() << "\nPlease try again.\n";
            }
        }

        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::AI) {
        XO_inf_AI ai;
        return ai.bestMove(player, '.', 13);
    }

    throw runtime_error("Unknown player type in get_move()");
}