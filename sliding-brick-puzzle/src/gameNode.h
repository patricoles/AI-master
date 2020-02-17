#include <memory>

#include "gameBoard.h"
#include "move.h"
using namespace std;

struct gameNode{
    gameBoard board;
    gameNode* parent;
    int step_count;
    int f_value;
    Move move;
};

class CompareGameNode{
    public:
        bool operator()(const gameNode& a, const gameNode& b)
        {
            return a.f_value > b.f_value;
        }
};
