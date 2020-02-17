/*
 * move.h
 *
 *  Created on: Apr 22, 2016
 *      Author: Home
 */

#ifndef MOVE_H_
#define MOVE_H_


class Move {

  public:
	enum Direction { u, d, l, r};

    Move();
    Move(int, Direction);
    int getPiece();
    Direction getDirection();
    void printMove();

  private:
	int piece;
	Direction direction;

};


#endif /* MOVE_H_ */
