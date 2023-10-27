#pragma once
#include "board.h"


enum PIECETYPE : int{
    BLACKPAWN,
    BLACKBISHOP = 1<<12,
    BLACKKNIGHT = 2<<12,
    BLACKROOK = 3<<12,
    BLACKQUEEN = 4<<12,
    BLACKKING = 5<<12,
    WHITEPAWN = 6<<12,
    WHITEBISHOP = 7<<12,
    WHITEKNIGHT = 8<<12,
    WHITEROOK = 9<<12,
    WHITEQUEEN = 10<<12,
    WHITEKING = 11<<12
};
enum CAPTUREFLAG : int{
    NONE,
    CAPTURE = 1 << 16
};
enum MUMIE : int{
    NORMAL,
    CASTLING = 1 << 17,
    ENPASSANT = 2 << 17,
    PROMOTION = 3 << 17
};
enum PROMOTION_TYPE : int{
    KNIGHT,
    BISHOP = 1 << 19,
    ROOK = 2 << 19,
    QUEEN = 3 << 19
};
enum CAPTURED_PIECE : int{
    CAPT_BLACKPAWN,
    CAPT_BLACKBISHOP = 1<<22,
    CAPT_BLACKKNIGHT = 2<<22,
    CAPT_BLACKQUEEN = 3<<22,
    CAPT_BLACKROOK = 4<<22,
    CAPT_BLACKKING = 5<<22,
    CAPT_WHITEPAWN = 6<<22,
    CAPT_WHITEBISHOP = 7<<22,
    CAPT_WHITEKNIGHT = 8<<22,
    CAPT_WHITEQUEEN = 9<<22,
    CAPT_WHITEROOK = 10<<22,
    CAPT_WHITEKING = 11<<22
};




int lsb(U64 b);
void move_to_string(int move);
void arrayToBitBoard(int rank, int file, char chessboard[rank][file]);
void printBinary(U64 bNum, int bitcount);
void printBoard();
