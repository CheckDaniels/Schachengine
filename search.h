#pragma once
#define my_side WHITE
#define op_side (1-WHITE)
#define unmake_move(x) make_move(x);


extern bool WHITE;
extern int ply;

//captured pieces, en passant, castling-rights and fifty-move rule//
// int search_list[][]; Stores the generated moves for each ply inside an 2D array in int format
// 32 bits are enough to store all the important data of a move
// Bit: 0-5     6-11    12-15       16             17-18                                   19-20                21      (22)            (23-26) only for unmaking moves
//      from    to      Piece type  |CaptureFlag|  (None/Castling/Promotion/en passant)    Promotion:N/B/R/Q    dppFlag (UnmakeFlag)    (captured_Piece: BP/BN/BR/BQ/BK/WP/WN/WB/WR/WQ/WK)
//                                  |Castling-side| = King-side/Queen-side
// captured pieces will be added after the move has been made in make_move()
// Piece types: {BP, BN, BB, BR, BQ, BK, WP, WN, WB, WR, WQ, WK}

extern int search_list[][256];
extern int *move_list;
extern int move_count;

// Stores additional Information about the Game state (i.e.: if castling/en passant is possible in a given position)
extern int Game_state[][2];//{ep_state, castling_rights} + the half move clock
extern int *position_state;

int search();
bool make_move(int* move);