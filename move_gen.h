#pragma once
#include "board.h"



// Bit: 0-5         6-11    12-13               14-15
//      (from<<6)+  to+     Promotion:N/Q/R/B+  (None/Promotion/en passant/Castling)

extern int movelist[];
extern int listindex;

enum Move_type : int{
    NORMAL,
    ENPASSANT = 1 << 14,
    PROMOTION = 2 << 14,
    CASTLING = 3 << 14
};

enum Promotion_type : int{
    QUEEN,
    KNIGHT = 1 << 12,
    BISHOP = 2 << 12,
    ROOK = 3 << 12
};

enum playertype : int{
    WHITE,
    BLACK
};
enum movemask : int{
    MASK_FROM = 63,
    MASK_TO = 63 << 6,
    MASK_PROMOTION = 3 << 12,
    MASK_MOVETYPE = 3 << 14
};

void makemove(int move);//makemove if(to==ENEMY)
void move_notation(int move);
void move_generator(int COLOR);
void genWPawn();
void genBPawn();
U64 genDiagonal(U64 DP);
U64 genStraight(U64 SP);
void genRook(U64 RBB);
void genBishop(U64 BBB);
void genQueen(U64 QBB);
void genKnight(U64 NBB);
void genKing(U64 KBB);

