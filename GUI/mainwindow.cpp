#include "mainwindow.h"
#include <QFormLayout>
#include <QMessageBox>

// Include Game Headers
#include "../Games/XO_inf/XO_inf.h"
#include "../Games/diamond_XO/dia_XO.h" 
#include "../Games/Four_in_a_row/four.h" 
#include "../Games/SUS/SUS.h"
#include "../Games/XO_num/xo_num.h" 
#include "../Games/Ultimate_Tic_Tac_Toe/Ultimate.h" 
#include "../Games/anti_XO/Anti_XO.h" 
#include "../Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h"
#include "../Games/PyramidXO/PyramidXO.h"
#include "../header/XO_Classes.h"
#include "../Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.h"
#include "../Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.h"
#include "../Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.h"
#include "../Games/4x4_Tic_Tac_Toe/4by4_XO.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *title = new QLabel("Select a Game to Play", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 20px;");
    layout->addWidget(title);

    // Game Selector
    gameSelector = new QComboBox(this);
    games = {
        {1, "Classic XO"},
        {2, "Infinite XO (XO_inf)"},
        {3, "Four in a Row"},
        {4, "Anti XO"},
        {5, "5x5 XO (Large Tic-Tac-Toe)"},
        {6, "Numerical Tic-Tac-Toe"},
        {7, "Ultimate Tic-Tac-Toe"},
        {8, "Pyramid XO"},
        {9, "Word XO"},
        {10, "Obstacles Tic-Tac-Toe"},
        {11, "Memory Tic-Tac-Toe"},
        {12, "4x4 Tic-Tac-Toe"},
        {13, "SUS"},
        {14, "Diamond XO"}
    };

    for(const auto &g : games) {
        gameSelector->addItem(g.name, g.id);
    }
    layout->addWidget(new QLabel("Game:", this));
    layout->addWidget(gameSelector);

    // Player 1 Setup
    QGroupBox *p1Group = new QGroupBox("Player 1", this);
    QFormLayout *p1Layout = new QFormLayout;
    player1Name = new QLineEdit("Player 1");
    player1Type = new QComboBox;
    player1Type->addItem("Human", QVariant::fromValue(PlayerType::HUMAN));
    player1Type->addItem("Computer (Random)", QVariant::fromValue(PlayerType::COMPUTER));
    player1Type->addItem("Smart AI", QVariant::fromValue(PlayerType::AI));
    p1Layout->addRow("Name:", player1Name);
    p1Layout->addRow("Type:", player1Type);
    p1Group->setLayout(p1Layout);
    layout->addWidget(p1Group);

    // Player 2 Setup
    QGroupBox *p2Group = new QGroupBox("Player 2", this);
    QFormLayout *p2Layout = new QFormLayout;
    player2Name = new QLineEdit("Player 2");
    player2Type = new QComboBox;
    player2Type->addItem("Human", QVariant::fromValue(PlayerType::HUMAN));
    player2Type->addItem("Computer (Random)", QVariant::fromValue(PlayerType::COMPUTER));
    player2Type->addItem("Smart AI", QVariant::fromValue(PlayerType::AI));
    p2Layout->addRow("Name:", player2Name);
    p2Layout->addRow("Type:", player2Type);
    p2Group->setLayout(p2Layout);
    layout->addWidget(p2Group);

    // Start Button
    QPushButton *startBtn = new QPushButton("Start Game", this);
    startBtn->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 10px;");
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    layout->addWidget(startBtn);

    layout->addStretch();
    setCentralWidget(centralWidget);
}

void MainWindow::onStartGameClicked() {
    int gameId = gameSelector->currentData().toInt();
    
    Board<char>* board = nullptr;

    try {
        switch (gameId) {
            case 1: board = new X_O_Board(); break;
            case 2: board = new XO_inf_Board(); break;
            case 3: board = new FOUR_Board(); break;
            case 4: board = new Anti_XO_Board(); break;
            case 5: board = new Large_XO_Board(); break;
            case 6: board = new XO_NUM_Board(); break;
            case 7: board = new Ultimate_Board(); break;
            case 8: board = new PyramidXO_Board(); break;
            case 9: board = new Word_XO_Board(); break;
            case 10: board = new Obstacles_Board(); break;
            case 11: board = new Memory_Board(); break;
            case 12: board = new _4by4XO_Board(); break;
            case 13: board = new SUS_Board(); break;
            case 14: board = new dia_XO_Board(); break;
            default: throw std::runtime_error("Unknown Game ID");
        }

        // Create Players
        Player<char>** players = new Player<char>*[2];
        
        string name1 = player1Name->text().toStdString();
        PlayerType type1 = player1Type->currentData().value<PlayerType>();

        string name2 = player2Name->text().toStdString();
        PlayerType type2 = player2Type->currentData().value<PlayerType>();

        // Default Symbols
        char p1Sym = 'X';
        char p2Sym = 'O';

        if (gameId == 13) { // SUS
            p1Sym = 'S';
            p2Sym = 'U';
        }

        players[0] = new Player<char>(name1, p1Sym, type1);
        players[1] = new Player<char>(name2, p2Sym, type2);

        // Assign board to players
        players[0]->set_board_ptr(board);
        players[1]->set_board_ptr(board);

         // Handle special symbols/logic if necessary (e.g. XO_NUM uses numbers, not X/O)
         // But the Player constructor takes the symbol. XO_NUM implementation might ignore it or use it.
         // In XO_NUM_Board header: "Players place numbers instead of X/O".
         // The logic in get_move usually prompts for the number. This is a challenge for generic GUI.

        GameWindow *gameWin = new GameWindow(board, players, gameId);
        gameWin->show();
        // this->hide(); // Optional: Hide menu
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
