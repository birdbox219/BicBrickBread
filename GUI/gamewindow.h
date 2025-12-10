#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <QPoint>
#include "../header/BoardGame_Classes.h"
#include "../header/XO_Classes.h"
// Include other headers as necessary for factory

class GameWindow : public QWidget {
    Q_OBJECT

public:
    // Takes ownership of board and players
    GameWindow(Board<char>* board, Player<char>** players, int gameId, QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void onCellClicked();
    void processComputerTurn();

private:
    Board<char>* board;
    Player<char>* players[2];
    int currentPlayerIndex; // 0 or 1
    int gameId; 

    // UI
    QGridLayout *boardLayout;
    QLabel *statusLabel;
    QVector<QPushButton*> cells; // Flat list mapping to board cells
    QTimer *computerTimer;
    QPoint selectedCell; // For 4x4 Sliding XO

    void updateUI();
    void handleTurn(int r, int c);
    bool checkGameEnd();
    void switchTurn();
    
    // AI Helper
    void performAIMove();
};

#endif // GAMEWINDOW_H
