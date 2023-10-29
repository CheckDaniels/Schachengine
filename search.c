#include <stdio.h>
#include "board.h"
#include "move_gen.h"
#include "search.h"
#define ARook_moved ((Piece==3+6*my_side)&&(from==7+56*op_side)) || ((capt_Piece==3+6*op_side)&&(from==7+56*op_side))
#define HRook_moved ((Piece==3+6*my_side)&&(from==56*op_side)) || ((capt_Piece==3+6*op_side)&&(from==56*op_side))
#define QueensideFlag CaptureFlag

bool WHITE;

int perftNodeCounter = 0;
int perftCaptCounter = 0;
int perftEPCounter = 0;
int perftCastleCounter = 0;
int perftMateCounter = 0;

/*int COUNTER = 0;
int COUNTERLIST[50];
int x = 0;*/

// Bit: 0-5    6-11    12-15       16          17-18                                   19-20                21      (22)            (23-26) only for unmaking moves
//      from   to      Piece_type  CaptureFlag (None/Castling/En passant/Promotion)    Promotion:N/B/R/Q    dppFlag (UnmakeFlag)    (captured_Piece: BP/BN/BR/BQ/BK/WP/WN/WB/WR/WQ/WK)
// Piece types: {BP, BN, BB, BR, BQ, BK, WP, WN, WB, WR, WQ, WK}
int search_list[MAX_PLY+1][256];
int *move_list;
int pseudolegal_move_count;
int counter_list[MAX_PLY+1];
bool Checkmate_list[MAX_PLY+1];
bool* Checkmate = &Checkmate_list[MAX_PLY];

struct Game_Status game_list[MAX_PLY+3];
struct Game_Status *game_state;



int search(int ply) {
    WHITE^=1;
    Checkmate--;
    if (ply==0){
        perftNodeCounter++;
    }else {
        game_state = &game_list[ply]; // game_state->ep_state->castling_rights->
        move_list = &search_list[ply][0];
        pseudolegal_move_count = 0;
        move_generator();
        counter_list[ply] = pseudolegal_move_count;
        *Checkmate = true;
        for (int i = 0; i < counter_list[ply]; i++) {
            if (make_move(&search_list[ply][i])){
                //printBoard();
                //move_to_string(search_list[ply][i]);
                //COUNTER++;
                search(ply-1);
            }
            unmake_move(&search_list[ply][i]);
            //if (ply == MAX_PLY){move_to_string(search_list[ply][i]);}
        }
        if(*Checkmate){
            //printf("CHECK_MATE\n");
            perftMateCounter++;
        }
        /*
        COUNTERLIST[x++] = COUNTER;
        printf("DEPTH HAS CHANGED ");
        printf("%d\n", COUNTER);
        COUNTER = 0;
         */

        //printf("\n");
        //printBoard();
    }
    Checkmate++;
    WHITE^=1;
    game_state = &game_list[ply]; // game_state->ep_state->castling_rights
    return 0;
}
bool make_move(int* move){

    int from = (*move) & 63;
    int to = (*move>>6) & 63;
    int Piece = (*move>>12)&15;
    int CaptureFlag = (*move>>16)&1;
    int movetype = (*move>>17)&3;
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
            if(QueensideFlag == 0) {
                *King <<= 2;//unmake king-side castling
            }else{
                *King >>= 2;//unmake queen-side castling
                QueensideFlag = 0; //so it is not counted as a capture later
            }
        }else {
            if(QueensideFlag == 0){ //CaptureFlag == 0 -> King-side castling, since a castling move can not be a capture
                // King-side Castling //
                if(square_attacked(*King)||square_attacked((*King)>>1)||square_attacked((*King)>>2)){
                    *King >>= 2;
                    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
                    *move ^= 1<<22; // turns on UnmakeFlag
                    return false;
                }else{
                    *King >>= 2;
                    UnmakeFlag = 1; //although it is no unmake move, it is to avoid king-safety check
                    game_state->castling_rights = 0;
                }
            }else{
                // Queen-side Castling //
                if(square_attacked(*King)||square_attacked((*King)<<1)||square_attacked((*King)<<2)){
                    *King <<= 2;
                    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
                    *move ^= 1<<22;
                    return false;
                }else{
                    *King <<= 2;
                    QueensideFlag = 0; //so it is not counted as a capture later
                    UnmakeFlag = 1; //although it is no unmake move, to skip king-safety check
                    game_state->castling_rights = 0;

                }
            }
        }
    }else if(movetype == 2){
        // En Passant //
        game_state++;
        *Bitboard[op_side*6] ^= 1ULL << ((game_state->ep_file)+24+8*my_side);game_state--; //captures the piece
    }else{
        // Promotions //
        *Bitboard[1 + promotion_Piece + my_side*6] ^= 1ULL<<to; //if the move is unmade the piece will be deleted again
        *Bitboard[Piece]^=(1ULL<<to);
    }
    if(dppFlag){
        // Double Pawn Pushes //
        game_state->ep_state ^= 1;
        game_state->ep_file ^= from%8;
        //*move ^= 1<<21; //turns off the dppFlag
    }else if(CaptureFlag == 1){
        // Captures //
        if(UnmakeFlag){
            *Bitboard[capt_Piece] ^= 1ULL<<to; // puts the captured piece back
        }else{
            for(int i=op_side*6; i < 5+op_side*6; i++){
                if(((*Bitboard[i])&(1ULL<<to))!=0){
                    *Bitboard[i] ^= 1ULL<<to;
                    *move ^= i<<23;
                    break;
                }
            }
        }
    }else{}

    // Moving the piece //
    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
    *move ^= 1<<22; //toggling the UnmakeFlag on/off
    if(UnmakeFlag){
        return true;
    }else{
        // King-safety // checking for legal moves
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
        if(square_attacked(*King)){
            return false;
        }else{
            // Losing Castling-Rights // now after the legal move check it can be checked weather castling rights are preserved or need to be changed?
            game_state+=2;   //game_state--next move    ++last move
            if((game_state->castling_rights == 0) || Piece==5+6*my_side){
                (game_state-=2)->castling_rights = 0; game_state+=2;
            }else if(game_state->castling_rights == 1){
                if(ARook_moved) {
                    (game_state-=2)->castling_rights = 0; game_state+=2;
                }else{
                    (game_state-=2)->castling_rights = 1; game_state+=2;
                }
            }else if(game_state->castling_rights == 2){
                if(HRook_moved){
                    (game_state-=2)->castling_rights = 0; game_state+=2;
                }else{
                    (game_state-=2)->castling_rights = 2; game_state+=2;
                }
            }else if(ARook_moved){
                (game_state-=2)->castling_rights = 1; game_state+=2;
            }else if(HRook_moved){
                (game_state-=2)->castling_rights = 2; game_state+=2;
            }
            //all castling rights preserved
            game_state-=2;

            // setting checkmate to false
            *Checkmate = false;
            return true;
        }

    }
}
//void copy_board();
//void take_back();