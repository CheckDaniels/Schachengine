#pragma once
#define my_side WHITE
#define op_side (1-WHITE)
#define unmake_move(x) make_move(x)
#define MAX_PLY 3

extern int perftNodeCounter;
extern int perftCaptCounter;
extern int perftEPCounter;
extern int perftCastleCounter;
extern int perftMateCounter;

//extern int COUNTERLIST[];
//extern int x;

extern bool WHITE;

//captured pieces, en passant, castling-rights and fifty-move rule//
// int search_list[][]; Stores the generated moves for each ply inside an 2D array in int format
// 32 bits are enough to store all the important data of a move
// Bit: 0-5     6-11    12-15       16             17-18                                   19-20                21      (22)            (23-26) only for unmaking moves
//      from    to      Piece type  |CaptureFlag|  (None/Castling/en passant/Promotion)    Promotion:N/B/R/Q    dppFlag (UnmakeFlag)    (captured_Piece: BP/BN/BR/BQ/BK/WP/WN/WB/WR/WQ/WK)
//                                  |Castling-side| = King-side/Queen-side
// captured pieces will be added after the move has been made in make_move()
// Piece types: {BP, BN, BB, BR, BQ, BK, WP, WN, WB, WR, WQ, WK}

extern int search_list[][256];
extern int *move_list;
extern int pseudolegal_move_count;
extern int counter_list[];

// Stores additional Information about the Game state (i.e.: if castling/en passant is possible in a given position)
// {ep_state, castling rights, half move clock, hash}
// ep_state: 8Bit
// castling_rights: 8 bits
// half move clock: 8 bits
// hash: 64 bit
struct Game_Status{
    bool ep_state;
    int ep_file;
    int castling_rights; // 0 Castling false, 1 KS-Castling false, 2 QS-Castling false, 3 Castling true
    //char half_move_clock;
};
extern struct Game_Status game_list[];
extern struct Game_Status *game_state;


int search(int ply);
bool make_move(int* move);
//void copy_board();
//void take_back();