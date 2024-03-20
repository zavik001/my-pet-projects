#ifndef MOVE_HPP
#define MOVE_HPP

#include "constants.hpp"
#include "board.hpp"

using namespace Constants;

class Board;

class Move {
public:
  bool crossMove;
  int y, x;

  Move();
  Move(bool _crossMove, int _y, int _x);
  ~Move();
  bool operator==(Move & move);
};

#endif