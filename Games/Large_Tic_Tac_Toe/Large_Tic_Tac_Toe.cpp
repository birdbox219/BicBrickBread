#include "Large_Tic_Tac_Toe.h"

using namespace std;

// ============================================================================
// Activation Functions
// ============================================================================
auto relu              = [](double x) { return x > 0.0 ? x : 0.0; };
auto relu_derivative   = [](double x) { return x > 0.0 ? 1.0 : 0.0; };
auto linear            = [](double x) { return x; };
auto linear_derivative = [](double x) { return 1.0; };

// ============================================================================
// Large_XO_Board Implementation
// ============================================================================

uint32_t Large_XO_Board::win3Masks[48] = {0};

// ----------------------------- Constructor -----------------------------
Large_XO_Board::Large_XO_Board()
    : Board(1, 1), emptyCell('.') 
{
    board.assign(5, vector<char>(5, emptyCell));

    if (win3Masks[0] == 0) {
        size_t maskIdx = 0;

        // Horizontal patterns
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c <= 2; ++c)
                win3Masks[maskIdx++] = (1u << (5 * r + c)) | (1u << (5 * r + c + 1)) | (1u << (5 * r + c + 2));

        // Vertical patterns
        for (int c = 0; c < 5; ++c)
            for (int r = 0; r <= 2; ++r)
                win3Masks[maskIdx++] = (1u << (5 * r + c)) | (1u << (5 * (r + 1) + c)) | (1u << (5 * (r + 2) + c));

        // Diagonal top-left to bottom-right
        for (int r = 0; r <= 2; ++r)
            for (int c = 0; c <= 2; ++c)
                win3Masks[maskIdx++] = (1u << (5 * r + c)) | (1u << (5 * (r + 1) + c + 1)) | (1u << (5 * (r + 2) + c + 2));

        // Diagonal top-right to bottom-left
        for (int r = 0; r <= 2; ++r)
            for (int c = 2; c < 5; ++c)
                win3Masks[maskIdx++] = (1u << (5 * r + c)) | (1u << (5 * (r + 1) + c - 1)) | (1u << (5 * (r + 2) + c - 2));
    }
}

// ----------------------------- Accessors -----------------------------
char Large_XO_Board::getCell(size_t r, size_t c) {
    size_t idx = 5 * r + c;
    if (boardX & (1u << idx)) return 'X';
    if (boardO & (1u << idx)) return 'O';
    return emptyCell;
}

char Large_XO_Board::getEmptyCell() { return emptyCell; }
int  Large_XO_Board::getMoveCount() { return nMoves; }

// ----------------------------- Mutators -----------------------------
bool Large_XO_Board::updateCell(size_t r, size_t c, char s) {
    if (r >= 5 || c >= 5) return false;
    size_t idx = 5 * r + c;

    if ((boardXO & (1u << idx)) && s != 0) return false;

    uint32_t mask = ~(1u << idx);
    if (s == 0) {
        boardX &= mask;
        boardO &= mask;
        boardXO &= mask;
        --nMoves;
        return true;
    }

    if (s == 'X') boardX |= (1u << idx);
    else boardO |= (1u << idx);

    boardXO |= (1u << idx);
    ++nMoves;
    return true;
}

bool Large_XO_Board::update_board(Move<char>* move) {
    return updateCell(move->get_x(), move->get_y(), move->get_symbol());
}

// ----------------------------- Game Logic -----------------------------
bool Large_XO_Board::game_is_over(Player<char>*) { return nMoves >= 24; }

bool Large_XO_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    char opp = (sym == 'X') ? 'O' : 'X';
    return countWin(sym) > countWin(opp) && game_is_over(nullptr);
}

bool Large_XO_Board::is_lose(Player<char>* player) {
    char sym = player->get_symbol();
    char opp = (sym == 'X') ? 'O' : 'X';
    return countWin(sym) < countWin(opp) && game_is_over(nullptr);
}

bool Large_XO_Board::is_draw(Player<char>* player) {
    char sym = player->get_symbol();
    char opp = (sym == 'X') ? 'O' : 'X';
    return countWin(sym) == countWin(opp) && game_is_over(nullptr);
}

// ----------------------------- Helpers -----------------------------
float Large_XO_Board::countWin(char sym) {
    uint32_t& temp = (sym == 'X') ? boardX : boardO;
    float score = 0;
    for (auto mask : win3Masks)
        score += ((temp & mask) == mask);
    return score;
}

void Large_XO_Board::encode(char ai, Matrix<double>& input) {
    input.resize(25, 1);
    char opp = (ai == 'X') ? 'O' : 'X';

    for (size_t idx = 0; idx < 25; ++idx) {
        char cell = getCell(idx / 5, idx % 5);
        input(idx, 0) = (cell == ai) ? 1.0 : (cell == opp ? -1.0 : 0.0);
    }
}

// ============================================================================
// Large_XO_UI Implementation
// ============================================================================

Large_XO_UI::Large_XO_UI()
    : Custom_UI<char>("5x5 XO", 5) {}

// ----------------------------- Get Move -----------------------------
Move<char>* Large_XO_UI::get_move(Player<char>* player) {
    if (!board)
        board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());

    int r = -1, c = -1;

    if (player->get_type() == PlayerType::HUMAN) {
        while (true) {
            try {
                cout << player->get_name() << " (" << player->get_symbol()
                     << ") enter your move (row col): ";
                if (!(cin >> r >> c)) throw runtime_error("Invalid input type");
                if (r < 0 || r > 4 || c < 0 || c > 4) throw out_of_range("Row and column must be 0-4");
                if (board->getCell(r, c) != board->getEmptyCell()) throw runtime_error("Cell is not empty");
                break;
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << ". Try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        r = rand() % 5;
        c = rand() % 5;
        while (board->getCell(r, c) != board->getEmptyCell()) {
            r = rand() % 5;
            c = rand() % 5;
        }
    }
    else if (player->get_type() == PlayerType::AI) {
        int movesMade = board->getMoveCount();
        int depth = (movesMade <= 4) ? 3 : (movesMade <= 16 ? 4 : 5);
        return AI.bestMove(player, '.', depth);
    }

    return new Move<char>(r, c, player->get_symbol());
}

// ----------------------------- Display Board -----------------------------
void Large_XO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    cout << "\n    ";
    for (int j = 0; j < 5; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * 5, '-') << "\n";

    for (int i = 0; i < 5; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < 5; ++j)
            cout << setw(cell_width) << (board ? board->getCell(i, j) : '.') << " |";
        cout << "\n   " << string((cell_width + 2) * 5, '-') << "\n";
    }
    cout << endl;
}

// ============================================================================
// Large_XO_AI Implementation
// ============================================================================

Large_XO_AI::Large_XO_AI() {
    srand((unsigned)time(nullptr));

    vector<int> layers = {25, 512, 512, 512, 25};
    vector<function<double(double)>> acts = {relu, relu, relu, linear};
    vector<function<double(double)>> derivs = {relu_derivative, relu_derivative, relu_derivative, linear_derivative};

    try {
        NNX = make_shared<NeuralNetwork>(layers, acts, derivs);
        NNX->load("netX.bin");
        NNO = make_shared<NeuralNetwork>(layers, acts, derivs);
        NNO->load("netO.bin");
    }
    catch (const exception& e) {
        cerr << "Error initializing AI networks: " << e.what() << "\n";
    }
}

// ----------------------------- Evaluate Board -----------------------------
float Large_XO_AI::evaluate(Board<char>* b, Player<char>* player) {
    auto* board = dynamic_cast<Large_XO_Board*>(b);
    char ai = player->get_symbol();
    char opp = (ai == 'X') ? 'O' : 'X';

    if (board->game_is_over(nullptr)) {
        float aiScore  = board->countWin(ai);
        float oppScore = board->countWin(opp);
        if (aiScore > oppScore) return 10000.0f + (aiScore - oppScore);
        if (oppScore > aiScore) return -10000.0f + (aiScore - oppScore);
        return 0.0f;
    }

    Matrix<double> input;
    board->encode(ai, input);
    Matrix<double> out = NN->predict(input);

    double bestQ = -1e9;
    for (int idx = 0; idx < 25; ++idx)
        if (board->getCell(idx / 5, idx % 5) == board->getEmptyCell())
            bestQ = max(bestQ, out(idx, 0));

    return static_cast<float>(bestQ);
}

// ----------------------------- Minimax Algorithm -----------------------------
float Large_XO_AI::minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) {
    auto* board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());
    char ai = player->get_symbol();
    char opp = (ai == 'X') ? 'O' : 'X';
    char turn = aiTurn ? ai : opp;

    if (depth == 0 || board->game_is_over(nullptr))
        return evaluate(board, player);

    vector<pair<double,int>> moves;

    // Predict scores for deeper search
    if (depth >= 2) {
        Matrix<double> input;
        board->encode(turn, input);
        Matrix<double> out = NN->predict(input);

        for (int i = 0; i < 25; ++i)
            if (board->getCell(i / 5, i % 5) == blankCell)
                moves.push_back({out(i, 0), i});

        sort(moves.begin(), moves.end(),
             aiTurn ? [](std::pair<double, int>& a, std::pair<double, int>& b){ return a.first > b.first; }
                    : [](std::pair<double, int>& a, std::pair<double, int>& b){ return a.first < b.first; });
    }
    else {
        for (int i = 0; i < 25; ++i)
            if (board->getCell(i / 5, i % 5) == blankCell)
                moves.push_back({0, i});
    }

    float best = aiTurn ? -1e9f : 1e9f;

    for (auto& mv : moves) {
        int r = mv.second / 5;
        int c = mv.second % 5;
        board->updateCell(r, c, turn);
        float score = minimax(!aiTurn, player, alpha, beta, blankCell, depth - 1);
        board->updateCell(r, c, 0);

        if (aiTurn) { best = max(best, score); alpha = max(alpha, score); }
        else        { best = min(best, score); beta  = min(beta, score); }

        if (beta <= alpha) break;  // Alpha-beta pruning
    }

    return best;
}

// ----------------------------- Best Move -----------------------------
Move<char>* Large_XO_AI::bestMove(Player<char>* player, char blankCell, int depth) {
    auto* board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());
    char ai = player->get_symbol();
    NN = (ai == 'X') ? NNX : NNO;

    float bestVal = -1e9f;
    int bestR = -1, bestC = -1;

    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c) {
            if (board->getCell(r, c) != blankCell) continue;
            board->updateCell(r, c, ai);
            float val = minimax(false, player, -1e9f, 1e9f, blankCell, depth - 1);
            board->updateCell(r, c, 0);
            if (val > bestVal) { bestVal = val; bestR = r; bestC = c; }
        }

    return new Move<char>(bestR, bestC, ai);
}