/*
 * gameBoard.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: Home
 */

#include <fstream>
#include <cstdio>
#include <ctime>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <memory>

#include "move.h"
#include "gameBoard.h"
#include "gameNode.h"

using namespace std;


size_t gameBoard::hash() const
{
    size_t temp = 0;
    for (int y=0; y<height;++y)
    for (int x = 0; x<width; ++x)
    {
        temp = 23*temp+board[y][x]+1;
    }

    return temp;
}

namespace std{
    template<> struct hash<gameBoard>{
        size_t operator()(gameBoard const& g) const
        {
            return g.hash();
        }
    };
}

//checks if space between two points is empty
bool gameBoard::isEmpty(int y1, int x1, int y2, int x2, int piece){

    int x_direction = 0, y_direction = 0;
    if(x2-x1 > 0)
        x_direction = 1;
    if( y2-y1 > 0)
        y_direction = 1;

    //special case of one cell
    if (x_direction == 0 && y_direction == 0)
        return board[y1][x1] == 0 || (piece == 2 && board[y1][x1] == -1);

    for(int x = x1, y=y1; x<=x2 && y<=y2; x+= x_direction, y+=y_direction)
    {
        if( board[y][x] > 0 || ( piece != 2 && board[y][x] == -1))
            return false;
    }

    return true;
}

bool gameBoard::isValidMove(Move m)
{
    int pieceIndex = m.getPiece();
    Shape piece = pieceMap[pieceIndex];
    int y, x, width, height;
    y = piece.y;
    x = piece.x;
    width = piece.width;
    height = piece.height;
    
    return (m.getDirection() == Move::u && isEmpty(y-1, x, y-1, x+width-1, pieceIndex))
    || (m.getDirection() == Move::d && isEmpty(y+height, x, y+height, x+width-1,pieceIndex))
    ||(m.getDirection() == Move::l && isEmpty(y,x-1,y+height-1,x-1,pieceIndex))
    || (m.getDirection() == Move::r && isEmpty(y, x+width, y+height-1, x+width,pieceIndex));
}

//check moves for piece on board by checking for empty spaces by piece
//first gets the pieces x y and width and height to properly check all empty spaces
//if empty space is total in the area then its added to the vector of moves possible
vector<Move> gameBoard::getMovesForPiece(int pieceIndex)
{
    vector<Move> result;
    vector<Move> testMoves = {Move(pieceIndex, Move::l), Move(pieceIndex, Move::r), Move(pieceIndex, Move::u), Move(pieceIndex, Move::d)};
    for(int i=0; i<testMoves.size(); ++i)
        if(isValidMove(testMoves[i]))
            result.push_back(testMoves[i]);

    return result;
}

//gets all possible moves on board and pushes to to a vector of moves

vector<Move> gameBoard::getAllMoves(){

    vector<Move> result;

    for(int i = 2; i <= maxPiece; i++){

        vector<Move> movesForPiece = getMovesForPiece(i);
        result.insert(result.end(), movesForPiece.begin(), movesForPiece.end() );
    }

    return result;
}


void gameBoard::applyMove(Move move){

    int y, x, width, height;
    Shape& piece = pieceMap[move.getPiece()];

    y = piece.y;
    x = piece.x;
    width = piece.width;
    height = piece.height;

    if(move.getDirection() == Move::u){
        for(int x1 = x; x1 < x+width; x1++){
            board[y-1][x1] = move.getPiece();
            board[y+height-1][x1] = 0;
        }

        piece.y--;
    }
    else if(move.getDirection() == Move::d){
        for(int x1 = x; x1 < x+width; x1++){
            board[y+height][x1] = move.getPiece();
            board[y][x1] = 0;
        }

        piece.y++;
    }
    else if(move.getDirection() == Move::l){
        for(int y1 = y; y1 < y+height; y1++){
            board[y1][x-1] = move.getPiece();
            board[y1][x+width-1] = 0;
        }

        piece.x--;
    }

    else if(move.getDirection() == Move::r){
        for(int y1 = y; y1 <y+height; y1++){
            board[y1][x+width] = move.getPiece();
            board[y1][x] = 0;
        }

        piece.x++;
    }

    normalize();
}

//this creates a gambaord
 gameBoard::gameBoard () {
    // row = -1;
    // column = -1;
     width = -1;
     height = -1;
}

bool operator==(gameBoard game1, gameBoard game2){

  return game1.board==game2.board;
}

 void gameBoard::swapIdx(int idx1,int idx2) {
    Shape piece1 = pieceMap[idx1];
    Shape piece2 = pieceMap[idx2];

    for(int y=piece1.y; y<piece1.y+piece1.height; ++y)
    for(int x=piece1.x; x<piece1.x+piece1.width; ++x)
       board[y][x] = idx2;

    for(int y=piece2.y; y<piece2.y+piece2.height; ++y)
    for(int x=piece2.x; x<piece2.x+piece2.width; ++x)
       board[y][x] = idx1;

    pieceMap[idx1] = piece2;
    pieceMap[idx2] = piece1;
}

 void gameBoard::normalize(){
     int nextIdx = 3;
     for(int i = 0;i < height;i++)
     for(int j = 0;j < width;j++) {
         if (board[i][j]==nextIdx) {
             nextIdx++;
         } else if (board[i][j] > nextIdx) {
             swapIdx(nextIdx,board[i][j]);
             nextIdx++;
         }
     }
 }


 void gameBoard::randomWalks(int n){

     srand(time(NULL));
     for( int x = 0; x < n; x++){

        vector <Move> moves= getAllMoves();
        Move move = moves[rand()%moves.size()];
        applyMove(move);
        move.printMove();
        consoleOutput();

        if(isSolved()){
            return;
        }
     }
 }


 //this copys a gameBoard
 gameBoard gameBoard::applyMoveCloning (Move move) {
    gameBoard result(*this);
    result.applyMove(move);
    return result;
 }



//parse the line to get those piece number between them commas
 vector<int> parseLine(string line)
{
     vector<int> result;
     int commaIndex;

     while( (commaIndex = line.find(',')) != -1 )
     {
         string token = line.substr(0,commaIndex);
         int tokenValue = atoi(token.c_str());
         result.push_back( tokenValue);
         line = line.substr(commaIndex+1);
     }

     return result;
}

//neeed to fix this //read in file
 // Reads values from a file and converts them to a 2D vector
void gameBoard::loadGame(string fileName){
    string line;
    ifstream myfile (fileName.c_str());
    getline (myfile,line);
    vector<int> dimensions = parseLine(line);
    width = dimensions[0];
    height = dimensions[1];

    while ( getline (myfile,line) )
    {
        board.push_back(parseLine(line));
    }

    myfile.close();

    normalize();

    bool goal_not_set = true;
    //find locations of pieces
    maxPiece = 0;
    for(int y=0; y<height; y++)
    for(int x=0; x<width; x++)
    {
        int pieceNumber = board[y][x];
        if( pieceNumber >=2 && !pieceMap.count(pieceNumber) )
        {
            if( pieceNumber > maxPiece )
                maxPiece = pieceNumber;

            Shape piece;
            piece.y = y;
            piece.x = x;    
            piece.width=0;
            piece.height=0;
            while(board[y][x+piece.width] == pieceNumber)
                piece.width++;
            while(board[y+piece.height][x] == pieceNumber)
                piece.height++; 

            pieceMap[pieceNumber] = piece;
        }
        else if( pieceNumber == -1 && goal_not_set)
        {
            goal.y = y;
            goal.x = x;
            goal.width = 0;
            goal.height = 0;
            while(x + goal.width < width && board[y][x+goal.width] == -1)
                goal.width++;
            while(y + goal.height < height && board[y+goal.height][x] == -1)
                goal.height++;

            goal_not_set = false;
        }
    }
}

 //this works
 // console outs the look of the vector board
//this is important and makes me feel good
 void gameBoard::consoleOutput(){
        int x;
        int y;
        cout << "width" << ':' << width ;
        cout << endl ;
        cout << "height" << ':' << height;
        cout << endl ;
        for(x = 0; x < height; x++){
            for(y = 0; y < width; y++){
                cout << board[x][y] << ',' ;
            }
            cout << endl ;
        }
 }


//check if the game is solved
// step through matrix and look for -1 which means brick isnt on goal state
bool gameBoard::isSolved(){


    int x;
    int y;

    for(x = 0; x < height; x++){
        for(y = 0;y < width; y++){
            if(board[x][y] == -1)
                return false;
        }
    }
    return true;
}

void depthFirstSearch(gameBoard gameState)
{
    unordered_set<gameBoard> visitedNodes;
    unordered_map<gameBoard, gameBoard> childToParent;
    unordered_map<gameBoard, vector<gameBoard> > parentToChildren;
    unordered_map<gameBoard, int> parentToCurrentChild;
    unordered_map<gameBoard, Move> generatingMoves;

    gameBoard currentNode = gameState;

    while(!currentNode.isSolved())
    {
        if( !parentToChildren.count(currentNode))
        {
            vector<gameBoard> children;
            vector<Move> moves = currentNode.getAllMoves();
            for(int i=0; i<moves.size(); ++i)
            {
                Move move1 = moves[i];
                gameBoard newNode = currentNode.applyMoveCloning(move1);
                if(!visitedNodes.count(newNode))
                {
                    children.push_back(newNode);
                    childToParent.insert(std::pair<gameBoard, gameBoard>(newNode,currentNode));
                    generatingMoves.insert(std::pair<gameBoard, Move>(newNode, moves[i]));
                }
            }

            parentToChildren.insert(std::pair<gameBoard, vector<gameBoard> >(currentNode,children));
            parentToCurrentChild.insert(std::pair<gameBoard, int>(currentNode, 0));
        }

        visitedNodes.insert(currentNode);

        if( parentToCurrentChild[currentNode] < (parentToChildren[currentNode]).size())
            currentNode = parentToChildren[currentNode][parentToCurrentChild[currentNode]++];
        else
            currentNode = childToParent[currentNode];
    }

    cout << visitedNodes.size() << endl;

    gameBoard endNode = currentNode;
    vector<Move> solution;
    while( childToParent.count(currentNode))
    {
        solution.insert(solution.begin(), generatingMoves[currentNode]);

        currentNode = childToParent[currentNode];
    }

    for(int i=0; i<solution.size(); ++i)
    {

        Move printMove = solution[i];
        printMove.printMove();
    }

    endNode.consoleOutput();
}

bool breadthFirstSearch (gameBoard gamestate, int searchDepth=1000000){

    vector <gameBoard> previousStates;
    previousStates.push_back(gamestate);

    unordered_set <gameBoard> visitedStates;
    visitedStates.insert(gamestate);

    //map
    unordered_map <gameBoard, gameBoard> childToParent;
    unordered_map <gameBoard, Move> generatingMoves;

    for(int n=0; n<searchDepth && !previousStates.empty(); ++n){
        //next states
        vector <gameBoard> nextStates;

        for(unsigned int x = 0; x < previousStates.size(); x++){

            vector <Move> moves= previousStates[x].getAllMoves();
            //for
            for(unsigned int y = 0; y < moves.size() ; y++){

                gameBoard newGame;
                Move newMove = moves[y];

                newGame = previousStates[x].applyMoveCloning( newMove);

                if(!(visitedStates.count(newGame))){

                    //nextStates.
                    nextStates.push_back(newGame);
                    childToParent[newGame]=previousStates[x];
                    generatingMoves[newGame] = newMove;
                    if(newGame.isSolved()){
                        cout << visitedStates.size() << endl;

                        vector<Move> solution;
                        gameBoard currentState = newGame;
                        solution.push_back(newMove);

                        while(childToParent.count(currentState))
                        {
                            currentState = childToParent[currentState];
                            solution.push_back(generatingMoves[currentState]);
                        }

                        for(int i=solution.size()-1; i>=0; i--)
                        {
                            //solution[i].printMove();
                            Move printMove = solution[i];
                            printMove.printMove();
                        }

                        newGame.consoleOutput();

                        return true;
                    }

                    visitedStates.insert(newGame);
                }

            }
        //is
        }

        previousStates = nextStates;

    //break;
    }

    return false;
}

void iterativeDeepeningSearch(gameBoard gameState)
{
    int depth = 3;
    while (!breadthFirstSearch(gameState,depth))
        depth++;
}

int h_improved(gameBoard g)
{
    int goal_y, goal_x, master_y, master_x;

    bool goal_top_or_left = g.goal.y==0 || g.goal.x == 0;
    if(goal_top_or_left)
    {
        goal_y = g.goal.y;
        goal_x = g.goal.x;
        master_y = g.pieceMap[2].y;
        master_x = g.pieceMap[2].x;
    }else
    {
        goal_y = g.goal.y + g.goal.height-1;
        goal_x = g.goal.x + g.goal.width-1;
        master_y = g.pieceMap[2].y + g.pieceMap[2].height-1;
        master_x = g.pieceMap[2].x + g.pieceMap[2].width-1;
    }

    bool isBlocked = !( (goal_x < master_x && g.isValidMove(Move(2,Move::l))) || (goal_x > master_x && g.isValidMove(Move(2,Move::r))) || (goal_y < master_y && g.isValidMove(Move(2,Move::u))) || (goal_y > master_y && g.isValidMove(Move(2,Move::d))));

    return h(g)+(int)isBlocked;
}
int h(gameBoard g)
{
    int goal_y, goal_x, master_y, master_x;

    bool goal_top_or_left = g.goal.y==0 || g.goal.x == 0;
    if(goal_top_or_left)
    {
        goal_y = g.goal.y;
        goal_x = g.goal.x;
        master_y = g.pieceMap[2].y;
        master_x = g.pieceMap[2].x;
    }else
    {
        goal_y = g.goal.y + g.goal.height-1;
        goal_x = g.goal.x + g.goal.width-1;
        master_y = g.pieceMap[2].y + g.pieceMap[2].height-1;
        master_x = g.pieceMap[2].x + g.pieceMap[2].width-1;
    }
    
    return abs(goal_y-master_y) + abs(goal_x-master_x);
}

void printSolution(gameNode* currentNode, int numVisited)
{
    vector<gameNode*> solution;
    while(currentNode != NULL)
    {
        solution.insert(solution.begin(), currentNode);
        currentNode = currentNode->parent;
    }

    cout << "Visited " << numVisited << " states" << endl;
    cout << "Solution is " << solution.size() << " moves." << endl;
    for(int i=0; i<solution.size(); ++i)
        solution[i]->move.printMove();
}

void aStarSearch(gameBoard gameState, int (*heuristic)(gameBoard))
{
    int numStatesVisited = 0;
    unordered_set <gameBoard> visitedStates;
    visitedStates.insert(gameState);

    priority_queue<gameNode, vector<gameNode>, CompareGameNode> frontier;
    gameNode startNode;
    startNode.board = gameState;
    startNode.parent = NULL;
    startNode.step_count = 0;
    startNode.f_value = heuristic(gameState);
    startNode.move = Move(0,Move::d);

    frontier.push(startNode);
  
    while(!frontier.empty())
    {
        gameNode currentNode = frontier.top();
        frontier.pop();
     
        numStatesVisited++; 
        if(currentNode.board.isSolved())
        {
            //change aStarSearch to return solution
            //then pass these arguments to printSolution in main
            //that way timing will be more accurate
            printSolution(&currentNode, numStatesVisited); 
            return;
        } 

        vector<Move> nextMoves = currentNode.board.getAllMoves();

        for(int i=0; i<nextMoves.size(); ++i)
        {
            gameBoard newState = currentNode.board.applyMoveCloning(nextMoves[i]);
            if(visitedStates.count(newState))
                continue;

            visitedStates.insert(newState);
            gameNode newNode;
            newNode.board = newState;
            newNode.parent = new gameNode(currentNode);
            newNode.step_count = currentNode.step_count+1;
            newNode.f_value = newNode.step_count + heuristic(newState);
            newNode.move = nextMoves[i];
       
            frontier.push(newNode); 
        }
    }
}


int main() {
        //std::unordered_set <gameBoard> visitedStates;
        gameBoard game;
        game.loadGame("../Puzzles/SBP-bricks-level7.txt");
        cout << "Using manhattan distance" << endl;
        aStarSearch(game,h);
        cout << "Using improved manhattan distance" << endl;
        aStarSearch(game,h_improved);
        return 0;
}
