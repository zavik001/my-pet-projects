#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include "board.hpp"
#include "move.hpp"

class Game {
private:
  Board board;
public:
  Game();
  ~Game();
  double minimax(Board b, int depth);
  std::vector<Move> moveGenerator;
  int startGame();
};

#endif