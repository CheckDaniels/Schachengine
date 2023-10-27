#pragma once
#include "board.h"
#include <stdbool.h>
#define my_side WHITE
#define op_side (1-WHITE)



// creates a move_list for unmaking moves
typedef struct{
    //reversible information
    int move;
    //irreversible information
    int captures;
    int en_passant;
    int castling;
    int fifty;
    int hash;
}move_history;


extern bool WHITE; //switching: with (1-WHITE)
extern bool WK_NOT_MOVED;
extern bool BK_NOT_MOVED;
extern bool RH1_NOT_MOVED;
extern bool RA1_NOT_MOVED;
extern bool RH8_NOT_MOVED;
extern bool RA8_NOT_MOVED;


// int search_list[][]; Stores the generated moves for each ply inside an 2D array in int format
// 32 bits are enough to store all the important data of a move
// Bit: 0-5     6-11    12-15       16             17-18                                   19-20               (21)         (22-25) only for unmaking moves
//      from    to      Piece type  |CaptureFlag|  (None/Castling/Promotion/en passant)    Promotion:N/B/R/Q   (UnmakeFlag) (captured_Piece: BP/BN/BR/BQ/BK/WP/WN/WB/WR/WQ/WK)
//                                  |Castling-side| = King-side/Queen-side
// captured pieces will be added after the move has been made in make_move()
// Piece types: {BP, BN, BB, BR, BQ, BK, WP, WN, WB, WR, WQ, WK}

extern int search_list[][256];
extern int *move_list;
extern int move_index;
extern int ply;

//game_list: is the search_list with additional information:
//captured pieces, en passant, castling-rights and fifty-move rule
//which is needed for unmake_move()
extern int game_list[16];
extern int *g_move;


bool make_move(int* move);//make_move if(to==ENEMY)
//void unmake_move();
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
void king_safety();
bool square_attacked(U64 square);

