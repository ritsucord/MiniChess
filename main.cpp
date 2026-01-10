#include <iostream>
#include "chess.hpp"
using namespace std;

void print(DefaultGameBoard& game) {
    const auto& board = game.board;
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++) {
            auto tile = board[x][y];
            string symbol = " ";
            if (tile->piece != nullptr)
                symbol = tile->piece->symbol;
            cout << symbol << " ";
        }
        cout << endl;
    }
}

int main() {
    DefaultGameBoard game = DefaultGameBoard();
    game.init();
    game.start();
    auto board = game.board;
    while (true) {
        cout << endl << "===============" << endl << endl;
        print(game);
        string input;
        cin >> input;
        int fromX = input[0] - '0';
        int fromY = input[1] - '0';
        int toX = input[2] - '0';
        int toY = input[3] - '0';
        game.moveCommand({fromX, fromY}, {toX, toY});
    }
    return 0;
}
