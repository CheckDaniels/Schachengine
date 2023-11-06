#include "move_gen.h"
#include "board.h"
#include "converter.h"
#include "search.h"

U64 OCCUPIED;
U64 EMPTY;
U64 ENEMY_PIECES;
U64 NOT_MY_PIECES;

static int move_counter;
static int* move_list;

void move_generator(int* movelist_ptr, int* move_count){
    move_counter = 0;
    move_list = movelist_ptr;
    OCCUPIED = (BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP);
    EMPTY = ~(OCCUPIED);
    if(WHITE){
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
    *move_count = move_counter;
}
void genWPawn(){
    // taking the last the lsb of the WP Board and deleting it at the 'end', just like indexing
    // Capture to the right //
    U64 PMoves = (WP << 7) & (ENEMY_PIECES) & (~FileA) & (~Rank8); // (~Rank8) -> no promotion
    while (PMoves != 0){
        *move_list++ = CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 7)); // search_list(to<<6+from)
        move_counter++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the search_list
    }
    // 'end'
    // Capture to the left //
    PMoves = (WP << 9) & (ENEMY_PIECES) & (~FileH) & (~Rank8);
    while (PMoves != 0){
        *move_list++ = CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 9));
        move_counter++;
        PMoves&= PMoves - 1;
    }
    // one forward //
    PMoves = (WP << 8) & (EMPTY) & (~Rank8);
    while (PMoves != 0){
        *move_list++ = WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 8));
        move_counter++;
        PMoves&= PMoves - 1;
    }
    // two forward // = double pawn pushes
    PMoves = (WP << 16) & (EMPTY) & (EMPTY << 8) & (Rank4);
    while (PMoves != 0){
        *move_list++ = DOUBLEPAWNPUSH | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 16));
        move_counter++;
        PMoves&= PMoves - 1;
    }
    // promotions //
    if ((WP&Rank7)!=0) {
        // promotion capture right //
        PMoves = (WP << 7) & (ENEMY_PIECES) & (Rank8) & (~FileA);
        while (PMoves != 0) {
            *move_list++ = KNIGHT | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 7));move_counter++;
            *move_list++ = BISHOP | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 7));move_counter++;
            *move_list++ = ROOK | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 7));move_counter++;
            *move_list++ = QUEEN | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 7));move_counter++;
            PMoves &= PMoves - 1;
        }
        // promotion capture left //
        PMoves = (WP << 9) & (ENEMY_PIECES) & (Rank8) & (~FileH);
        while (PMoves != 0) {
            *move_list++ = KNIGHT | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 9));move_counter++;
            *move_list++ = BISHOP | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 9));move_counter++;
            *move_list++ = ROOK | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 9));move_counter++;
            *move_list++ = QUEEN | PROMOTION | CAPTURE | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 9));move_counter++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward //
        PMoves = (WP << 8) & (EMPTY) & (Rank8);
        while (PMoves != 0) {
            *move_list++ = KNIGHT | PROMOTION | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 8));move_counter++;
            *move_list++ = BISHOP | PROMOTION | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 8));move_counter++;
            *move_list++ = ROOK | PROMOTION | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 8));move_counter++;
            *move_list++ = QUEEN | PROMOTION | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 8));move_counter++;
            PMoves &= PMoves - 1;
        }
    }
    // En passant //
    if(EP_STATE && ((WP & Rank5) != 0)) {
        // en passant right
        PMoves = ((WP>>1) & (1ULL << EP_PAWN) & (~FileA)) << 8;
        if(PMoves){
            *move_list++ = ENPASSANT | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 7));move_counter++;
        }
        // en passant left
        PMoves = ((WP<<1) & (1ULL << EP_PAWN) & (~FileH)) << 8;
        if(PMoves){
            *move_list++ = ENPASSANT | WHITEPAWN | (lsb(PMoves) << 6) | (lsb(PMoves >> 9));move_counter++;
        }
    }
    EP_STATE = false;
}
void genBPawn(){ // same as genWPawn just in opposite direction
    // Capture to the right //
    U64 PMoves = (BP >> 7) & (ENEMY_PIECES) & (~FileH) & (~Rank1); // (~Rank8) -> no promotion
    while (PMoves != 0){
        *move_list++ = CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 7)); // search_list(from+to)
        move_counter++;
        PMoves &= PMoves - 1; //deletes the move from PMoves after adding it to the search_list
    }
    // Capture to the left //
    PMoves = (BP >> 9) & (ENEMY_PIECES) & (~FileA) & (~Rank1);
    while (PMoves != 0){
        *move_list++ = CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 9));
        move_counter++;
        PMoves&= PMoves - 1;
    }
    // one forward //
    PMoves = (BP >> 8) & (EMPTY) & (~Rank1);
    while (PMoves != 0){
        *move_list++ = BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 8));
        move_counter++;
        PMoves&= PMoves - 1;
    }
    // two forward //
    PMoves = (BP >> 16) & (EMPTY) & (EMPTY >> 8) & (Rank5);
    while (PMoves != 0){
        *move_list++ = DOUBLEPAWNPUSH | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 16));
        move_counter++;
        PMoves&= PMoves - 1;
    }
    // promotions //
    if ((BP&Rank2)!=0) { // optimized: only generates Promotion if Pawns are on the seventh Rank
        // promotion capture to the right //
        PMoves = (BP >> 7) & (ENEMY_PIECES) & (Rank1) & (~FileH);
        while (PMoves != 0) {
            *move_list++ = QUEEN | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 7)); move_counter++;
            *move_list++ = KNIGHT | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 7)); move_counter++;
            *move_list++ = BISHOP | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 7)); move_counter++;
            *move_list++ = ROOK | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 7)); move_counter++;
            PMoves &= PMoves - 1;
        }
        // promotion capture to the left //
        PMoves = (BP >> 9) & (ENEMY_PIECES) & (Rank1) & (~FileA); // &(~FileA) cannot be excluded
        while (PMoves != 0) {
            *move_list++ = QUEEN | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) + (lsb(PMoves << 9)); move_counter++;
            *move_list++ = KNIGHT | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) + (lsb(PMoves << 9)); move_counter++;
            *move_list++ = BISHOP | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) + (lsb(PMoves << 9)); move_counter++;
            *move_list++ = ROOK | PROMOTION | CAPTURE | BLACKPAWN | (lsb(PMoves) << 6) + (lsb(PMoves << 9)); move_counter++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward //
        PMoves = (BP >> 8) & (EMPTY) & (Rank1);
        while (PMoves != 0) {
            *move_list++ = QUEEN | PROMOTION | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 8)); move_counter++;
            *move_list++ = KNIGHT | PROMOTION | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 8)); move_counter++;
            *move_list++ = BISHOP | PROMOTION | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 8)); move_counter++;
            *move_list++ = ROOK | PROMOTION | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 8)); move_counter++;
            PMoves &= PMoves - 1;
        }
    }
    // En passant //
    if(EP_STATE && (BP & Rank4)) {
        // en passant right
        PMoves = ((BP<<1) & (1ULL << EP_PAWN) & (~FileH)) >> 8;
        if(PMoves){
            *move_list++ = ENPASSANT | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 7)); move_counter++;
        }
        // en passant left
        PMoves = ((BP>>1) & (1ULL << EP_PAWN) & (~FileA)) >> 8;
        if(PMoves){
            *move_list++ = ENPASSANT | BLACKPAWN | (lsb(PMoves) << 6) | (lsb(PMoves << 9)); move_counter++;
        }
    }
    EP_STATE = false;
}

U64 genDiagonal(U64 DMP){ // DMP: Diagonal Moving Piece
    int8_t i = lsb(DMP);
    int8_t file = i % 8;
    int8_t rank = i / 8;
    // Diagonal Moves //
    U64 m = SD_mask[rank + file];
    U64 diagonalMoves = (((OCCUPIED & m) - 2*DMP ) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(DMP))) & m;
    // Anti-diagonal Moves //
    m = AD_mask[7 + rank - file];
    diagonalMoves |= (((OCCUPIED & m) - 2*DMP) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(DMP))) & m;
    return diagonalMoves;
}
U64 genStraight(U64 SMP){ // SMP: Straight Moving Piece
    int8_t i = lsb(SMP);
    // Vertical Moves //
    U64 m = V_mask[i % 8];
    U64 straightMoves = (((OCCUPIED&m) - 2*SMP ) ^ reverse_bytes(reverse_bytes(OCCUPIED & m) - 2 * reverse_bytes(SMP))) & m;
    // Horizontal Moves //
    int8_t rank = i/8;
    m = H_mask[rank];
    straightMoves |= ((OCCUPIED - 2*SMP)^ reverse_rank(reverse_rank(OCCUPIED, rank) - 2*reverse_rank(SMP, rank), rank))&m;
    return straightMoves;
}
U64 genKnightSpan(U64 NMP){ // KMP: Night Moving Piece
    U64 NMoves;
    int8_t from = lsb(NMP);
    if(from>=18){
        NMoves = KnightSpan<<(from-18);
    }else{
        NMoves = KnightSpan>>(18-from);
    }if(from%8 < 4){
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
    }if(from%8 < 4){
        KMoves &=(~FileA);
    }else{
        KMoves &=(~FileH);
    }
    return KMoves;
}
void genRook(U64 RBB){ // RBB = Rook BitBoard
    U64 RMoves;
    U64 move;
    U64 R = RBB&~(RBB-1);   // takes the LSB from RBB
    while (R != 0) {
        int8_t from = lsb(R);
        RMoves = genStraight(R) & NOT_MY_PIECES;
        move = RMoves&~(RMoves-1);      // takes the LSB from RMoves

        while(move != 0){
            *move_list++ = ((move & OCCUPIED) ? CAPTURE : NONE) | ((WHITE) ? WHITEROOK : BLACKROOK) | (lsb(move) << 6) | from;
            move_counter++;
            RMoves &= ~move;            // deletes the LSB from RMoves
            move = RMoves&~(RMoves-1);  // takes next LSB from RMoves
        }
        RBB &= ~R;          // deletes the LSB from RBB
        R = RBB&~(RBB-1);   // takes next LSB from RBB
    }
}
void genBishop(U64 BBB){ // BBB = Bishop BitBoard, same as genRook()
    U64 BMoves;
    U64 move;
    U64 B = BBB & ~(BBB - 1);
    while (B != 0) {
        int8_t from = lsb(B);
        BMoves = genDiagonal(B) & NOT_MY_PIECES;
        move = BMoves&~(BMoves-1);
        while(move != 0){
            *move_list++ = ((move & OCCUPIED) ? CAPTURE : NONE) | ((WHITE) ? WHITEBISHOP : BLACKBISHOP) | (lsb(move) << 6) | from;
            move_counter++;
            BMoves &= ~move;
            move = BMoves&~(BMoves-1);
        }
        BBB &= ~B;
        B = BBB & ~(BBB - 1);
    }
}
void genQueen(U64 QBB){ // QBB = Queen BitBoard, same as genRook()
    U64 QMoves;
    U64 move;
    U64 Q = QBB & ~(QBB - 1);
    while (Q != 0) {
        int8_t from = lsb(Q);
        QMoves = (genStraight(Q)|genDiagonal(Q)) & NOT_MY_PIECES;
        move = QMoves & ~(QMoves - 1);
        while(move != 0){
            *move_list++ = ((move & OCCUPIED) ? CAPTURE : NONE) | ((WHITE) ? WHITEQUEEN : BLACKQUEEN) | (lsb(move) << 6) | from;
            move_counter++;
            QMoves &= ~move;
            move = QMoves & ~(QMoves - 1);
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
        int8_t from = lsb(N);
        NMoves = genKnightSpan(N) & NOT_MY_PIECES;
        move = NMoves & ~(NMoves-1);
        while(move != 0){
            *move_list++ = ((move & OCCUPIED) ? CAPTURE : NONE) | ((WHITE) ? WHITEKNIGHT : BLACKKNIGHT) | (lsb(move) << 6) | from;
            move_counter++;
            NMoves &= ~move;
            move = NMoves & ~(NMoves-1);
        }
        NBB &= ~N;
        N = NBB & ~(NBB-1);
    }
}
void genKing(U64 KBB){ // KBB = King BitBoard
    U64 KMoves = genKingSpan(KBB) & NOT_MY_PIECES;
    U64 move = KMoves & ~(KMoves-1);
    int8_t from = lsb(KBB);
    while(move != 0){
        *move_list++ = ((move & OCCUPIED) ? CAPTURE : NONE) | ((WHITE) ? WHITEKING : BLACKKING) | (lsb(move) << 6) | from;
        move_counter++;
        KMoves &= ~move;
        move = KMoves & ~(KMoves-1);
    }
}
void genWCastling(){
    if(CASTLING_RIGHTS&3){
        if((CASTLING_RIGHTS&1) && ((OCCUPIED&F1G1) == 0)){
            *move_list++ = WHITE_KS_CASTLING;
            move_counter++;
        }
        if((CASTLING_RIGHTS&2) && ((OCCUPIED&B1C1D1) == 0)){
            *move_list++ = WHITE_QS_CASTLING;
            move_counter++;
        }
    }
}
void genBCastling(){
    if(CASTLING_RIGHTS&12){
        if((CASTLING_RIGHTS&4) && ((OCCUPIED&F8G8) == 0)){
            *move_list++ = BLACK_KS_CASTLING;
            move_counter++;
        }
        if((CASTLING_RIGHTS&8) && ((OCCUPIED&B8C8D8) == 0)){
            *move_list++ = BLACK_QS_CASTLING;
            move_counter++;
        }
    }
}
bool square_attacked(U64 square){
    // Queen - Rook
    U64 straight_attacks = genStraight(square);
    if(straight_attacks & ((*Bitboard[6 + op_side]) | (*Bitboard[8 + op_side]))) {
        return true;
    }
    // Queen - Bishop
    U64 diagonal_attacks = genDiagonal(square);
    if(diagonal_attacks & ((*Bitboard[4 + op_side]) | (*Bitboard[8 + op_side]))){
        return true;
    }
    // Knight
    U64 knight_attacks = genKnightSpan(square);
    if(knight_attacks & (*Bitboard[2 + op_side])){
        return true;
    }
    // Pawn
    U64 pawn_attacks = (WHITE)? ((square << 7)&(~FileA)) | ((square << 9)&(~FileH)): ((square >> 7)&(~FileH)) | ((square >> 9)&(~FileA));
    if(pawn_attacks & (*(Bitboard[op_side]))){
        return true;
    }
    // King
    U64 king_attacks = genKingSpan(square);
    if(king_attacks & (*Bitboard[10 + op_side])){
        return true;
    }
    return false;
}