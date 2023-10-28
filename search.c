#include <stdio.h>
#include "board.h"
#include "move_gen.h"
#include "converter.h"
#include "benchmarking.h"
#include "search.h"

const int MAX_PLY = 2;
bool WHITE = false;

int ply = MAX_PLY+1;

// Bit: 0-5    6-11    12-15       16          17-18                                   19-20                21      (22)            (23-26) only for unmaking moves
//      from   to      Piece_type  CaptureFlag (None/Castling/En passant/Promotion)    Promotion:N/B/R/Q    dppFlag (UnmakeFlag)    (captured_Piece: BP/BN/BR/BQ/BK/WP/WN/WB/WR/WQ/WK)
// Piece types: {BP, BN, BB, BR, BQ, BK, WP, WN, WB, WR, WQ, WK}
int search_list[16][256];
int *move_list;
int move_count;

int Game_state[16][2];//{ep_state, castling_rights}and the halfmove clock
int *position_state;

int search() {
    ply--;
    if (ply==0){
        //do nothing
    }else {
        move_generator();
        //int *position_state = &Game_state[ply][0];
        for (int i = 0; i < move_count; i++) {
            move_to_string(search_list[ply][i]);
            if (make_move(&search_list[ply][i])){
                printBoard();
                WHITE ^= 1;
                search();
                WHITE ^= 1;
            }else{}
            unmake_move(&search_list[ply][i]);
        }
        printf("\n");
        printBoard();
    }
    ply++;
    return 1;
}
bool make_move(int* move){

    int from = (*move) & 63;
    int to = (*move>>6) & 63;
    int Piece = (*move>>12)&15;
    int CaptureFlag = (*move>>16)&1;
    int movetype = (*move >> 17) & 3;
    int promotion_Piece = (*move>>19)&3;
    int dppFlag = (*move>>21)&1;
    int UnmakeFlag = (*move>>22)&1;
    int capt_Piece = (*move>>23)&15;
    U64* King = Bitboard[5+my_side*6];

    if(movetype == 0){
        // Normal Move //

    }else if(movetype == 1){
        // Castling // is only moving the king, the rook is moved as a regular move
        if(UnmakeFlag){
            if(CaptureFlag == 0) {
                *King <<= 2;//unmake king-side castling
            }else{
                *King >>= 2;//unmake queen-side castling
            }
        }else {

            if(CaptureFlag == 0){ //CaptureFlag == 0 -> King-side castling, since a castling move can not be a capture
                // King-side Castling //
                if(square_attacked(*King)||square_attacked((*King)>>1)||square_attacked((*King)>>2)){
                    *King >>= 2;
                    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
                    *move ^= 1<<22;
                    printf("IN_CHECK\n");
                    return false;
                }else{
                    *King >>= 2;
                    UnmakeFlag = 1; //although it is no unmake move, to avoid king-safety check
                }
            }else{
                // Queen-side Castling //
                if(square_attacked(*King)||square_attacked((*King)<<1)||square_attacked((*King)<<2)){
                    *King <<= 2;
                    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
                    *move ^= 1<<22;
                    printf("IN_CHECK\n");
                    return false;
                }else{
                    *King <<= 2;
                    UnmakeFlag = 1; //although it is no unmake move, to skip king-safety check

                }
            }
        }
    }else if(movetype == 2){
        // En Passant //
        *Bitboard[op_side*6] ^= 1ULL << (doublePawnpushFile+24+8*my_side);

    }else{
        // Promotions //
        *Bitboard[1 + promotion_Piece + my_side*6] ^= 1ULL<<to; //if the move is unmade the piece will be deleted again
    }

    if(dppFlag){
        // Double Pawn Pushes //
        Enpassant_possibility = true;
        doublePawnpushFile = from%8;
        *move ^= 1<<21; //turns off the dppFlag
    }if(CaptureFlag == 1){
        // Captures //
        if(UnmakeFlag){
            *Bitboard[capt_Piece] ^= 1ULL<<to; // puts the captured piece back
        }else{
            for(int i=op_side*6; i > 5+op_side*6; i++){
                if((*Bitboard[i])&(1ULL<<to)){
                    *Bitboard[i] ^= 1ULL<<to;
                    *move ^= (i+op_side*6)<<23;
                    break;
                }else{printf("NO PIECE TO CAPTURE BUT CAPTURE-FLAG ON\n");}
            }
        }
    }else{}

    /// Moving the piece ///
    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
    *move ^= 1<<22; //toggling the UnmakeFlag on/off
    if(UnmakeFlag){
        return true;
    }else{
        // King-safety //
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
        if(square_attacked(*King)){
            printf("IN_CHECK\n");
            return false;
        }else{
            return true;
        }

    }
}