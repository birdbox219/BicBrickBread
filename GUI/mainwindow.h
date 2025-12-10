#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "gamewindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGameClicked();

private:
    // UI Elements
    QComboBox *gameSelector;
    QLineEdit *player1Name;
    QComboBox *player1Type; // Human or Computer
    QLineEdit *player2Name;
    QComboBox *player2Type;
    
    // Helper to setup UI
    void setupUi();

    // Map game index to readable name
    struct GameInfo {
        int id;
        QString name;
    };
    QList<GameInfo> games;
};

#endif // MAINWINDOW_H
