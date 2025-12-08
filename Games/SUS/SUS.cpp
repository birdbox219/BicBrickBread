#include "SUS.h"
#include <iostream>

SUS_Board::SUS_Board() : Board(3, 3)
{
    // Initialize all cells with blank_symbol
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}
void SUS_Board::score(int x, int y, char sym){
    // Check if there is S-U-S  in the row of the last move
    if (board[x][0]=='S'&&board[x][1]=='U'&&board[x][2]=='S'){
        sym == 'S' ? s_score++ : u_score++;
    }
     // Check if there is S-U-S in the column of the last move
    if(board[0][y]=='S'&&board[1][y]=='U'&&board[2][y]=='S'){
        sym == 'S' ? s_score++ : u_score++;
    }
     // Check if there is S-U-S in the diagonal + last move is in the diagonal
    if (board[0][0]=='S'&& board[1][1]=='U'&& board[2][2]=='S' &&(x==y) ){
        sym == 'S' ? s_score++ : u_score++;
    }
    if (board[0][2]=='S'&& board[1][1]=='U' &&board[2][0]=='S' &&(x+y==2)){
        sym == 'S' ? s_score++ : u_score++;
    }

       
 }
bool SUS_Board::update_board(Move<char> *move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
     if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0))
    {

        if (mark == 0)
        { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else
        { // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }

        return true;
    }
    return false;
}
bool SUS_Board::is_win(Player<char> *player)
{
     const char sym = player->get_symbol();
     if (n_moves==9 && s_score>u_score && sym=='S' ){
        return true;

     }
   
    return false;
}
bool SUS_Board::is_lose(Player<char> *player)
{
     const char sym = player->get_symbol();
   
    if (n_moves==9 && s_score<u_score && sym=='S' ){
        return true;
     }
    return false;
}

bool SUS_Board::is_draw(Player<char> *player)
{
    return (n_moves == 9 &&  s_score == u_score );
}

bool SUS_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player);
}

//===============================UI========================================
SUS_UI::SUS_UI() : Custom_UI<char>("SUS", 3) {}

Player<char> **SUS_UI::setup_players()
{
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer", "AI", "Random"};

    string nameX = get_player_name("Player S");
    PlayerType typeX = get_player_type_choice("Player S", type_options);
    players[0] = create_player(nameX, static_cast<char>('S'), typeX);

    string nameO = get_player_name("Player U");
    PlayerType typeO = get_player_type_choice("Player U", type_options);
    players[1] = create_player(nameO, static_cast<char>('U'), typeO);

    return players;
}

Player<char> *SUS_UI::create_player(string &name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char> *SUS_UI::get_move(Player<char> *player)
{ 
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
       
        cout << player->get_name() << " (" << player->get_symbol() << ") "
         << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
        
    }
    else if(player->get_type() == PlayerType::COMPUTER) {
        x = rand()%3;
        y = rand()%3;
    }
    else if (player->get_type() == PlayerType::AI) {
        SUS_AI ai;
        return ai.bestMove(player, '.', 9);
    } 
    return new Move<char>(x, y, player->get_symbol());
}

float SUS_AI::evaluate(Board<char>* board, Player<char>* player)
{
    char AI = player->get_symbol();
    int score = 0;
    
    // Check all rows for SUS
    for (int i = 0; i < 3; i++) {
        if (board->get_cell(i, 0) == 'S' && board->get_cell(i, 1) == 'U' && board->get_cell(i, 2) == 'S') {
            char owner = board->get_cell(i, 1);
            score += (owner == AI) ? 10 : -10;
        }
    }
    
    // Check all columns for SUS
    for (int j = 0; j < 3; j++) {
        if (board->get_cell(0, j) == 'S' && board->get_cell(1, j) == 'U' && board->get_cell(2, j) == 'S') {
            char owner = board->get_cell(1, j);
            score += (owner == AI) ? 10 : -10;
        }
    }
    
    // Check main diagonal
    if (board->get_cell(0, 0) == 'S' && board->get_cell(1, 1) == 'U' && board->get_cell(2, 2) == 'S') {
        char owner = board->get_cell(1, 1);
        score += (owner == AI) ? 10 : -10;
    }
    
    // Check anti-diagonal
    if (board->get_cell(0, 2) == 'S' && board->get_cell(1, 1) == 'U' && board->get_cell(2, 0) == 'S') {
        char owner = board->get_cell(1, 1);
        score += (owner == AI) ? 10 : -10;
    }
    
    return score;
}

bool SUS_AI::isMovesLeft(Board<char>* board)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board->get_cell(i, j) == '.') return true;
    return false;
}

float SUS_AI::minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth)
{
    auto* board = player->get_board_ptr();
    float score = evaluate(board, player);
    
    if (score >= 10) return score - depth;
    if (score <= -10) return score + depth;
    if (!isMovesLeft(board)) return 0;

    float best = aiTurn ? INT_MIN : INT_MAX;
    char symbol = aiTurn ? player->get_symbol() : (player->get_symbol() == 'X' ? 'O' : 'X');
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->get_cell(i, j) == '.') {
                Move<char> move(i, j, symbol);
                board->update_board(&move);
                float val = minimax(!aiTurn, player, alpha, beta, blankCell, depth + 1);
                best = aiTurn ? max(best, val) : min(best, val);
                Move<char> undo(i, j, 0);
                board->update_board(&undo);
                
                if (aiTurn) alpha = max(alpha, best);
                else beta = min(beta, best);
                if (beta <= alpha) break;
            }
        }
        if (beta <= alpha) break;
    }
    return best;
}

Move<char>* SUS_AI::bestMove(Player<char>* player, char blankCell, int depth)
{
    auto* board = player->get_board_ptr();
    char AI = player->get_symbol();
    float bestVal = INT_MIN;
    int bestRow = -1, bestCol = -1;
    float alpha = INT_MIN, beta = INT_MAX;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->get_cell(i, j) == '.') {
                Move<char> move(i, j, AI);
                board->update_board(&move);
                float moveVal = minimax(false, player, alpha, beta, blankCell, 1);
                Move<char> undo(i, j, 0);
                board->update_board(&undo);
                
                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestRow = i;
                    bestCol = j;
                }
                alpha = max(alpha, bestVal);
            }
        }
    }

    return new Move<char>(bestRow, bestCol, AI);
}