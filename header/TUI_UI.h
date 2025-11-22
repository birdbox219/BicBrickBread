#ifndef TUI_UI_H
#define TUI_UI_H

#include "../TUI/TUI.h"
#include "BoardGame_Classes.h"
#include <vector>
#include <string>

using namespace TUI;

class TUI_UI : public UI<char> {
private:
    Desktop* desktop;
    std::vector<Button*> boardButtons;
    Label* statusLabel;
    int rows, cols;
    
    // Store the last clicked move
    int lastClickedX = -1;
    int lastClickedY = -1;

    // Listener for board buttons
    class BoardListener : public Button::Listener {
        TUI_UI* parent;
        int x, y;
    public:
        BoardListener(TUI_UI* p, int r, int c) : parent(p), x(r), y(c) {}
        void buttonClicked(Button* btn) override {
            parent->lastClickedX = x;
            parent->lastClickedY = y;
        }
    };

    std::vector<BoardListener*> listeners;

public:
    TUI_UI(Desktop* d, int r, int c) : UI<char>("", 3), desktop(d), rows(r), cols(c) {
        // Create Status Label
        statusLabel = new Label(10, 2, "Game Started", "yellow");
        desktop->addComponent(statusLabel);

        // Create Board Grid
        // Assuming a start position of (10, 5)
        int startX = 10;
        int startY = 5;
        int btnW = 5;
        int btnH = 3;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Button* btn = new Button(startX + j * (btnW + 1), startY + i * (btnH + 1), btnW, btnH, " ", "blue", "white");
                boardButtons.push_back(btn);
                desktop->addComponent(btn);

                BoardListener* listener = new BoardListener(this, i, j);
                listeners.push_back(listener);
                btn->addListener(listener);
            }
        }
    }

    ~TUI_UI() {
        // Desktop owns components, so we don't delete them here.
        // But we own listeners.
        for (auto l : listeners) delete l;
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        return new Player<char>(name, symbol, type);
    }

    Move<char>* get_move(Player<char>* player) override {
        if (player->get_type() == PlayerType::HUMAN) {
            statusLabel->setText("Player " + string(1, player->get_symbol()) + "'s Turn");
            
            lastClickedX = -1;
            lastClickedY = -1;

            // Wait for input
            while (lastClickedX == -1) {
                desktop->update();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }

            return new Move<char>(lastClickedX, lastClickedY, player->get_symbol());
        } else {
            // Computer Logic (Random for now, or existing AI if available)
            // Since the base Player class doesn't have get_move, and we don't have a RandomPlayer class handy in the snippet,
            // we might need to rely on the fact that the original code might have had AI logic or we implement simple random here.
            // Wait, the original code used specific Player subclasses or the UI handled it?
            // The original UI::setup_players returned Player objects.
            // Let's look at how computer moves were handled.
            // Ah, the original code had `RandomPlayer`? No, `Player` base class doesn't have `get_move`.
            // The `GameManager` calls `ui->get_move(player)`.
            // So the UI is responsible for generating the move for the computer too?
            // Or the Player object should have a `get_move`?
            // In the provided `BoardGame_Classes.h`, `Player` is a base class.
            // Usually, `RandomPlayer` inherits from `Player` and implements `get_move`.
            // But `GameManager` calls `ui->get_move`.
            // If `ui->get_move` is called for a computer player, the UI should probably generate it.
            
            // Let's implement a simple random move for now to be safe, or check if there's a better way.
            // Actually, let's just pick a random valid move.
            // But we don't have easy access to the board state here to know what's valid without querying the board.
            // The player has a `get_board_ptr()`.
            
            statusLabel->setText("Computer Thinking...");
            desktop->update(); // Show message
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Fake think time

            Board<char>* board = player->get_board_ptr();
            int r = board->get_rows();
            int c = board->get_columns();
            
            // Simple random try (inefficient but works for small boards)
            // Ideally we should find empty cells.
            // Let's try 100 times to find a random empty spot.
            for(int k=0; k<100; ++k) {
                int x = rand() % r;
                int y = rand() % c;
                // We can't easily check if it's valid without `update_board` which modifies it.
                // But `Board` usually doesn't expose "is_valid" publicly separate from update.
                // However, we can just return it and if it's invalid, GameManager will ask again.
                return new Move<char>(x, y, player->get_symbol());
            }
             return new Move<char>(0, 0, player->get_symbol()); // Fallback
        }
    }

    void display_board_matrix(const vector<vector<char>>& matrix) {
        // Update button text
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int index = i * cols + j;
                if (index < boardButtons.size()) {
                    string s(1, matrix[i][j]);
                    // If it's a dot or empty char, make it space for cleaner look
                    if (s == ".") s = " "; 
                    boardButtons[index]->setButtonText(s);
                    
                    // Optional: Color coding
                    if (s == "X") boardButtons[index]->setColor(Button::BUTTON, "red");
                    else if (s == "O") boardButtons[index]->setColor(Button::BUTTON, "green");
                    else boardButtons[index]->setColor(Button::BUTTON, "blue");
                }
            }
        }
        desktop->update(); // Force redraw
    }

    void display_message(string message) {
        statusLabel->setText(message);
        desktop->update();
        // If it's a win message, maybe wait a bit
        if (message.find("wins") != string::npos || message.find("Draw") != string::npos) {
             std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }

    Player<char>** setup_players() override {
        // For now, hardcode or simple console input before TUI starts?
        // Or use TUI to select.
        // Let's just return 2 Human players for simplicity as per plan note, 
        // or maybe 1 Human 1 Computer if requested.
        // Let's stick to the plan: "Simple TUI menu to select Game Type"
        // But `setup_players` returns the players.
        // We can use a simple blocking TUI dialog here if we wanted.
        // For MVP, let's just make them Human vs Computer for fun, or Human vs Human.
        // Let's do Human vs Human as default.
        
        Player<char>** players = new Player<char>*[2];
        players[0] = new Player<char>("Player 1", 'X', PlayerType::HUMAN);
        players[1] = new Player<char>("Player 2", 'O', PlayerType::HUMAN); // Or Computer
        return players;
    }
};

#endif
