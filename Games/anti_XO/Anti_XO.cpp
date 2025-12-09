#include "Anti_XO.h"
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;

/**
 * @brief Initializes a 3×3 Anti-XO board with '.' cells.
 */
Anti_XO_Board::Anti_XO_Board():Board(3,3)
{
    board.assign(3, vector<char>(3, '.'));
}

/**
 * @brief Applies a move if the target cell is empty and inside bounds.
 */
bool Anti_XO_Board::update_board(Move<char>* move)
{
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (r < 0 || r >= 3 || c < 0 || c >= 3)
        return false;

    if (board[r][c] != '.')
        return false;

    board[r][c] = s;
    return true;
}

/**
 * @brief Checks if the player has 3 of their symbols in a row, column, or diagonal.
 *        In Anti-XO, this means losing the game.
 */
bool Anti_XO_Board::is_lose(Player<char>* player)
{
    char sym = player->get_symbol();

    // Check all rows
    for (int r = 0; r < 3; r++)
        if (board[r][0] == sym && board[r][1] == sym && board[r][2] == sym)
            return true;

    // Check all columns
    for (int c = 0; c < 3; c++)
        if (board[0][c] == sym && board[1][c] == sym && board[2][c] == sym)
            return true;

    // Main diagonal
    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
        return true;

    // Anti-diagonal
    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;

    return false;
}

/**
 * @brief Draw occurs when board is full and no losing line exists.
 */
bool Anti_XO_Board::is_draw(Player<char>* player)
{
    if (is_lose(player)) 
        return false;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == '.')
                return false;

    return true;
}

/**
 * @brief Game ends only when the player loses or board draws.
 */
bool Anti_XO_Board::game_is_over(Player<char>* player)
{
    return is_lose(player) || is_draw(player);
}

/**
 * @brief Creates Anti-XO UI with board size 3.
 */
Anti_XO_UI::Anti_XO_UI() 
    : Custom_UI<char>("anti_XO",3) {}

/**
 * @brief Creates a new player instance.
 */
Player<char>* Anti_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets the move based on player type:
 *        - HUMAN → console input
 *        - COMPUTER → random move
 *        - AI → Anti_AI bestMove
 */
Move<char>* Anti_XO_UI::get_move(Player<char>* player)
{
    int r, c;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
    } 
    else if (player->get_type() == PlayerType::COMPUTER) {
        r = rand()%3;
        c = rand()%3;
    } 
    else if (player->get_type() == PlayerType::AI) {
        Anti_AI ai;
        return ai.bestMove(player, '.');
    }

    return new Move<char>(r, c, player->get_symbol());
}

/**
 * @brief Checks whether coordinates fall inside the 3×3 board.
 */
bool Anti_AI::bounded(int x, int y)
{
    return (x>0 && x<3 && y>0 && y<3);
}

/**
 * @brief Selects the move with minimal “danger” by checking adjacency to same-symbol cells.
 *        Adds slight randomness to avoid deterministic behavior.
 */
Move<char> *Anti_AI::bestMove(Player<char> *player, char blankCell, int depth)
{
    auto* board = player->get_board_ptr();
    char s = player->get_symbol();

    vector<pair<int, pair<int,int>>> scores;

    // Directions around the cell
    int dx[8] = { -1,-1, 0, 1, 1, 1, 0,-1 };
    int dy[8] = {  0, 1, 1, 1, 0,-1,-1,-1 };

    // Evaluate each empty position
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (board->get_cell(x, y) != '.') continue;

            int danger = 0;

            for (int d = 0; d < 8; d++) {
                int nx = x + dx[d];
                int ny = y + dy[d];

                if (!bounded(nx, ny)) continue;

                if (board->get_cell(nx, ny) == s)
                    danger++;
            }

            scores.push_back({ danger, {x, y} });
        }
    }

    if (scores.empty())
        throw std::runtime_error("Empty Move");

    // Sort by danger ascending
    sort(scores.begin(), scores.end(),
         [](auto &a, auto &b){ return a.first < b.first; });

    // Pick randomly among top K lowest danger moves
    const int K = 2;  
    int maxChoices = min(K, (int)scores.size());
    int idx = rand() % maxChoices;

    auto &choice = scores[idx];
    return new Move(choice.second.first, choice.second.second, s);
}