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

#include "Games/anti_XO/Anti_XO.h"
#include "Games/XO_inf/XO_inf.h"
#include "XO_Classes.h"
#include "BoardGame_Classes.h"


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

    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

    // Create an instance of the specific UI for X-O using a pointer 
   /* UI<char>* game_ui = new XO_UI();*/

    // Create the game board. For X-O, this is an X_O_Board.
   /* Board<char>* xo_board = new X_O_Board();*/

    // Use the UI to set up the players for the game.
    // The UI returns a dynamically allocated array of Player pointers.
   /* Player<char>** players = game_ui->setup_players();*/

    // Create the game manager with the board and the array of players.
   /* GameManager<char> x_o_game(xo_board, players, game_ui);*/

    // Run the game loop.
   /* x_o_game.run();*/

    // --- Cleanup ---
    // Delete the dynamically allocated board object.
   /* delete xo_board;*/

    // Delete the individual player objects.
    /*for (int i = 0; i < 2; ++i) {
        delete players[i];
    }*/
    // Delete the dynamically allocated array of player pointers itself.
    //delete[] players;
    bool finish = 0;
    while (!finish) {
        int choice;
        cout << "Choose a game:\n";
        cout << "1)XO\n2)XO_inf\n3)Anti_XO\n4)Exit\n";
        cin >> choice;
        if (choice == 1) {
            UI<char>* game_ui = new XO_UI();
            Board<char>* xo_board = new X_O_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> x_o_game(xo_board, players, game_ui);
            x_o_game.run();
            delete xo_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }

        else if (choice == 2) {
            UI<char>* game_ui = new XO_inf_UI();
            Board<char>* inf_board = new XO_inf_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> XO_inf_game(inf_board, players, game_ui);
            XO_inf_game.run();
            delete inf_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }
        else if(choice == 3){
            UI<char>* game_ui = new Anti_XO_UI();
            Board<char>* inf_board = new Anti_XO_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> antiXO_game(inf_board, players, game_ui);
            antiXO_game.run();
            delete inf_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }
        else if (choice == 4) { finish = 1; }

    }
    return 0; // Exit successfully
}

// =====================================================================
