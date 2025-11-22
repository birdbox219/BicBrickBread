/**
 * @file XO_Demo.cpp
 * @brief Entry point for the FCAI X-O (Tic-Tac-Toe) game.
 *
 * This file initializes the X-O game by creating the user interface, board, and players.
 * It then launches the game loop via the GameManager class.
 * All dynamically allocated objects are properly deleted at the end.
 */

#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr

#include "TUI/TUI.h"
#include "header/TUI_UI.h"
#include "header\BoardGame_Classes.h"
#include "header\XO_Classes.h"
#include "Games\XO_inf\XO_inf.h"
using namespace std;

/**
 * @brief Main function to run the X-O game.
 *
 * This function orchestrates the game by:
 * - Initializing the random number generator
 * - Creating the X-O specific UI and board
 * - Setting up players using the UI
 * - Running the main game loop through the GameManager
 * - Performing cleanup of dynamically allocated memory
 *
 * @return int Returns 0 on successful execution.
 */
int main() {
    // Enable UTF-8 support
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned int>(time(0)));

    TUI::Desktop desktop;
    desktop.init();

    // Main Menu
    TUI::Label* title = new TUI::Label(10, 2, "Board Game Main Menu", "cyan");
    TUI::Button* btnXO = new TUI::Button(10, 6, 20, 3, "Play XO", "blue", "white");
    TUI::Button* btnXOInf = new TUI::Button(10, 10, 20, 3, "Play XO Infinite", "blue", "white");
    TUI::Button* btnExit = new TUI::Button(10, 14, 20, 3, "Exit", "red", "white");

    desktop.addComponent(title);
    desktop.addComponent(btnXO);
    desktop.addComponent(btnXOInf);
    desktop.addComponent(btnExit);

    // Menu Listener
    class MenuListener : public TUI::Button::Listener {
        TUI::Desktop* desktop;
    public:
        int choice = 0;
        MenuListener(TUI::Desktop* d) : desktop(d) {}
        void buttonClicked(TUI::Button* btn) override {
            if (btn->getButtonText() == "Play XO") choice = 1;
            else if (btn->getButtonText() == "Play XO Infinite") choice = 2;
            else if (btn->getButtonText() == "Exit") choice = 3;
        }
    };

    MenuListener menuListener(&desktop);
    btnXO->addListener(&menuListener);
    btnXOInf->addListener(&menuListener);
    btnExit->addListener(&menuListener);

    while (true) {
        // Reset choice
        menuListener.choice = 0;
        
        // Show Menu
        title->setVisible(true);
        btnXO->setVisible(true);
        btnXOInf->setVisible(true);
        btnExit->setVisible(true);

        // Wait for menu selection
        while (menuListener.choice == 0) {
            desktop.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        // Hide Menu
        title->setVisible(false);
        btnXO->setVisible(false);
        btnXOInf->setVisible(false);
        btnExit->setVisible(false);

        if (menuListener.choice == 3) break; // Exit

        // Setup Game
        UI<char>* game_ui = nullptr;
        Board<char>* board = nullptr;
        
        if (menuListener.choice == 1) {
            // XO
            game_ui = new TUI_UI(&desktop, 3, 3);
            board = new X_O_Board();
        } else if (menuListener.choice == 2) {
            // XO Infinite
            game_ui = new TUI_UI(&desktop, 3, 3); // Assuming 3x3 for infinite too? Or is it larger? 
            // XO_inf usually is 3x3 but with shifting rules.
            board = new XO_inf_Board();
        }

        if (game_ui && board) {
            Player<char>** players = game_ui->setup_players();
            GameManager<char> game(board, players, game_ui);
            
            // Run Game
            game.run();

            // Cleanup
            delete board;
            for (int i = 0; i < 2; ++i) delete players[i];
            delete[] players;
            delete game_ui;
            
            // Small pause before returning to menu
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    return 0;
}

// =====================================================================
