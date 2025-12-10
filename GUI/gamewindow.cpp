#include "gamewindow.h"
#include <QInputDialog>
#include <QVariant>

// AI Headers
#include "../header/AI.h"
#include "../header/XO_Classes.h" // XO_AI
#include "../Games/XO_inf/XO_inf.h"
#include "../Games/diamond_XO/dia_XO.h" 
#include "../Games/Four_in_a_row/four.h" 
#include "../Games/SUS/SUS.h"
#include "../Games/XO_num/xo_num.h" 
#include "../Games/Ultimate_Tic_Tac_Toe/Ultimate.h" 
#include "../Games/anti_XO/Anti_XO.h" 
#include "../Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h"
#include "../Games/PyramidXO/PyramidXO.h"
#include "../Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.h"
#include "../Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.h"
#include "../Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.h"
#include "../Games/4x4_Tic_Tac_Toe/4by4_XO.h"

GameWindow::GameWindow(Board<char>* b, Player<char>** p, int id, QWidget *parent)
    : QWidget(parent), board(b), gameId(id), currentPlayerIndex(0), selectedCell(-1, -1) {
    
    players[0] = p[0];
    players[1] = p[1];
    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    setWindowTitle(QString::fromStdString("Game: " + players[0]->get_name() + " vs " + players[1]->get_name()));
    resize(600, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    statusLabel = new QLabel(QString::fromStdString("Current Turn: " + players[0]->get_name()));
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(statusLabel);

    boardLayout = new QGridLayout();
    boardLayout->setSpacing(5);
    mainLayout->addLayout(boardLayout);

    // Initial Render
    updateUI();

    // Timer for AI interaction to avoid freezing UI
    computerTimer = new QTimer(this);
    computerTimer->setSingleShot(true);
    connect(computerTimer, &QTimer::timeout, this, &GameWindow::processComputerTurn);

    // If first player is computer, trigger
    if (players[0]->get_type() == PlayerType::COMPUTER) {
        computerTimer->start(500);
    }
}

GameWindow::~GameWindow() {
    delete board;
    delete players[0];
    delete players[1];
    // GameWindow does not own the array pointer itself, or it was passed as double pointer.
    // If MainWindow allocated `new Player*[2]`, we might leak `p`, but `players[2]` is member.
    // We do NOT delete[] players here because `players` is a fixed array member.
    // Qt deletes child widgets automatically
}

void GameWindow::updateUI() {
    vector<vector<char>> matrix = board->get_board_matrix();
    int rows = matrix.size();
    int cols = (rows > 0) ? matrix[0].size() : 0;

    if (cells.isEmpty()) {
        // Create Buttons
        for(int r=0; r<rows; ++r) {
            for(int c=0; c<cols; ++c) {
                QPushButton *btn = new QPushButton(this);
                btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                btn->setProperty("row", r);
                btn->setProperty("col", c);
                
                // Base Styling
                QString style = "font-size: 20px; font-weight: bold; background-color: #f0f0f0; border: 1px solid #999;";
                
                // Special Handling for Ultimate (ID 7): Thicker borders between 3x3 grids
                if (gameId == 7) {
                    if (c % 3 == 0 && c > 0) style += "border-left: 2px solid #000;";
                    if (r % 3 == 0 && r > 0) style += "border-top: 2px solid #000;";
                }
                
                btn->setProperty("baseStyle", style);
                btn->setStyleSheet(style);
                btn->setMinimumSize(40, 40);

                connect(btn, &QPushButton::clicked, this, &GameWindow::onCellClicked);
                boardLayout->addWidget(btn, r, c);
                cells.append(btn);

                // Special Handling for Pyramid (ID 8): Hide invalid cells
                if (gameId == 8) {
                    bool valid = false;
                    if (r == 0 && c == 2) valid = true;
                    else if (r == 1 && (c >= 1 && c <= 3)) valid = true;
                    else if (r == 2 && (c >= 0 && c <= 4)) valid = true;
                    
                    if (!valid) {
                        btn->setVisible(false);
                        btn->setEnabled(false);
                    }
                }
            }
        }
    }

    // Update Text
    for(int r=0; r<rows; ++r) {
        for(int c=0; c<cols; ++c) {
            int index = r * cols + c;
            if(index < cells.size()) {
                char symbol = matrix[r][c];
                QString text = (symbol == '.') ? QString() : QString(QChar(symbol));
                if (symbol == 0) text = ""; 
                
                cells[index]->setText(text);
                
                // Restore base style
                QString style = cells[index]->property("baseStyle").toString();
                
                // Colorize symbols
                if (text == "X") style += "color: blue;";
                else if (text == "O") style += "color: red;";
                else if (text == "S") style += "color: blue;";
                else if (text == "U") style += "color: red;";
                if (text.length() == 1 && text[0].isDigit()) style += "color: darkgreen;";
                
                // Highlight selected cell for 4x4
                if (gameId == 12 && selectedCell.x() == r && selectedCell.y() == c) {
                     style += "border: 3px solid green;";
                }
                
                cells[index]->setStyleSheet(style);
            }
        }
    }
}

void GameWindow::onCellClicked() {
    if (players[currentPlayerIndex]->get_type() == PlayerType::COMPUTER) return;

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int r = btn->property("row").toInt();
    int c = btn->property("col").toInt();

    handleTurn(r, c);
}

void GameWindow::handleTurn(int r, int c) {
    Player<char>* p = players[currentPlayerIndex];
    char symbol = p->get_symbol();

    // Special Handling for 4x4 Tic-Tac-Toe (Game ID 12)
    if (gameId == 12) {
        // Phase 1: Selection
        if (selectedCell.x() == -1) {
             // Try to select if own piece
             if (board->get_cell(r, c) == symbol) {
                 selectedCell = QPoint(r, c);
                 updateUI();
             }
             return; 
        }

        // Phase 2: Move or Reselect
        if (selectedCell.x() == r && selectedCell.y() == c) {
            // Deselect if clicked same cell
            selectedCell = QPoint(-1, -1);
            updateUI();
            return;
        }

        if (board->get_cell(r, c) == symbol) {
            // Change selection to new piece
            selectedCell = QPoint(r, c);
            updateUI();
            return;
        }

        // Attempt Move
        _4by4XO_Move move(selectedCell.x(), selectedCell.y(), r, c, symbol);
        if (board->update_board(&move)) {
             selectedCell = QPoint(-1, -1);
             updateUI();
             if (checkGameEnd()) return;
             switchTurn();
        } else {
             QMessageBox::warning(this, "Invalid Move", "You can only move your token to an adjacent empty cell.");
        }
        return;
    }

    // Special Handling for Games needing Input
    if (gameId == 6) { // XO_NUM
        bool ok;
        int num = QInputDialog::getInt(this, "Enter Number", "Number:", 1, 1, 9, 1, &ok);
        if(!ok) return;
         // XO_NUM logic uses char for numbers
        symbol = (char)(num + '0');
    }
    else if (gameId == 9) { // Word XO
        bool ok;
        QString text = QInputDialog::getText(this, "Enter Letter", "Letter:", QLineEdit::Normal, "", &ok);
        if(!ok || text.isEmpty()) return;
        symbol = text.at(0).toLatin1();
    }

    Move<char> move(r, c, symbol);
    if (board->update_board(&move)) {
        updateUI();
        if (checkGameEnd()) return;
        switchTurn();
    } else {
        QMessageBox::warning(this, "Invalid Move", "That move is not allowed.");
    }
}

bool GameWindow::checkGameEnd() {
    Player<char>* p = players[currentPlayerIndex];
    if (board->is_win(p)) {
        QMessageBox::information(this, "Game Over", QString::fromStdString(p->get_name() + " Wins!"));
        close();
        return true;
    }
    if (board->is_lose(p)) {
        QMessageBox::information(this, "Game Over", QString::fromStdString(players[1-currentPlayerIndex]->get_name() + " Wins!"));
        close();
        return true;
    }
    if (board->is_draw(p)) {
        QMessageBox::information(this, "Game Over", "It's a Draw!");
        close();
        return true;
    }
    return false;
}

void GameWindow::switchTurn() {
    currentPlayerIndex = 1 - currentPlayerIndex;
    statusLabel->setText(QString::fromStdString("Current Turn: " + players[currentPlayerIndex]->get_name()));

    if (players[currentPlayerIndex]->get_type() == PlayerType::COMPUTER) {
        computerTimer->start(500); // Trigger AI after delay
    }
}

void GameWindow::processComputerTurn() {
    performAIMove();
    updateUI();
    if (!checkGameEnd()) {
        switchTurn();
    }
}

void GameWindow::performAIMove() {
    // Create AI Instance based on GameID
    // Since AI classes are stateless usually, we can instantiate locally.
    // Or we should have stored a pointer to AI in the class if it has state.
    // Checking headers: Most AIs seem stateless (evaluate/minimax/bestMove).
    // EXCEPT: Some AIs might need the Board state which they get via function args.
    
    Player<char>* p = players[currentPlayerIndex];
    Move<char>* move = nullptr;
    
    // Default blank symbol
    char blank = '.'; 
    if (gameId == 6) blank = 0; // XO_NUM might use different blank or logic
    // Actually XO_NUM_AI bestMove takes blankCell.
    
    AI* ai = nullptr;

    // Use raw pointers for simplicity, delete after use
    switch(gameId) {
        case 1: ai = new X_O_AI(); break;
        case 2: ai = new XO_inf_AI(); break;
        case 3: ai = new Four_AI(); break;
        case 4: ai = new Anti_AI(); break;
        case 5: ai = new Large_XO_AI(); break;
        case 6: ai = new XO_NUM_AI(); break;
        case 7: ai = new Ultimate_AI(); break;
        case 8: ai = new PyramidXO_AI(); break;
        case 9: ai = new Word_AI(); break;
        // case 10: Obstacles
        case 10: ai = new Obstacles_AI(); break; 
        case 11: ai = new Memory_AI(); break;
        case 12: ai = new _4by4XO_AI(); break;
        case 13: ai = new SUS_AI(); break;
        case 14: ai = new dia_XO_AI(); break;
        default: break;
    }

    if (ai) {
       move = ai->bestMove(p, blank);
       delete ai;
    } else {
        // Fallback for games without AI class (like Word XO potentially) or if simple random is needed
        // Or specific handling.
        // For now, if no AI, we do nothing or simple random move if we can access available moves.
        // But Board doesn't expose `getAvailableMoves` in base class! It's specific.
        // So we strictly need the AI class or a Cast to specific board.
    }

    if (move) {
        board->update_board(move);
        delete move;
    }
}
