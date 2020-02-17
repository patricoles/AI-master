/*
 * move.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: Home
 */

// example: one class, two objects
#include <iostream>
using namespace std;

#include "move.h"


//Constructors
Move::Move(){
	piece = 0;
	direction = u;
}

/*
Move::Move(const Move &mSource){

	piece = mSource.piece;
	direction = mSource.direction;
}
*/

Move::Move(int piece, Direction direction)
{
	this->piece = piece;
	this->direction = direction;
}

//Inspectors
void Move::printMove(){
    if(piece == 0)
    {
        cout << "Initial state" << endl;
        return;
    }
	if(direction == u)
	  cout << "(" << piece << ", up)" << endl;
	else if (direction == d)
		cout << "(" << piece << ", down)" << endl;
	else if (direction == l)
		cout << "(" << piece << ", left)" << endl;
	else if (direction == r)
		cout << "(" << piece << ", right)" << endl;
}

int Move::getPiece(){
	return piece;
}

Move::Direction Move::getDirection(){
	return direction;
}



