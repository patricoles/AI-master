#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>

using namespace std;

struct Shape{
    int y;
    int x;
    int width;
    int height;
};

class gameBoard {
    int width, height;
    int maxPiece;
    vector< vector< int > > board;
    unordered_map<int, Shape>  pieceMap;
    Shape goal;

    bool isEmpty(int y1, int x1, int y2, int x2, int piece);
    void swapIdx(int idx1,int idx2);
    void normalize();

  public:
    gameBoard();
    void loadGame(string filename);

    bool isValidMove(Move m);
    vector<Move> getMovesForPiece(int piece);
    vector<Move>  getAllMoves();
    void applyMove(Move move);
    gameBoard applyMoveCloning (Move move);

    bool isSolved();
    friend int h(gameBoard g);
    friend int h_improved(gameBoard g);

    void consoleOutput();
    friend bool operator==(gameBoard game1, gameBoard game2);
    size_t hash () const;

    void randomWalks(int n);
};

#endif
