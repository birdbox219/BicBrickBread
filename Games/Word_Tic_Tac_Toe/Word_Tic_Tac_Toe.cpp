#include "Word_Tic_Tac_Toe.h"
#include <iostream>
#include <random>
#include <cstdint>
#include <set>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <random>

// ============================================================================
// Static Member Initialization
// ============================================================================

/**
 * @brief Dictionary of valid words (forward) for the Word Tic-Tac-Toe board.
 */
std::set<std::string> Word_XO_Board::dict;

/**
 * @brief Dictionary of valid words in reverse for the Word Tic-Tac-Toe board.
 */
std::set<std::string> Word_XO_Board::revDict;

/**
 * @brief Precomputed scoring table for AI heuristics on each board position.
 */
std::vector<std::pair<int, char>> Word_XO_UI::score[3][3];

// ============================================================================
// Word_XO_Board Implementation
// ============================================================================

/**
 * @brief Constructs a new Word_XO_Board object.
 *
 * @details Initializes a 3x3 board with empty cells and loads the dictionary
 * from "dic.txt" if not already loaded. Also stores reversed words for AI use.
 * Throws a runtime exception if the dictionary file is missing.
 */
Word_XO_Board::Word_XO_Board() : Board(3, 3), emptyCell('.')
{ 
    if(dict.empty()) {
        std::ifstream in("dic.txt");
        if (in.fail()) throw std::runtime_error("Can't Find \"dic.txt\"");
        std::string word;
        while(in >> word) {
            dict.insert(word);
            std::reverse(word.begin(), word.end());
            revDict.insert(word);
        }
    }

    for (auto &row : board)
        for (auto &cell : row)
            cell = emptyCell;
}

/**
 * @brief Returns the symbol representing an empty cell.
 * @return The empty cell character ('.').
 */
char Word_XO_Board::getEmptyCell()
{
    return emptyCell;
}

/**
 * @brief Returns the number of moves made so far on the board.
 * @return Integer move count in [0, 9].
 */
int Word_XO_Board::getMoveCount()
{
    return nMoves;
}

/**
 * @brief Checks if any valid word exists on the current board.
 *
 * @details Scans all rows, columns, and diagonals for words in `dict` or `revDict`.
 * @return True if a valid word is found, otherwise false.
 */
bool Word_XO_Board::wordExist()
{
    std::string word;
    bool isFound = false;

    // Horizontal
    for(int r = 0; r < 3; ++r) {
        word.clear();
        word.push_back(board[r][0]);
        word.push_back(board[r][1]);
        word.push_back(board[r][2]);
        isFound |= (dict.find(word) != dict.end());
        isFound |= (revDict.find(word) != revDict.end());
    }

    // Vertical
    for(int c = 0; c < 3; ++c) {
        word.clear();
        word.push_back(board[0][c]);
        word.push_back(board[1][c]);
        word.push_back(board[2][c]);
        isFound |= (dict.find(word) != dict.end());
        isFound |= (revDict.find(word) != revDict.end());
    }

    // Diagonal
    word.clear();
    for(int rc = 0; rc < 3; ++rc)
        word.push_back(board[rc][rc]); 
    isFound |= (dict.find(word) != dict.end());
    isFound |= (revDict.find(word) != revDict.end());

    // Anti-diagonal
    word.clear();
    for(int rc = 0; rc < 3; ++rc)
        word.push_back(board[rc][2 - rc]); 
    isFound |= (dict.find(word) != dict.end());
    isFound |= (revDict.find(word) != revDict.end());

    return isFound;
}

/**
 * @brief Updates the board with a given move.
 *
 * @param move Pointer to the Move object containing row, column, and symbol.
 * @return True if move applied successfully, false if invalid.
 */
bool Word_XO_Board::update_board(Move<char> *move)
{
    size_t r = move->get_x();
    size_t c = move->get_y();
    char sym = move->get_symbol();

    if (r > 2 || c > 2 || (board[r][c] != emptyCell && sym != 0)) return false;

    if (sym == 0) {
        board[r][c] = emptyCell;
        --nMoves;
        return true; 
    }

    board[r][c] = sym;
    ++nMoves;
    return true;
}

/**
 * @brief Records the last player who moved on the board.
 * @param player Pointer to the Player who just played.
 */
void Word_XO_Board::setLastPlayer(Player<char> *player)
{
    lastPlayer = player;
}

/**
 * @brief Determines if the game has reached a terminal state.
 * @param player Pointer to the player for whom the check is made.
 * @return True if game is over, otherwise false.
 */
bool Word_XO_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_lose(player) || is_draw(player);
}

/**
 * @brief Checks if the given player has won.
 * @param player Pointer to player.
 * @return True if the player formed a word and was last to move.
 */
bool Word_XO_Board::is_win(Player<char> *player)
{
    return wordExist() && player == lastPlayer;
}

/**
 * @brief Checks if the given player has lost.
 * @param player Pointer to player.
 * @return True if a word exists but player was not last to move.
 */
bool Word_XO_Board::is_lose(Player<char> *player)
{  
    return wordExist() && player != lastPlayer;
}

/**
 * @brief Checks if the game ended in a draw.
 * @param player Pointer to player.
 * @return True if board is full and no player won.
 */
bool Word_XO_Board::is_draw(Player<char> *player)
{
    return !is_win(player) && !is_lose(player) && nMoves == 9;
}

// ============================================================================
// Word_XO_UI Implementation
// ============================================================================

/**
 * @brief Constructs a new UI object and precomputes AI scores.
 */
Word_XO_UI::Word_XO_UI() : Custom_UI<char>("Word Tic Tac Toe"s, 5)
{
    if (score[0][0].empty()) {
        for(int r = 0; r < 3; ++r) {
            for(int c = 0; c < 3; ++c) {
                score[r][c] = evaluate(r, c);
            }
        }
    }
}

/**
 * @brief Get the next move from a player in Word Tic-Tac-Toe.
 *
 * @param player Pointer to the active player.
 * @return Pointer to a Move<char> representing the player's move.
 *
 * @details
 * - For HUMAN players, repeatedly prompts for row, column, and character input
 *   until valid values are provided.
 * - For COMPUTER players, generates random row, column, and character.
 * - For AI players, uses Word_AI to compute the optimal move.
 */
Move<char> *Word_XO_UI::get_move(Player<char> *player)
{
    int r, c;
    char sym;

    if (player->get_type() == PlayerType::HUMAN) {
        while (true) {
            try {
                cout << player->get_name() << " (" << player->get_symbol() 
                     << ") enter your move (row col): ";
                if (!(cin >> r >> c)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid input: row/col must be numbers");
                }

                cout << "Enter the character: ";
                if (!(cin >> sym)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid input: symbol must be a character");
                }
                sym = toupper(sym);

                // Validate bounds
                if (r < 0 || r >= 3 || c < 0 || c >= 3) {
                    throw out_of_range("Row or column out of bounds (0-2)");
                }

                break; // valid input
            }
            catch (const exception &e) {
                cerr << "Error: " << e.what() << "\nPlease try again.\n";
            }
        }
    } 
    else if (player->get_type() == PlayerType::COMPUTER) {
        r = rand() % 3;
        c = rand() % 3;
        sym = rand() % 26 + 'A';
    } 
    else if (player->get_type() == PlayerType::AI) {
        Word_AI AI;
        dynamic_cast<Word_XO_Board*>(player->get_board_ptr())->setLastPlayer(player);
        return AI.bestMove(player, '.');
    }

    dynamic_cast<Word_XO_Board*>(player->get_board_ptr())->setLastPlayer(player);
    return new Move<char>(r, c, sym);
}

/**
 * @brief Evaluates the potential score of each character for a given cell.
 *
 * @param r Row index of cell.
 * @param c Column index of cell.
 * @return Vector of pairs {score, character} sorted descending.
 */
std::vector<std::pair<int, char>> Word_XO_UI::evaluate(size_t r, size_t c)
{
    std::vector<std::pair<int, char>> ret;
    int posType = ((r == 1 && c == 1) ? 1 : ((r == 0 || r == 2) && (c == 0 || c == 2)) ? 0 : 2);

    for(char ch = 'A'; ch <= 'Z'; ++ch) {
        std::set<char> s1, s2;
        for(auto& word : Word_XO_Board::dict) {
            if(word[posType] != ch) continue;
            int temp = 0;
            for(int k = 0; k < 3; ++k) {
                if(k == posType) continue;
                if(temp == 0) s1.insert(word[k]);
                else s2.insert(word[k]);
                ++temp;
            }
        }
        ret.push_back({s1.size() + s2.size(), ch});
    }
    std::sort(ret.rbegin(), ret.rend());
    return ret;
}

/**
 * @brief Determines the best AI move for a given player using heuristics.
 *
 * @param player Pointer to player.
 * @param blankCell Character representing empty cells.
 * @param depth Depth for minimax (unused in current implementation).
 * @return Pointer to the best Move.
 */
Move<char> *Word_AI::bestMove(Player<char> *player, char blankCell, int depth)
{
    Word_XO_Board* board = dynamic_cast<Word_XO_Board*>(player->get_board_ptr());
    static std::random_device rd;
    static std::mt19937 gen(rd());

    if(board->getMoveCount() == 0) {
        int topCount = std::max(1, (int)(Word_XO_UI::score[1][1].size() * 0.3));
        std::uniform_int_distribution<int> dis(0, topCount - 1);
        return new Move<char> (1, 1, Word_XO_UI::score[1][1][dis(gen)].second);
    }

    std::vector<std::vector<std::pair<int, int>>> lines = {
        {{0,0},{0,1},{0,2}}, {{1,0},{1,1},{1,2}}, {{2,0},{2,1},{2,2}},
        {{0,0},{1,0},{2,0}}, {{0,1},{1,1},{2,1}}, {{0,2},{1,2},{2,2}},
        {{0,0},{1,1},{2,2}}, {{0,2},{1,1},{2,0}}
    };

    auto check_line_for_word = [&](std::vector<std::pair<int,int>> line) -> std::tuple<int,int,char> {
        int emptyR=-1, emptyC=-1, filledCount=0;
        for(const auto& coord : line) {
            if(board->get_cell(coord.first,coord.second)==board->getEmptyCell()) {
                emptyR=coord.first; emptyC=coord.second;
            } else ++filledCount;
        }
        if(filledCount==2) {
            for(char sym='A'; sym<='Z'; ++sym) {
                std::string temp_word;
                for(const auto& coord: line)
                    temp_word.push_back((coord.first==emptyR && coord.second==emptyC)?sym:board->get_cell(coord.first,coord.second));
                if(Word_XO_Board::dict.find(temp_word)!=Word_XO_Board::dict.end() ||
                   Word_XO_Board::revDict.find(temp_word)!=Word_XO_Board::revDict.end())
                    return {emptyR,emptyC,sym};
            }
        }
        return {-1,-1,0};
    };

    std::vector<std::tuple<int,int,char>> winningMoves;
    for(const auto& line: lines) {
        auto m = check_line_for_word(line);
        if(std::get<0>(m)!=-1) winningMoves.push_back(m);
    }
    if(!winningMoves.empty()) {
        std::uniform_int_distribution<int> dis(0, winningMoves.size()-1);
        auto [r,c,ch]=winningMoves[dis(gen)];
        return new Move<char>(r,c,ch);
    }

    std::vector<std::pair<int,int>> emptyCells;
    for(int r=0;r<3;++r)
        for(int c=0;c<3;++c)
            if(board->get_cell(r,c)==board->getEmptyCell())
                emptyCells.push_back({r,c});

    std::vector<std::pair<int,int>> safeMoves;
    for(const auto& cell: emptyCells) {
        int r=cell.first, c=cell.second; bool isSafe=true;
        for(char ch='A'; ch<='Z' && isSafe; ++ch) {
            Move<char> tempMove(r,c,ch); board->update_board(&tempMove);
            for(const auto& line: lines) {
                auto threat = check_line_for_word(line);
                if(std::get<0>(threat)!=-1) { isSafe=false; break; }
            }
            Move<char> undoMove(r,c,0); board->update_board(&undoMove);
        }
        if(isSafe) safeMoves.push_back({r,c});
    }
    if(!safeMoves.empty()) {
        int i = std::rand() % safeMoves.size();
        int r = safeMoves[i].first, c = safeMoves[i].second;
        int topCount = std::max(1,(int)(Word_XO_UI::score[r][c].size()*0.3));
        std::uniform_int_distribution<int> dis(0,topCount-1);
        return new Move<char>(r,c,Word_XO_UI::score[r][c][dis(gen)].second);
    }

    std::vector<std::tuple<int,int,char>> blockMoves;
    for(const auto& [r,c]: emptyCells) {
        for(char ch='A'; ch<='Z'; ++ch) {
            Move<char> tempMove(r,c,ch); board->update_board(&tempMove);
            bool blocksOpponent=true;
            for(const auto& line: lines) {
                auto opp = check_line_for_word(line);
                if(std::get<0>(opp)!=-1) blocksOpponent=false;
            }
            Move<char> undoMove(r,c,0); board->update_board(&undoMove);
            if(blocksOpponent) blockMoves.push_back({r,c,ch});
        }
    }
    if(!blockMoves.empty()) {
        std::uniform_int_distribution<int> dis(0,blockMoves.size()-1);
        int idx = dis(gen);
        return new Move<char>(std::get<0>(blockMoves[idx]), std::get<1>(blockMoves[idx]), std::get<2>(blockMoves[idx]));
    }

    std::uniform_int_distribution<int> dis('A','Z');
    return new Move<char>(emptyCells[0].first, emptyCells[0].second,(char)dis(gen));
}