#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include "move.hpp"
#include "constants.hpp"

using namespace Constants;

class Move;

class Board {
private:
  int board[SIZE][SIZE];
public:
  Board();
  ~Board();
  void set(int figure, int y, int x);
  double evalute();
  std::vector<Move> generator();
  void go(Move& mv);
  void print();
  bool testOfLegalMove(Move& mv);
  bool isTerminate();

  bool crossMove = true;
};

#endif