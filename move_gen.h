#pragma once
#include "board.h"
#include <stdbool.h>

extern U64 OCCUPIED;
extern U64 EMPTY;
extern U64 ENEMY_PIECES;
extern U64 NOT_MY_PIECES;

enum PIECETYPE : int{
    BLACKPAWN = 0,
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
    CAPTURE = 1 << 16, // the CaptureFlag = QUEEN_SIDE flag, since only one of them is possible at a time.
    QUEEN_SIDE = 1 <<16,
    DOUBLEPAWNPUSH = 1<<21
};
enum MOVEMENT_TYPE : int{
    NORMAL,
    CASTLING = 1 << 17,
    ENPASSANT = 2 << 17,
    PROMOTION = 3 << 17
};
enum PROMOTION_TYPE : int{
    KNIGHT = 0,
    BISHOP = 1 << 19,
    ROOK = 2 << 19,
    QUEEN = 3 << 19
};
enum CASTLINGS : int{
    WHITE_KS_CASTLING = CASTLING + WHITEROOK + (2 << 6) + 0,
    WHITE_QS_CASTLING = CASTLING + QUEEN_SIDE + WHITEROOK + (4 << 6) + 7,
    BLACK_KS_CASTLING = CASTLING + BLACKROOK + (58 << 6) + 56,
    BLACK_QS_CASTLING = CASTLING + QUEEN_SIDE + BLACKROOK + (60 << 6) + 63
};


void move_generator(int* movelist_ptr, int* move_count);
void genWPawn();
void genBPawn();
U64 genDiagonal(U64 DMP);
U64 genStraight(U64 SMP);
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

