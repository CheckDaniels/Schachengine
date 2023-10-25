#pragma once
#include "board.h"

int lsb(U64 b);
void arrayToBitBoard(int rank, int file, char chessboard[rank][file]);
void printBinary(U64 bNum, int bitcount);
void printBoard();
