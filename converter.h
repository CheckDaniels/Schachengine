#pragma once
#include "board.h"

int lsb(U64 b);
void move_to_string(int move);
void arrayToBitBoard(int rank, int file, char chessboard[rank][file]);
void printBinary(U64 bNum, int bitcount);
void printBoard();

U64 reverse_bytes(U64 BitB);
U64 reverse_rank(U64 BitB, int rank);

