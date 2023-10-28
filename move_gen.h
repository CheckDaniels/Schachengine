#pragma once
#include "board.h"
#include <stdbool.h>


extern U64 OCCUPIED;
extern U64 EMPTY;
extern U64 ENEMY_PIECES;
extern U64 NOT_MY_PIECES;

extern bool WK_NOT_MOVED;
extern bool BK_NOT_MOVED;
extern bool RH1_NOT_MOVED;
extern bool RA1_NOT_MOVED;
extern bool RH8_NOT_MOVED;
extern bool RA8_NOT_MOVED;
extern bool Enpassant_possibility;
extern int doublePawnpushFile; // 0:FileH, 1:FileG, ..., 7:FileA

enum PIECETYPE : int{
    BLACKPAWN,
    BLACKKNIGHT = 1<<12,
    BLACKBISHOP = 2<<12,
    BLACKROOK = 3<<12,
    BLACKQUEEN = 4<<12,
    BLACKKING = 5<<12,
    WHITEPAWN = 6<<12,
    WHITEKNIGHT = 7<<12,
    WHITEBISHOP = 8<<12,
    WHITEROOK = 9<<12,
    WHITEQUEEN = 10<<12,
    WHITEKING = 11<<12
};
enum FLAGS : int{
    NONE,
    CAPTURE = 1 << 16,
    DOUBLEPAWNPUSH = 1<<21
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
    CAPT_BLACKKNIGHT = 1<<23,
    CAPT_BLACKBISHOP = 2<<23,
    CAPT_BLACKROOK = 3<<23,
    CAPT_BLACKQUEEN = 4<<23,
    CAPT_BLACKKING = 5<<23,
    CAPT_WHITEPAWN = 6<<23,
    CAPT_WHITEKNIGHT = 7<<23,
    CAPT_WHITEBISHOP = 8<<23,
    CAPT_WHITEROOK = 9<<23,
    CAPT_WHITEQUEEN = 10<<23,
    CAPT_WHITEKING = 11<<23
};


void move_generator();
void genWPawn();
void genBPawn();
U64 genDiagonal(U64 DP);
U64 genStraight(U64 SP);
U64 genKnightSpan(U64 NMP);
U64 genKingSpan(U64 KMP);
void genRook(U64 RBB);
void genBishop(U64 BBB);
void genQueen(U64 QBB);
void genKnight(U64 NBB);
void genKing(U64 KBB);

void genWCastling();
void genBCastling();
bool square_attacked(U64 square);

