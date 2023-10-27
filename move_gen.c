#include <stdlib.h>
#include "move_gen.h"
#include "board.h"
#include "converter.h"
#include <stdbool.h>
#include "benchmarking.h"

bool WHITE = true;
bool WK_NOT_MOVED = true;
bool BK_NOT_MOVED = true;
bool RH1_NOT_MOVED = true;
bool RA1_NOT_MOVED = true;
bool RH8_NOT_MOVED = true;
bool RA8_NOT_MOVED = true;

U64 reverse_bytes(U64 BitB){
    //reverses the bytes, not the bits, therefor reverses the rank order
    //for macOS and Linux: #include <arpa/inet.h>   return be64toh(BB);
    return _byteswap_uint64(BitB);
}
U64 reverse_rank(U64 BitB, int rank){ // BitB: Bitboard
    //reverses one rank
    U64 reversedRank = 0;
    BitB >>= rank*8;                    // shifts the rank to the first bit
    for (int i = 0; i < 8; i++) {
        reversedRank <<= 1;             // left-shifts reversedRank to make room for the next bit
        reversedRank |= (BitB & 1);     // Set the least significant bit of reversedRank to the least significant bit of BitB
        BitB >>= 1;                     // right-shifts the BitB to process the next bit
    }
    reversedRank <<= rank*8;            // shifts the rank back
    return reversedRank;
}

U64 OCCUPIED;
U64 EMPTY;
U64 ENEMY_PIECES;
U64 NOT_MY_PIECES;


// Bit: 0-5    6-11    12-15       16          17-18                                   19-20                (21)         (22-25) only for unmaking moves
//      from   to      Piece_type  CaptureFlag (None/Castling/En passant/Promotion)    Promotion:N/B/R/Q    (UnmakeFlag) (captured_Piece: BP/BN/BR/BQ/BK/WP/WN/WB/WR/WQ/WK)
// Piece types: {BP, BN, BB, BR, BQ, BK, WP, WN, WB, WR, WQ, WK}

int search_list[16][256];
int *move_list;
int move_index;
int ply;

//int game_list[16];
//int *g_move;



bool make_move(int* move){
    int from = (*move) & 63;
    int to = (*move>>6) & 63;
    int Piece = (*move>>12)&15;
    int CaptureFlag = (*move>>16)&1;
    int movetype = (*move >> 17) & 3;
    int promotion_Piece = (*move>>19)&3;
    int UnmakeFlag = (*move>>21)&1;
    int capt_Piece = (*move>>22)&15;
    U64* King = Bitboard[5+my_side*6];

    if(movetype == 0){
        // Normal Move //
        //do nothing

    }else if(movetype == 1){
    // Castling //
    // is only moving the king, the rook is moved as a regular move

        if(UnmakeFlag){
            if(CaptureFlag == 0) { //unmake king-side castling
                *King <<= 2;
            }else{ //unmake queen-side castling
                *King >>= 2;
            }
        }else {
            if(CaptureFlag == 0){ // king-side castling
                if(square_attacked(*King)|square_attacked((*King)>>1)|square_attacked((*King)>>2)){
                    // return make_move(int next_move); //CASTLING_FAILED
                }else{
                    *King >>= 2;
                    UnmakeFlag = 1; //although it is no unmake move, to avoid king-safety check
                }
            }else{// queen-side castling
                if(square_attacked(*King)|square_attacked((*King)<<1)|square_attacked((*King)<<2)){
                    // return make_move(int next_move); //CASTLING_FAILED
                }else{
                    *King <<= 2;
                    UnmakeFlag = 1; //although it is no unmake move, to avoid king-safety check

                }
            }
        }
    }else if(movetype == 2){
        // En Passant //
        printf("EN PASSANT IS NOT DEFINED YET\n");

    }else{
        // Promotions //
        *Bitboard[1 + promotion_Piece + my_side*6] ^= 1ULL<<to; //if the move is unmade the piece will be deleted again
    }

    // Captures //
    if(CaptureFlag == 1){
        if(UnmakeFlag){
            *Bitboard[capt_Piece] ^= 1ULL<<to; // puts the captured piece back
        }else{
            for(int i=op_side*6; i > 5+op_side*6; i++){
                if((*Bitboard[i])&(1ULL<<to)){
                    *Bitboard[i] ^= 1ULL<<to;
                    *move ^= (i+op_side*6)<<22;
                    break;
                }else{printf("NO PIECE TO CAPTURE BUT CAPTUREFLAG ON\n");}
            }
        }
    }else{}
    // Moving the piece //
    *Bitboard[Piece]^=(1ULL<<from)|(1ULL<<to);
    *move ^= 1<<21; //toggling the UnmakeFlag on/off
    if(UnmakeFlag){
        return true;
    }
    // King-safety //
    else{
        return square_attacked(*King);
    }
}

void move_generator(){
    ply = 0;
    move_list = &search_list[ply][0];
    move_index = 0;
    if(WHITE){
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
        EMPTY = ~(OCCUPIED);
        NOT_MY_PIECES = ~(WR|WN|WB|WQ|WK|WP|BK); // black King included to disable King captures
        ENEMY_PIECES = (BR|BN|BB|BQ|BP); // black King excluded "
        genWPawn();
        genKnight(WN);
        genBishop(WB);
        genRook(WR);
        genQueen(WQ);
        genKing(WK);
        genWCastling();
    }else{
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
        EMPTY = ~(OCCUPIED);
        NOT_MY_PIECES = ~(BR|BN|BB|BQ|BK|BP|WK); //white King included to disable King captures
        ENEMY_PIECES = (WR|WN|WB|WQ|WP); // white King excluded "
        genBPawn();
        genKnight(BN);
        genBishop(BB);
        genRook(BR);
        genQueen(BQ);
        genKing(BK);
        genBCastling();
    }
}
void genWPawn(){
    // taking the last the lsb of the WP Board and deleting it at the 'end', just like indexing
    // Capture to the right //
    U64 PMoves = (WP << 7) & (ENEMY_PIECES) & (~FileA) & (~Rank8); // (~Rank8) -> no promotion
    while (PMoves != 0){
        *move_list++ = CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7)); // search_list(to<<6+from)
        move_index++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the search_list
    }
    // 'end'
    // Capture to the left //
    PMoves = (WP << 9) & (ENEMY_PIECES) & (~FileH) & (~Rank8);
    while (PMoves != 0){
        *move_list++ = CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));
        move_index++;
        PMoves&= PMoves - 1;
    }
    // one forward //
    PMoves = (WP << 8) & (EMPTY) & (~Rank8);
    while (PMoves != 0){
        *move_list++ = WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));
        move_index++;
        PMoves&= PMoves - 1;
    }
    // two forward //
    PMoves = (WP << 16) & (EMPTY) & (EMPTY << 8) & (Rank4);
    while (PMoves != 0){
        *move_list++ = WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 16));
        move_index++;
        PMoves&= PMoves - 1;
    }
    // promotions //
    if ((WP&Rank7)!=0) {
        // promotion capture right //
        PMoves = (WP << 7) & (ENEMY_PIECES) & (Rank8) & (~FileA);
        while (PMoves != 0) {
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));move_index++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));move_index++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));move_index++;
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));move_index++;
            PMoves &= PMoves - 1;
        }
        // promotion capture left //
        PMoves = (WP << 9) & (ENEMY_PIECES) & (Rank8) & (~FileH);
        while (PMoves != 0) {
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));move_index++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));move_index++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));move_index++;
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));move_index++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward //
        PMoves = (WP << 8) & (EMPTY) & (Rank8);
        while (PMoves != 0) {
            *move_list++ = KNIGHT   + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));move_index++;
            *move_list++ = BISHOP   + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));move_index++;
            *move_list++ = ROOK     + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));move_index++;
            *move_list++ = QUEEN    + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));move_index++;
            PMoves &= PMoves - 1;
        }
    }
}
void genBPawn(){ // same as genWPawn just in opposite direction
    // Capture to the right //
    U64 PMoves = (BP >> 7) & (ENEMY_PIECES) & (~FileH) & (~Rank1); // (~Rank8) -> no promotion
    while (PMoves != 0){
        *move_list++ = CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7)); // search_list(from+to)
        move_index++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the search_list
    }
    // Capture to the left //
    PMoves = (BP >> 9) & (ENEMY_PIECES) & (~FileA) & (~Rank1);
    while (PMoves != 0){
        *move_list++ = CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));
        move_index++;
        PMoves&= PMoves - 1;
    }
    // one forward //
    PMoves = (BP >> 8) & (EMPTY) & (~Rank1);
    while (PMoves != 0){
        *move_list++ = BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));
        move_index++;
        PMoves&= PMoves - 1;
    }
    // two forward //
    PMoves = (BP >> 16) & (EMPTY) & (EMPTY >> 8) & (Rank5);
    while (PMoves != 0){
        *move_list++ = BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 16));
        move_index++;
        PMoves&= PMoves - 1;
    }
    // promotions //
    if ((WP&Rank2)!=0) { // optimized: only generates Promotion if Pawns are on the seventh Rank
        // promotion capture to the right //
        PMoves = (BP >> 7) & (ENEMY_PIECES) & (Rank1) & (~FileH);
        while (PMoves != 0) {
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));move_index++;
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));move_index++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));move_index++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));move_index++;
            PMoves &= PMoves - 1;
        }
        // promotion capture to the left //
        PMoves = (BP >> 9) & (ENEMY_PIECES) & (Rank1) & (~FileA); // &(~FileA) cannot be excluded
        while (PMoves != 0) {
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));move_index++;
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));move_index++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));move_index++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));move_index++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward //
        PMoves = (BP >> 8) & (EMPTY) & (Rank1);
        while (PMoves != 0) {
            *move_list++ = QUEEN    + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));move_index++;
            *move_list++ = KNIGHT   + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));move_index++;
            *move_list++ = BISHOP   + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));move_index++;
            *move_list++ = ROOK     + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));move_index++;
            PMoves &= PMoves - 1;
        }
    }
}
U64 genDiagonal(U64 DMP){ // DMP: Diagonal Moving Piece
    int i = lsb(DMP);
    // Diagonal Moves //
    U64 m = SD_mask[i / 8 + i % 8];
    U64 diagonalMoves = (( (OCCUPIED&m) - 2*DMP ) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(DMP))) & m;
    // Anti-diagonal Moves //
    m = AD_mask[7 + i / 8 - i % 8];
    diagonalMoves |= (((OCCUPIED & m) - 2 * DMP) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(DMP))) & m;
    return diagonalMoves;
}
U64 genStraight(U64 SMP){ // SMP: Straight Moving Piece
    int i = lsb(SMP);
    // Vertical Moves //
    U64 m = V_mask[7 - i % 8];
    U64 straightMoves = (((OCCUPIED&m) - 2*SMP ) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(SMP))) & m;
    // Horizontal Moves //
    int rank = i/8;
    m = H_mask[rank];
    straightMoves |= (( (OCCUPIED) - 2*SMP)^ reverse_rank(reverse_rank(OCCUPIED, rank) - 2*reverse_rank(SMP, rank), rank))&m;
    return straightMoves;
}
U64 genKnightSpan(U64 NMP){ // KMP: Night Moving Piece
    U64 NMoves;
    int from = lsb(NMP);
    if(from>=17){
        NMoves = KnightSpan<<(from-18);
    }else{
        NMoves = KnightSpan>>(18-from);
    }if(from%8<4){
        NMoves &= (~File_AB);
    }else{
        NMoves &= (~File_GH);
    }
    return NMoves;
}
U64 genKingSpan(U64 KMP){ // KMP: King Moving Piece
    int from = lsb(KMP);
    U64 KMoves;
    if(from>=9){
        KMoves = KingSpan<<(from-9);
    }else{
        KMoves = KingSpan>>(9-from);
    }if(from%8<4){
        KMoves &=(~FileA);
    }else{
        KMoves &=(~FileH);
    }
    return KMoves;
}
void genRook(U64 RBB){ // RBB = Rook BitBoard
    int color = (WHITE)?WHITEROOK:BLACKROOK;
    U64 RMoves;
    U64 move;
    U64 R = RBB&~(RBB-1);   // takes the LSB from RBB
    while (R != 0) {
        int from = lsb(R);
        RMoves = genStraight(R) & NOT_MY_PIECES;
        move = RMoves&~(RMoves-1);      // takes the LSB from RMoves

        while(move != 0){
            *move_list++ =  ((move&OCCUPIED)?CAPTURE:NONE) + color + (lsb(move) << 6) + from;
            move_index++;
            RMoves &= ~move;            // deletes the LSB from RMoves
            move = RMoves&~(RMoves-1);  // takes next LSB from RMoves
        }
        RBB &= ~R;          // deletes the LSB from RBB
        R = RBB&~(RBB-1);   // takes next LSB from RBB
    }
}
void genBishop(U64 BBB){ // BBB = Bishop BitBoard, same as genRook()
    int color = (WHITE)?WHITEBISHOP:BLACKBISHOP;
    U64 BMoves;
    U64 move;
    U64 B = BBB & ~(BBB - 1);
    while (B != 0) {
        int from = lsb(B);
        BMoves = genDiagonal(B) & NOT_MY_PIECES;
        move = BMoves&~(BMoves-1);
        while(move != 0){
            *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + color + (lsb(move) << 6) + from;
            move_index++;
            BMoves &= ~move;
            move = BMoves&~(BMoves-1);
        }
        BBB &= ~B;
        B = BBB & ~(BBB - 1);
    }
}
void genQueen(U64 QBB){ // QBB = Queen BitBoard, same as genRook()
    int color = (WHITE)?WHITEQUEEN:BLACKQUEEN;
    U64 QMoves;
    U64 move;
    U64 Q = QBB & ~(QBB - 1);
    while (Q != 0) {
        int from = lsb(Q);
        QMoves = (genStraight(Q)|genDiagonal(Q)) & NOT_MY_PIECES;
        move = QMoves & ~(QMoves - 1);
        while(move != 0){
            *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + color + (lsb(move) << 6) + from;
            move_index++;
            QMoves &= ~move;
            move = QMoves & ~(QMoves - 1);
        }
        QBB &= ~Q;
        Q = QBB & ~(QBB - 1);
    }
}
void genKnight(U64 NBB){ // NBB = kNight BitBoard
    int color = (WHITE)?WHITEKNIGHT:BLACKKNIGHT;
    U64 NMoves;
    U64 move;
    U64 N = NBB & ~(NBB - 1);
    while (N != 0) {
        int from = lsb(N);
        NMoves = genKnightSpan(N) & NOT_MY_PIECES;
        move = NMoves & ~(NMoves-1);
        while(move != 0){
            *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + color + (lsb(move) << 6) + from;
            move_index++;
            NMoves &= ~move;
            move = NMoves & ~(NMoves-1);
        }
        NBB &= ~N;
        N = NBB & ~(NBB-1);
    }
}
void genKing(U64 KBB){ // KBB = King BitBoard
    int color = (WHITE)?WHITEKING:BLACKKING;
    U64 KMoves = genKingSpan(KBB) & NOT_MY_PIECES;
    U64 move = KMoves & ~(KMoves-1);
    int from = lsb(KBB);
    while(move != 0){
        *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + color + (lsb(move) << 6) + from;
        move_index++;
        KMoves &= ~move;
        move = KMoves & ~(KMoves-1);
    }
}
void genWCastling(){
    if(WK_NOT_MOVED){
        if(RH1_NOT_MOVED & ((OCCUPIED&F1G1) == 0)){
            *move_list++ = CASTLING + WHITEROOK + (2<<6) + 0;
            move_index++;
        }else{}
        if(RA1_NOT_MOVED & ((OCCUPIED&B1C1D1) == 0)){
            *move_list++ = CASTLING + WHITEROOK + (4<<6) + 7;
            move_index++;
        }else{}
    }else{}
}
void genBCastling(){
    if(BK_NOT_MOVED){
        if(RH8_NOT_MOVED & ((OCCUPIED&F8G8) == 0)){
            *move_list++ = CASTLING + BLACKROOK + (58<<6) + 56;
            move_index++;
        }else{}
        if(RA8_NOT_MOVED & ((OCCUPIED&B8C8D8) == 0)){
            *move_list++ = CASTLING + BLACKROOK + (60<<6) + 63;
            move_index++;
        }else{}
    }else{}
}
bool square_attacked(U64 square){
    // Queen - Rook
    U64 straight_attacks = genStraight(square);
    if(straight_attacks & ((*Bitboard[3 + op_side * 6]) | (*Bitboard[4 + op_side * 6]))) {
        return true;
    }else{}
    // Queen - Bishop
    U64 diagonal_attacks = genDiagonal(square);
    if(diagonal_attacks & ((*Bitboard[2 + op_side * 6]) | (*Bitboard[4 + op_side * 6]))){
        return true;
    }else{}
    // Knight
    U64 knight_attacks = genKnightSpan(square);
    if(knight_attacks & (*Bitboard[1 + op_side * 6])){
        return true;
    }else{}
    // Pawn
    U64 pawn_attacks = (my_side) ? (square << 7) | (square << 9) : (square >> 7) | (square >> 9);
    if(pawn_attacks & (*Bitboard[op_side * 6])){
        return true;
    }else{}
    // King
    U64 king_attacks = genKingSpan(square);
    if(king_attacks & (*Bitboard[5 + op_side * 6])){
        return true;
    }else{}
    return false;
}