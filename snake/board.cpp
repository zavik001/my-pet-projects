#include "board.hpp"

Board::Board() {
  for(int y = 0; y < SIZE; ++y) {
    for(int x = 0; x < SIZE; ++x) {
      board[y][x] = EMPTY;
    }
  }
}

Board::~Board() {}

void Board::set(int figure, int y, int x) {
  board[y][x] = figure;
}

double Board::evalute() {
  int mul = 1;

  if(!crossMove) {
    mul = -1;
  }

  for(int y = 0; y < SIZE; ++y) {
    int count = 0;
    for(int x = 0; x < SIZE; ++x) {
      if(board[y][x] == EMPTY) { break; }
      if(board[y][x] == board[y][0]) { ++count; }
    }

    if(count >= SIZE) {
      if(board[y][0] == CROSS) { return 1 * mul; }
      else { return -1 * mul; }
    }
  }

  for(int x = 0; x < SIZE; ++x) {
    int count = 0;
    for(int y = 0; y < SIZE; ++y) {
      if(board[y][x] == EMPTY) { break; }
      if(board[y][x] == board[0][x]) { ++count; }
    }

    if(count >= SIZE) {
      if(board[0][x] == CROSS) { return 1 * mul; }
      else { return -1 * mul; }
    }
  }

  int count = 0;
  for(int i = 0, j = 0; i < SIZE || j < SIZE; ++i, ++j) {
    if(board[i][j] == EMPTY) {
      break;
    }

    if(board[i][j] == board[0][0]) {
      ++count;
    }
  }

  if(count >= SIZE) {
    if(board[0][0] == CROSS) { return 1 * mul; }
    else { return -1 * mul; }
  }

  count = 0;
  for(int i = 0, j = SIZE - 1; i < SIZE || j >= 0; ++i, --j) {
    if(board[i][j] == EMPTY) {
      break;
    }

    if(board[i][j] == board[0][SIZE - 1]) {
      ++count;
    }
  }

  if(count >= SIZE) {
    if(board[0][SIZE-1] == CROSS) { return 1 * mul; }
    else { return -1 * mul; }
  }

  return 0;
}

std::vector<Move> Board::generator() {
  std::vector<Move> result;

  for(int y = 0; y < SIZE; ++y) {
    for(int x = 0; x < SIZE; ++x) {
      if(board[y][x] == EMPTY) {
        result.push_back(Move(crossMove, y, x));
      }
    }
  }

  return result;
}

void Board::go(Move& mv) {
  if(mv.crossMove) {
    set(CROSS, mv.y, mv.x);
  } else {
    set(ZERO, mv.y, mv.x);
  }

  crossMove = !crossMove;
}

void Board::print() {
  for(int y = 0; y < SIZE; ++y) {
    for(int x = 0; x < SIZE; ++x) {
      if(board[y][x] == CROSS) {
        std::cout << "x";
      } else if(board[y][x] == ZERO) {
        std::cout << "o";
      } else {
        std::cout << ".";
      }
    }

    std::cout << "\n";
  }
}

bool Board::testOfLegalMove(Move& mv) {
  std::vector<Move> moves = generator();

  for(unsigned int i = 0; i < moves.size(); ++i) {
    if(moves[i] == mv) { return true; }
  }

  return false;
}

bool Board::isTerminate() {
  if(evalute() != 0) {
    return true;
  }

  for(int i = 0; i < SIZE; ++i) {
    for(int j = 0; j < SIZE; ++j) {
      if(board[i][j] == EMPTY) { return false; }
    }
  }

  return true;
}