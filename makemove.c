#include "move_gen.h"
#include "search.h"
#include "board.h"

#define QueensideFlag CaptureFlag

// castling rights update constants // by Code Monkey King's -> https://www.youtube.com/@chessprogramming591 //
/*
                           castling   move     in      in
                              right update     binary  decimal

 king & rooks didn't move:     1111 & 1111  =  1111    15

        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13

         black king moved:     1111 & 0011  =  0011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7

*/
const int8_t castling_rights[64] = {
        14, 15, 15, 12, 15, 15, 15, 13,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        11, 15, 15,  3, 15, 15, 15, 7,
};

/// Storing Moves inside the move_list[256] ///
/// 32 bits - one integer - is more than enough to store all the important data of a move
/// Bit: 0-5     6-11    12-15       16                         17-18                   19-20       21
///      from    to      Piece type  |CaptureFlag|              Move type:              Promotion:  dppFlag
///      square  square              |Castling-side|            (Normal/Castling/       N/B/R/Q     double pawn push flag
///                                  (King-side/Queen-side)     En passant/Promotion)
///
/// Piece types: {BP, WP, BN, WN, BB, WB, BR, WR, BQ, WQ, BK, WK}

bool make_move(int move){
    int8_t from = move & 63;
    int8_t to = (move>>6) & 63;
    int8_t Piece = (move>>12)&15;
    bool CaptureFlag = (move>>16)&1;
    int8_t movetype = (move>>17)&3;
    int8_t promotion_Piece = (move>>19)&3;
    bool dppFlag = (move>>21)&1;

    U64* King = Bitboard[10+my_side];


    if(movetype == 0){
        // Normal Move //

    }else if(movetype == 1){
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);//updates occupancies
        // Castling //
        if(QueensideFlag == 0){ //CaptureFlag == 0 -> King-side castling, since a castling move can not be a capture
            // King-side Castling //
            if(square_attacked(*King)||square_attacked((*King)>>1)||square_attacked((*King)>>2)){
                return false;
            }else{
                *King >>= 2;
                *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
                CASTLING_RIGHTS &= (WHITE)?12:3;
                //perftCastleCounter++; // for perft
                Checkmate = false; // a legal move has been made
                return true;

            }
        }else{
            // Queen-side Castling //
            if(square_attacked(*King)||square_attacked((*King)<<1)||square_attacked((*King)<<2)){
                return false;
            }else{
                *King <<= 2;
                *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
                CASTLING_RIGHTS &= (WHITE)?12:3;
                //perftCastleCounter++; // for perft
                Checkmate = false; // a legal move has been made
                return true;

            }
        }
    }else if(movetype == 2){
        // En Passant //
        *Bitboard[op_side] ^= 1ULL << (to-SIGN*8); // delete the en passant pawn
        //OCCUPIED ^= 1ULL << (to-(my_side*2-1)*8);
    }else{
        // Promotions //
        *Bitboard[Piece] ^= 1ULL<<to; // add the pawn, causing delete when moved
        *Bitboard[(2*promotion_Piece)+2+my_side] ^= 1ULL<<to; // add the promotion_Piece

    }
    if(CaptureFlag){
        // Captures //
        for(int i=op_side; i < 12; i+=2){
            if(((*Bitboard[i])&(1ULL<<to))!=0){
                *Bitboard[i] ^= 1ULL<<to;
                //OCCUPIED ^= 1ULL<<to;
                break;
            }
        }
    }
    // Moving the piece //
    *Bitboard[Piece] ^= (1ULL<<from)|(1ULL<<to);
    //OCCUPIED ^=(1ULL<<from)|(1ULL<<to);


    // LEGALITY CHECK //

    // updating the occupied squares
    OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);

    // Checking weather the King is in Check.
    if(square_attacked(*King))
        return false;

    //the move is now legal//
    else{

        // Update Castling-Rights //
        CASTLING_RIGHTS &= castling_rights[from];
        CASTLING_RIGHTS &= castling_rights[to];

        // Double Pawn Pushes // for en passant rights
        if(dppFlag){
            EP_STATE = true;
            EP_PAWN = to;
        }

        // perft //
        /*
        if(CaptureFlag){
            perftCaptCounter++;
        }
        if(movetype==2){
            perftEPCounter++;
        }
        if(movetype==3){
            perftPromotionCounter++;
        }*/

        // setting checkmate to false, since a legal move has been made
        Checkmate = false;
        return true;
    }
}