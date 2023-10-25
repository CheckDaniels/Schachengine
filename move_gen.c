#include <stdlib.h>
#include "move_gen.h"
#include "board.h"
#include "converter.h"
#include "benchmarking.h"

U64 reverse_bytes(U64 bit){
    //reverses the bytes, not the bits. Therefor reverses the rank order
    //for MacOS and Linux: #include <arpa/inet.h>   be64toh
    return _byteswap_uint64(bit);
}
U64 reverse_rank(U64 bit, int rank){
    //reverses one rank
    U64 reversedRank = 0;
    bit >>= rank*8;
    for (int i = 0; i < 8; i++) {
        reversedRank <<= 1;          // Left-shift reversedValue to make room for the next bit
        reversedRank |= (bit & 1);   // Set the least significant bit of reversedValue based on the least significant bit of value
        bit >>= 1;                   // Right-shift value to process the next bit
    }
    reversedRank <<= rank*8;
    return reversedRank;
}
U64 OCCUPIED;
U64 EMPTY;
U64 ENEMY_PIECES;
U64 NOT_MY_PIECES;

int movelist[256];
int listindex=0;

void makemove(int move){

}
void move_notation(int move){
    int from = move&MASK_FROM;
    int to = (move&MASK_TO)>>6;
    int promotion = (move&MASK_PROMOTION)>>12;
    int movetype = (move&MASK_MOVETYPE)>>14;

    char file_from = (7-from%8)+97;
    char rank_from = (from/8)+49;
    char file_to = (7-to%8)+97;
    char rank_to = (to/8)+49;

    char promoted_to;
    char type_of_move;
    switch(movetype){
        case 0: type_of_move = 'N'; break;
        case 1: type_of_move = 'E'; break;
        case 2: type_of_move = 'P';
            switch(promotion){
                case 0: promoted_to = 'q'; break;
                case 1: promoted_to = 'n'; break;
                case 2: promoted_to = 'b'; break;
                case 3: promoted_to = 'r'; break;
            }break;
        case 3: type_of_move = 'C'; break;
    }
    char notated_move[6];
    sprintf(notated_move,"%c%c%c%c%c%c",file_from, rank_from, file_to, rank_to, promoted_to, type_of_move);
    printf("%s ",notated_move);
}

void move_generator(int COLOR){
    if(COLOR == WHITE){
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
        EMPTY = ~(OCCUPIED);
        NOT_MY_PIECES = ~(WR|WN|WB|WQ|WK|WP|BK); // black King included to disable King captures
        ENEMY_PIECES = (BR|BN|BB|BQ|BP); // black King excluded "
        listindex=0;
        genRook(WR);
        genKnight(WN);
        genBishop(WB);
        genQueen(WQ);
        genKing(WK);
        genWPawn();

    }else if(COLOR == BLACK){
        OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
        EMPTY = ~(OCCUPIED);
        NOT_MY_PIECES = ~(BR|BN|BB|BQ|BK|BP|WK); //white King included to disable King captures
        ENEMY_PIECES = (WR|WN|WB|WQ|WP); // white King excluded "
        listindex=0;
        genRook(BR);
        genKnight(BN);
        genBishop(BB);
        genQueen(BQ);
        genKing(BK);
        genBPawn();
    }else{
        printf("THE COLOR IS MISSING");
    }
}
void genWPawn(){
    // taking the last the lsb of the WP Board and deleting it at the 'end', just like indexing
    // Capture to the right
    U64 PMoves = (WP << 7) & (ENEMY_PIECES) & (~FileA) & (~Rank8); // (~Rank8) -> no promotion
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 7)); // movelist(to<<6+from)
        listindex++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the movelist
    }
    // 'end'
    // Capture to the left
    PMoves = (WP << 9) & (ENEMY_PIECES) & (~FileH) & (~Rank8);
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 9));
        listindex++;
        PMoves&= PMoves - 1;
    }
    // one forward
    PMoves = (WP << 8) & (EMPTY) & (~Rank8);
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 8));
        listindex++;
        PMoves&= PMoves - 1;
    }
    // two forward
    PMoves = (WP << 16) & (EMPTY) & (EMPTY << 8) & (Rank4);
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 16));
        listindex++;
        PMoves&= PMoves - 1;
    }
    // generating promotions
    if ((WP&Rank7)!=0) {
        // promotion capture right
        PMoves = (WP << 7) & (ENEMY_PIECES) & (Rank8) & (~FileA);
        while (PMoves != 0) {
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 7) +QUEEN+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 7) +KNIGHT+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 7) +BISHOP+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 7) +ROOK+ PROMOTION); listindex++;
            PMoves &= PMoves - 1;
        }
        // promotion capture left
        PMoves = (WP << 9) & (ENEMY_PIECES) & (Rank8) & (~FileH);
        while (PMoves != 0) {
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 9) +QUEEN+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 9) +KNIGHT+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 9) +BISHOP+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 9) +ROOK+ PROMOTION); listindex++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward
        PMoves = (WP << 8) & (EMPTY) & (Rank8);
        while (PMoves != 0) {
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 8) +QUEEN+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 8) +KNIGHT+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 8) +BISHOP+ PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves >> 8) +ROOK+ PROMOTION); listindex++;
            PMoves &= PMoves - 1;
        }
    }
    /*
    int *movelist;
    // Allocate memory for an array of 256 integers
    movelist = (int*)malloc(256 * sizeof(int));
    *movelist++ =

    if (dynamicArray == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    free(movelist);
    */
}
void genBPawn(){ // same as genWPawn just in opposite direction
    // Capture to the right
    U64 PMoves = (BP >> 7) & (ENEMY_PIECES) & (~FileH) & (~Rank1); // (~Rank8) -> no promotion
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 7)); // movelist(from+to)
        listindex++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the movelist
    }
    // Capture to the left
    PMoves = (BP >> 9) & (ENEMY_PIECES) & (~FileA) & (~Rank1);
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 9));
        listindex++;
        PMoves&= PMoves - 1;
    }
    // one forward
    PMoves = (BP >> 8) & (EMPTY) & (~Rank1);
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 8));
        listindex++;
        PMoves&= PMoves - 1;
    }
    // two forward
    PMoves = (BP >> 16) & (EMPTY) & (EMPTY >> 8) & (Rank5);
    while (PMoves != 0){
        movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 16));
        listindex++;
        PMoves&= PMoves - 1;
    }
    // generating promotions
    if ((WP&Rank2)!=0) { // optimized: only generates Promotion if Pawns are on the seventh Rank
        // promotion capture to the right
        PMoves = (BP >> 7) & (ENEMY_PIECES) & (Rank1) & (~FileH);
        while (PMoves != 0) {
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 7) +QUEEN+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 7) +KNIGHT+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 7) +BISHOP+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 7) +ROOK+PROMOTION); listindex++;
            PMoves &= PMoves - 1;
        }
        // promotion capture to the left
        PMoves = (BP >> 9) & (ENEMY_PIECES) & (Rank1) & (~FileA); // &(~FileA) cannot be excluded
        while (PMoves != 0) {
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 9) +QUEEN+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 9) +KNIGHT+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 9) +BISHOP+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 9) +ROOK+PROMOTION); listindex++;
            listindex++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward
        PMoves = (BP >> 8) & (EMPTY) & (Rank1);
        while (PMoves != 0) {
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 8) +QUEEN+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 8) +KNIGHT+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 8) +BISHOP+PROMOTION); listindex++;
            movelist[listindex] = (lsb(PMoves) << 6) + (lsb(PMoves << 8) +ROOK+PROMOTION); listindex++;
            listindex++;
            PMoves &= PMoves - 1;
        }
    }
}
U64 genDiagonal(U64 DMP){ // DP: Diagonal Moving Piece
    int i = lsb(DMP);
    // Diagonal Moves
    U64 m = dMask[i/8+i%8];
    U64 diagonalMoves = (( (OCCUPIED&m) - 2*DMP ) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(DMP))) & m;
    // Anti-diagonal Moves
    m = aMask[7+i/8-i%8];
    diagonalMoves |= (((OCCUPIED & m) - 2 * DMP) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(DMP))) & m;
    return diagonalMoves;
}
U64 genStraight(U64 SMP){ // SP: Straight Moving Piece
    int i = lsb(SMP);
    // Vertical Moves //
    U64 m = vMask[7-i%8];
    U64 straightMoves = (((OCCUPIED&m) - 2*SMP ) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(SMP))) & m;
    // Horizontal Moves //
    int rank = i/8;
    m = hMask[rank];
    straightMoves |= (( (OCCUPIED) - 2*SMP)^ reverse_rank(reverse_rank(OCCUPIED, rank) - 2*reverse_rank(SMP, rank), rank))&m;
    return straightMoves;
}
void genRook(U64 RBB){ // RBB = Rook BitBoard
    U64 RMoves;
    U64 move;
    U64 R = RBB&~(RBB-1);
    while (R != 0) {
        int from = lsb(R);
        RMoves = genStraight(R)&NOT_MY_PIECES;
        move = RMoves&~(RMoves-1);
        while(move != 0){
            movelist[listindex] = (lsb(move)<<6)+from;
            listindex++;
            RMoves &= ~move;            //delete move from RMoves
            move = RMoves&~(RMoves-1);  //take next move from RMoves
        }
        RBB &= ~R;
        R = RBB&~(RBB-1);
    }
}
void genBishop(U64 BBB){ // BBB = Bishop BitBoard
    U64 BMoves;
    U64 move;
    U64 B = BBB & ~(BBB - 1);
    while (B != 0) {
        int from = lsb(B);
        BMoves = genDiagonal(B) & NOT_MY_PIECES;
        move = BMoves&~(BMoves-1);
        while(move != 0){
            movelist[listindex] = (lsb(move)<<6)+from;
            listindex++;
            BMoves &= ~move;            //delete move from RMoves
            move = BMoves&~(BMoves-1);  //take next move from RMoves
        }
        BBB &= ~B;
        B = BBB & ~(BBB - 1);
    }
}
void genQueen(U64 QBB){ // QBB = Rook BitBoard
    U64 QMoves;
    U64 move;
    U64 Q = QBB & ~(QBB - 1);
    while (Q != 0) {
        int from = lsb(Q);
        QMoves = (genStraight(Q)|genDiagonal(Q))& NOT_MY_PIECES;
        move = QMoves & ~(QMoves - 1);
        while(move != 0){
            movelist[listindex] = (lsb(move)<<6)+from;
            listindex++;
            QMoves &= ~move;                //delete move from QMoves
            move = QMoves & ~(QMoves - 1);  //take next move from QMoves
        }
        QBB &= ~Q;
        Q = QBB & ~(QBB - 1);
    }
}
void genKnight(U64 NBB){ // NBB = kNight BitBoard
    U64 NMoves;
    U64 move;
    U64 N = NBB & ~(NBB - 1);
    while (N != 0) {
        int from = lsb(N);
        if(from>=17){
            NMoves = KnightSpan<<(from-18);
        }else{
            NMoves = KnightSpan>>(18-from);
        }if(from%8<4){
            NMoves &= (~File_AB) & NOT_MY_PIECES;
        }else{
            NMoves &= (~File_GH) & NOT_MY_PIECES;
        }
        move = NMoves & ~(NMoves-1);
        while(move != 0){
            movelist[listindex] = (lsb(move)<<6)+from;
            listindex++;
            NMoves &= ~move;              //delete move from NMoves
            move = NMoves & ~(NMoves-1);  //take next move from NMoves
        }
        NBB &= ~N;
        N = NBB & ~(NBB-1);
    }
}
void genKing(U64 KBB){ // need tests //
    U64 KMoves;
    U64 move;
    U64 K = KBB & ~(KBB - 1);
    while (K != 0) { // incase I want to implement multiple Kings, idn
        int from = lsb(K);
        if(from>=9){
            KMoves = KingSpan<<(from-9);
        }else{
            KMoves = KingSpan>>(9-from);
        }if(from%8<4){
            KMoves &=(~FileA)&NOT_MY_PIECES;
        }else{
            KMoves &=(~FileH)&NOT_MY_PIECES;
        }
        move = KMoves & ~(KMoves-1);
        while(move != 0){
            movelist[listindex] = (lsb(move)<<6)+from;
            listindex++;
            KMoves &= ~move;              //delete move from NMoves
            move = KMoves & ~(KMoves-1);  //take next move from NMoves
        }
        KBB &= ~K;
        K = KBB & ~(KBB-1);
    }
}