#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class Tile;
class Piece;
class Rook;
class Knight;
class Bishop;
class Queen;
class King;
class Pawn;
class GameBoard;
class DefaultGameBoard;
enum class PieceType { ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN };

class Tile
{
public:
    GameBoard& at;
    Piece* piece = nullptr;
    int x;
    int y;
    
    Tile(GameBoard&);
};

class Piece
{
public:
    Tile* at;
    int team;
    bool alive = true;
    PieceType type;
    string symbol;
    
    Piece(Tile*, int);
    virtual ~Piece() = default;
    virtual vector<pair<int, int>> getMoves() = 0;
};

class Rook: public Piece
{
public:
    bool castling = true;
    PieceType type = PieceType::ROOK;
    
    Rook(Tile*, int);
    ~Rook();
    vector<pair<int, int>> getMoves();
};

class Knight: public Piece
{
public:
    PieceType type = PieceType::KNIGHT;
    
    Knight(Tile*, int);
    ~Knight();
    vector<pair<int, int>> getMoves();
};

class Bishop: public Piece
{
public:
    PieceType type = PieceType::BISHOP;
    
    Bishop(Tile*, int);
    ~Bishop();
    vector<pair<int, int>> getMoves();
};

class Queen: public Piece
{
public:
    PieceType type = PieceType::QUEEN;
    
    Queen(Tile*, int);
    ~Queen();
    vector<pair<int, int>> getMoves();
};

class King: public Piece
{
public:
    bool castling = true;
    PieceType type = PieceType::KING;
    
    King(Tile*, int);
    ~King();
    vector<pair<int, int>> getMoves();
};

class Pawn: public Piece
{
public:
    bool enp = false;
    PieceType type = PieceType::PAWN;
    
    Pawn(Tile*, int);
    ~Pawn();
    vector<pair<int, int>> getMoves();
};

class GameBoard
{
public:
    size_t size_x;
    size_t size_y;
    vector<vector<Tile*>> board;
    vector<Piece*> pieces[2];
    Piece* royal[2] = {nullptr, nullptr};
    int turn;
    bool playing = false;
    
    GameBoard(size_t _size_x, size_t _size_y);
    ~GameBoard();
    virtual void init() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual bool isChecked(Piece*) = 0;
    virtual vector<pair<int, int>> getLegalMoves(Piece*) = 0;
    virtual bool movePiece(Piece*, pair<int, int>) = 0;
    virtual bool moveCommand(pair<int, int>, pair<int, int>) = 0;
};

class DefaultGameBoard: public GameBoard
{
public:
    DefaultGameBoard();
    ~DefaultGameBoard();
    void init();
    bool start();
    bool stop();
    bool isChecked(Piece*);
    vector<pair<int, int>> getLegalMoves(Piece*);
    bool movePiece(Piece*, pair<int, int>);
    bool moveCommand(pair<int, int>, pair<int, int>);
};

Tile::Tile(GameBoard& _at): at(_at)
{}

Piece::Piece(Tile* _at, int _team): at(_at), team(_team)
{}

Rook::Rook(Tile* _at, int _team): Piece(_at, _team)
{
    symbol = team? "♜": "♖";
}

Rook::~Rook()
{}

vector<pair<int, int>> Rook::getMoves()
{
    vector<pair<int, int>> moves;
    if (!alive) return moves;
    const Tile& tile = *at;
    const GameBoard& board = tile.at;
    for (int x = tile.x + 1; x < board.size_x; ++x) {
        const pair<int, int> move = {x, tile.y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int y = tile.y + 1; y < board.size_y; ++y) {
        const pair<int, int> move = {tile.x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x - 1; x >= 0; --x) {
        const pair<int, int> move = {x, tile.y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int y = tile.y - 1; y >= 0; --y) {
        const pair<int, int> move = {tile.x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    return moves;
}

Knight::Knight(Tile* _at, int _team): Piece(_at, _team)
{
    symbol = team? "♞": "♘";
}

Knight::~Knight()
{}

vector<pair<int, int>> Knight::getMoves()
{
    vector<pair<int, int>> moves;
    if (!alive) return moves;
    const Tile& tile = *at;
    const GameBoard& board = tile.at;
    int movePresets[8][2] = {
        {1,  2}, {2,  1}, {-1,  2}, {-2,  1},
        {1, -2}, {2, -1}, {-1, -2}, {-2, -1}
    };
    for (int i = 0; i < 8; ++i) {
        const pair<int, int> move = {tile.x + movePresets[i][0], tile.y + movePresets[i][1]};
        if (move.first < 0 || move.first >= board.size_x || move.second < 0 || move.second >= board.size_y)
            continue;
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            continue;
        }
        moves.push_back(move);
    }
    return moves;
}

Bishop::Bishop(Tile* _at, int _team): Piece(_at, _team)
{
    symbol = team? "♝": "♗";
}

Bishop::~Bishop()
{}

vector<pair<int, int>> Bishop::getMoves()
{
    vector<pair<int, int>> moves;
    if (!alive) return moves;
    const Tile& tile = *at;
    const GameBoard& board = tile.at;
    for (int x = tile.x + 1, y = tile.y + 1;
        x < board.size_x && y < board.size_y; ++x, ++y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x + 1, y = tile.y - 1;
        x < board.size_x && y >= 0; ++x, --y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x - 1, y = tile.y + 1;
        x >= 0 && y < board.size_y; --x, ++y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x - 1, y = tile.y - 1;
        x >= 0 && y >= 0; --x, --y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    return moves;
}

Queen::Queen(Tile* _at, int _team): Piece(_at, _team)
{
    symbol = team? "♛": "♕";
}

Queen::~Queen()
{}

vector<pair<int, int>> Queen::getMoves()
{
    vector<pair<int, int>> moves;
    if (!alive) return moves;
    const Tile& tile = *at;
    const GameBoard& board = tile.at;
    for (int x = tile.x + 1; x < board.size_x; ++x) {
        const pair<int, int> move = {x, tile.y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int y = tile.y + 1; y < board.size_y; ++y) {
        const pair<int, int> move = {tile.x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x - 1; x >= 0; --x) {
        const pair<int, int> move = {x, tile.y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int y = tile.y - 1; y >= 0; --y) {
        const pair<int, int> move = {tile.x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x + 1, y = tile.y + 1;
        x < board.size_x && y < board.size_y; ++x, ++y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x + 1, y = tile.y - 1;
        x < board.size_x && y >= 0; ++x, --y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x - 1, y = tile.y + 1;
        x >= 0 && y < board.size_y; --x, ++y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    for (int x = tile.x - 1, y = tile.y - 1;
        x >= 0 && y >= 0; --x, --y) {
        const pair<int, int> move = {x, y};
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            break;
        }
        moves.push_back(move);
    }
    return moves;
}

King::King(Tile* _at, int _team): Piece(_at, _team)
{
    symbol = team? "♚": "♔";
}

King::~King()
{}

vector<pair<int, int>> King::getMoves()
{
    vector<pair<int, int>> moves;
    if (!alive) return moves;
    Tile& tile = *at;
    GameBoard& board = tile.at;
    int movePresets[8][2] = {
        {0, -1}, {0,  1}, {-1, -1}, {1,  1},
        {1,  0}, {-1, 0}, {1,  -1}, {-1, 1}
    };
    for (int i = 0; i < 8; ++i) {
        const pair<int, int> move = {tile.x + movePresets[i][0], tile.y + movePresets[i][1]};
        if (move.first < 0 || move.first >= board.size_x || move.second < 0 || move.second >= board.size_y)
            continue;
        const Piece* piece = board.board[move.first][move.second]->piece;
        if (piece != nullptr) {
            if (piece->team != team) moves.push_back(move);
            continue;
        }
        moves.push_back(move);
    }
    // if (castling && !board.isChecked(this)) {
    if (castling) {
        Piece* p1 = board.board[0][tile.y]->piece;
        if (p1 != nullptr && p1->type == PieceType::ROOK) {
            Rook* rook = static_cast<Rook*>(p1);
            if (rook->castling) {
                bool isEmpty = true;
                for (int x = 1; x < tile.x; ++x) {
                    if (board.board[x][tile.y]->piece != nullptr) {
                        isEmpty = false;
                        break;
                    }
                }
                bool onAttack = false;
                // for (int dx = 0; dx > -2; --dx) {
                //     board.board[tile.x + dx][tile.y]->piece = nullptr;
                //     board.board[tile.x + dx - 1][tile.y]->piece = this;
                //     at = board.board[tile.x + dx - 1][tile.y];
                //     if (board.isChecked(this))
                //         onAttack = true;
                // }
                // board.board[tile.x - 2][tile.y]->piece = nullptr;
                // tile.piece = this;
                // at = &tile;
                if (isEmpty && !onAttack)
                    moves.push_back({tile.x - 2, tile.y});
            }
        }
        Piece* p2 = board.board[board.size_x - 1][tile.y]->piece;
        if (p2 != nullptr && p2->type == PieceType::ROOK) {
            Rook* rook = static_cast<Rook*>(p2);
            if (rook->castling) {
                bool isEmpty = true;
                for (int x = board.size_x - 2; x > tile.x; --x) {
                    if (board.board[x][tile.y]->piece != nullptr) {
                        isEmpty = false;
                        break;
                    }
                }
                bool onAttack = false;
                // for (int dx = 0; dx < 2; ++dx) {
                //     board.board[tile.x + dx][tile.y]->piece = nullptr;
                //     board.board[tile.x + dx + 1][tile.y]->piece = this;
                //     at = board.board[tile.x + dx + 1][tile.y];
                //     if (board.isChecked(this))
                //         onAttack = true;
                // }
                // board.board[tile.x + 2][tile.y]->piece = nullptr;
                // tile.piece = this;
                // at = &tile;
                if (isEmpty && !onAttack)
                    moves.push_back({tile.x + 2, tile.y});
            }
        }
    }
    return moves;
}

Pawn::Pawn(Tile* _at, int _team): Piece(_at, _team)
{
    symbol = team? "♟": "♙";
}

Pawn::~Pawn()
{}

vector<pair<int, int>> Pawn::getMoves()
{
    vector<pair<int, int>> moves;
    if (!alive) return moves;
    const int enemy = team? 0: 1;
    const Tile& tile = *at;
    const GameBoard& board = tile.at;
    if (team == 0) {
        if (tile.y == board.size_y - 1) return moves;
        if (board.board[tile.x][tile.y + 1]->piece == nullptr)
            moves.push_back({tile.x, tile.y + 1});
        if (tile.y == 1 && board.board[tile.x][tile.y + 2]->piece == nullptr)
            moves.push_back({tile.x, tile.y + 2});
        if (tile.x > 0) {
            const pair<int, int> move = {tile.x - 1, tile.y + 1};
            const Piece* piece = board.board[move.first][move.second]->piece;
            if (piece != nullptr && piece->team != team)
                moves.push_back(move);
            // En Passant
            Piece* enpPiece = board.board[move.first][move.second - 1]->piece;
            if (enpPiece != nullptr && enpPiece->team == enemy && enpPiece->type == PieceType::PAWN) {
                Pawn* enpPawn = static_cast<Pawn*>(enpPiece);
                if (enpPawn->enp)
                    moves.push_back(move);
            }
        }
        if (tile.x < board.size_x - 1) {
            const pair<int, int> move = {tile.x + 1, tile.y + 1};
            const Piece* piece = board.board[move.first][move.second]->piece;
            if (piece != nullptr && piece->team != team)
                moves.push_back(move);
            // En Passant
            Piece* enpPiece = board.board[move.first][move.second - 1]->piece;
            if (enpPiece != nullptr && enpPiece->team == enemy && enpPiece->type == PieceType::PAWN) {
                Pawn* enpPawn = static_cast<Pawn*>(enpPiece);
                if (enpPawn->enp)
                    moves.push_back(move);
            }
        }
    }
    if (team == 1) {
        if (tile.y == 0) return moves;
        if (board.board[tile.x][tile.y - 1]->piece == nullptr)
            moves.push_back({tile.x, tile.y - 1});
        if (tile.y == board.size_y - 2 && board.board[tile.x][tile.y - 2]->piece == nullptr)
            moves.push_back({tile.x, tile.y - 2});
        if (tile.x > 0) {
            const pair<int, int> move = {tile.x - 1, tile.y - 1};
            const Piece* piece = board.board[move.first][move.second]->piece;
            if (piece != nullptr && piece->team != team)
                moves.push_back(move);
            // En Passant
            Piece* enpPiece = board.board[move.first][move.second + 1]->piece;
            if (enpPiece != nullptr && enpPiece->team == enemy && enpPiece->type == PieceType::PAWN) {
                Pawn* enpPawn = static_cast<Pawn*>(enpPiece);
                if (enpPawn->enp)
                    moves.push_back(move);
            }
        }
        if (tile.x < board.size_x - 1) {
            const pair<int, int> move = {tile.x + 1, tile.y - 1};
            const Piece* piece = board.board[move.first][move.second]->piece;
            if (piece != nullptr && piece->team != team)
                moves.push_back(move);
            // En Passant
            Piece* enpPiece = board.board[move.first][move.second + 1]->piece;
            if (enpPiece != nullptr && enpPiece->team == enemy && enpPiece->type == PieceType::PAWN) {
                Pawn* enpPawn = static_cast<Pawn*>(enpPiece);
                if (enpPawn->enp)
                    moves.push_back(move);
            }
        }
    }
    return moves;
}

GameBoard::GameBoard(size_t _size_x, size_t _size_y):
    size_x(_size_x), size_y(_size_y), board(_size_x, vector<Tile*>(_size_y))
{}

GameBoard::~GameBoard()
{
    for (const vector<Tile*>& v: board)
        for (const Tile* tile: v)
            delete tile;
    board.clear();
    for (int i = 0; i < 2; ++i) {
        for (const Piece* piece: pieces[i])
            delete piece;
        pieces[i].clear();
    }
}

DefaultGameBoard::DefaultGameBoard(): GameBoard(8, 8)
{}

DefaultGameBoard::~DefaultGameBoard()
{}

void DefaultGameBoard::init()
{
    for (int i = 0; i < 2; ++i) {
        for (auto piece: pieces[i])
            delete piece;
        pieces[i].resize(0);
    }
    for (int x = 0; x < size_x; ++x) {
        for (int y = 0; y < size_y; ++y) {
            Tile* tile = board[x][y];
            if (tile != nullptr)
                delete tile;
            tile = new Tile(*this);
            board[x][y] = tile;
            tile->x = x;
            tile->y = y;
        }
    }
    const int presets[2][2] = {{0, 1}, {7, 6}};
    for (int team = 0; team < 2; ++team) {
        vector<Piece*>& teamPieces = pieces[team];
        int row1 = presets[team][0];
        int row2 = presets[team][1];
        // Rooks
        Tile& tileRook1 = *board[0][row1];
        Tile& tileRook2 = *board[7][row1];
        Rook* rook1 = new Rook(&tileRook1, team);
        Rook* rook2 = new Rook(&tileRook2, team);
        tileRook1.piece = rook1;
        tileRook2.piece = rook2;
        teamPieces.push_back(rook1);
        teamPieces.push_back(rook2);
        // Knights
        Tile& tileKnight1 = *board[1][row1];
        Tile& tileKnight2 = *board[6][row1];
        Knight* knight1 = new Knight(&tileKnight1, team);
        Knight* knight2 = new Knight(&tileKnight2, team);
        tileKnight1.piece = knight1;
        tileKnight2.piece = knight2;
        teamPieces.push_back(knight1);
        teamPieces.push_back(knight2);
        // Bishops
        Tile& tileBishop1 = *board[2][row1];
        Tile& tileBishop2 = *board[5][row1];
        Bishop* bishop1 = new Bishop(&tileBishop1, team);
        Bishop* bishop2 = new Bishop(&tileBishop2, team);
        tileBishop1.piece = bishop1;
        tileBishop2.piece = bishop2;
        teamPieces.push_back(bishop1);
        teamPieces.push_back(bishop2);
        // Queen
        Tile& tileQueen = *board[3][row1];
        Queen* queen = new Queen(&tileQueen, team);
        tileQueen.piece = queen;
        teamPieces.push_back(queen);
        // King
        Tile& tileKing = *board[4][row1];
        King* king = new King(&tileKing, team);
        tileKing.piece = king;
        teamPieces.push_back(king);
        royal[team] = king;
        // Pawns
        for (int x = 0; x < 8; ++x) {
            Tile& tile = *board[x][row2];
            Pawn* pawn = new Pawn(&tile, team);
            tile.piece = pawn;
            teamPieces.push_back(pawn);
        }
    }
}

bool DefaultGameBoard::start()
{
    if (playing) return false;
    turn = 0;
    playing = true;
    return true;
}

bool DefaultGameBoard::stop()
{
    if (!playing) return false;
    playing = false;
    return true;
}

bool DefaultGameBoard::isChecked(Piece* piece)
{
    const Tile& tile = *piece->at;
    const pair<int, int> pos = {tile.x, tile.y};
    const int team = piece->team;
    const int enemy = team? 0: 1;
    for (Piece* piece: pieces[enemy]) {
        const vector<pair<int, int>> moves = piece->getMoves();
        for (pair<int, int> move: moves)
            if (move == pos)
                return true;
    }
    return false;
}

vector<pair<int, int>> DefaultGameBoard::getLegalMoves(Piece* piece)
{
    vector<pair<int, int>> moves;
    if (!playing || piece->team != turn) return moves;
    Tile& tile = *piece->at;
    const pair<int, int> pos = {tile.x, tile.y};
    const int team = piece->team;
    const int enemy = team? 0: 1;
    const vector<pair<int, int>> rawMoves = piece->getMoves();
    bool isLegal = false;
    for (const pair<int, int> move: rawMoves) {
        // En Passant
        bool isEnp = false;
        Tile* pTile;
        Pawn* enpPawn;
        if (piece->type == PieceType::PAWN &&
            abs(tile.x - move.first) == 1 && abs(tile.y - move.second) == 1) {
            Piece* enpPiece = board[move.first][move.second + (team == 0? -1: 1)]->piece;
            if (enpPiece != nullptr && enpPiece->team == enemy && enpPiece->type == PieceType::PAWN) {
                enpPawn = static_cast<Pawn*>(enpPiece);
                if (enpPawn->enp) {
                    pTile = enpPawn->at;
                    enpPawn->alive = false;
                    pTile->piece = nullptr;
                    isEnp = true;
                }
            }
        }
        // Castling
        bool isCastling = false;
        Tile* cFTile;
        Tile* cTTile;
        Rook* cRook;
        if (piece->type == PieceType::KING &&
            abs(tile.x - move.first) == 2) {
            bool toRight = tile.x - move.first < 0;
            Piece* cPiece = board[toRight? size_x - 1: 0][tile.y]->piece;
            cRook = static_cast<Rook*>(cPiece);
            cFTile = cRook->at;
            cFTile->piece = nullptr;
            cTTile = board[toRight? move.first - 1: move.first + 1][tile.y];
            cTTile->piece = cRook;
            cRook->at = cTTile;
            isCastling = true;
        }
        // End
        tile.piece = nullptr;
        Tile& newTile = *board[move.first][move.second];
        piece->at = &newTile;
        Piece* orgPiece = newTile.piece;
        if (orgPiece != nullptr)
            orgPiece->alive = false;
        newTile.piece = piece;
        if(!isChecked(royal[team])) moves.push_back(move);
        piece->at = &tile;
        tile.piece = piece;
        if (orgPiece != nullptr) {
            orgPiece->alive = true;
            newTile.piece = orgPiece;
        } else {
            newTile.piece = nullptr;
        }
        // En Passant
        if (isEnp) {
            enpPawn->alive = true;
            pTile->piece = enpPawn;
        }
        // Castling
        if (isCastling) {
            cRook->at = cFTile;
            cFTile->piece = cRook;
            cTTile->piece = nullptr;
        }
    }
    return moves;
}

bool DefaultGameBoard::movePiece(Piece* piece, pair<int, int> target)
{
    if (!playing || piece->team != turn) return false;
    Tile& tile = *piece->at;
    const pair<int, int> from = {tile.x, tile.y};
    const int team = piece->team;
    const int enemy = team? 0: 1;
    const vector<pair<int, int>> moves = getLegalMoves(piece);
    bool isLegal = false;
    for (const pair<int, int> move: moves) {
        if (target == move) {
            isLegal = true;
            break;
        }
    }
    if (!isLegal) return false;
    tile.piece = nullptr;
    Tile& newTile = *board[target.first][target.second];
    piece->at = &newTile;
    Piece* orgPiece = newTile.piece;
    if (orgPiece != nullptr)
        orgPiece->alive = false;
    newTile.piece = piece;
    // Special Rules
    // 1. En Passant
    if (piece->type == PieceType::PAWN) {
        if (abs(from.second - target.second) == 2) {
            Pawn* pawn = static_cast<Pawn*>(piece);
            pawn->enp = true;
        }
        if (abs(from.first - target.first) == 1 && abs(from.second - target.second) == 1) {
            Piece* enpPiece = board[target.first][target.second + (team == 0? -1: 1)]->piece;
            if (enpPiece != nullptr && enpPiece->team == enemy && enpPiece->type == PieceType::PAWN) {
                Pawn* enpPawn = static_cast<Pawn*>(enpPiece);
                if (enpPawn->enp) {
                    Tile& pTile = *enpPawn->at;
                    enpPawn->alive = false;
                    pTile.piece = nullptr;
                }
            }
        }
    }
    // 2. Castling
    if (piece->type == PieceType::KING) {
        King* king = static_cast<King*>(piece);
        king->castling = false;
    }
    if (piece->type == PieceType::ROOK) {
        Rook* rook = static_cast<Rook*>(piece);
        rook->castling = false;
    }
    if (piece->type == PieceType::KING &&
        abs(tile.x - target.first) == 2) {
        bool toRight = tile.x - target.first < 0;
        Piece* cPiece = board[toRight? size_x - 1: 0][tile.y]->piece;
        Rook* cRook = static_cast<Rook*>(cPiece);
        Tile* cFTile = cRook->at;
        cFTile->piece = nullptr;
        Tile* cTTile = board[toRight? target.first - 1: target.first + 1][tile.y];
        cTTile->piece = cRook;
        cRook->at = cTTile;
    }
    // 3. Promotion
    if (piece->type == PieceType::PAWN && (target.second == 0 || target.second == size_y - 1)) {
        string pName;
        Piece* newPiece;
        while (true) {
            cin >> pName;
            if (pName == "q") {
                newPiece = new Queen(&tile, team);
                break;
            }
            if (pName == "r") {
                newPiece = new Rook(&tile, team);
                break;
            }
            if (pName == "b") {
                newPiece = new Bishop(&tile, team);
                break;
            }
            if (pName == "n" || pName == "k") {
                newPiece = new Knight(&tile, team);
                break;
            }
        }
        piece->alive = false;
        pieces[team].push_back(newPiece);
        newTile.piece = newPiece;
        newPiece->at = &newTile;
    }
    // End
    turn = enemy;
    // Disabling En Passant
    for (Piece* t: pieces[turn]) {
        if (t->type == PieceType::PAWN) {
            Pawn* pawn = static_cast<Pawn*>(t);
            pawn->enp = false;
        }
    }
    return true;
}

bool DefaultGameBoard::moveCommand(pair<int, int> from, pair<int, int> target)
{
    Piece* piece = board[from.first][from.second]->piece;
    if (piece == nullptr) return false;
    return movePiece(piece, target);
}