#include "move_gen.h"
#include "board.h"
#include "converter.h"
#include "search.h"

U64 OCCUPIED;
U64 EMPTY;
U64 ENEMY_PIECES;
U64 NOT_MY_PIECES;

void move_generator(){
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
        pseudolegal_move_count++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the search_list
    }
    // 'end'
    // Capture to the left //
    PMoves = (WP << 9) & (ENEMY_PIECES) & (~FileH) & (~Rank8);
    while (PMoves != 0){
        *move_list++ = CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));
        pseudolegal_move_count++;
        PMoves&= PMoves - 1;
    }
    // one forward //
    PMoves = (WP << 8) & (EMPTY) & (~Rank8);
    while (PMoves != 0){
        *move_list++ = WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));
        pseudolegal_move_count++;
        PMoves&= PMoves - 1;
    }
    // two forward // = double pawn pushes
    PMoves = (WP << 16) & (EMPTY) & (EMPTY << 8) & (Rank4);
    while (PMoves != 0){
        *move_list++ = DOUBLEPAWNPUSH + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 16));
        pseudolegal_move_count++;
        PMoves&= PMoves - 1;
    }
    // promotions //
    if ((WP&Rank7)!=0) {
        // promotion capture right //
        PMoves = (WP << 7) & (ENEMY_PIECES) & (Rank8) & (~FileA);
        while (PMoves != 0) {
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));pseudolegal_move_count++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));pseudolegal_move_count++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));pseudolegal_move_count++;
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));pseudolegal_move_count++;
            PMoves &= PMoves - 1;
        }
        // promotion capture left //
        PMoves = (WP << 9) & (ENEMY_PIECES) & (Rank8) & (~FileH);
        while (PMoves != 0) {
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));pseudolegal_move_count++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));pseudolegal_move_count++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));pseudolegal_move_count++;
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));pseudolegal_move_count++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward //
        PMoves = (WP << 8) & (EMPTY) & (Rank8);
        while (PMoves != 0) {
            *move_list++ = KNIGHT   + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));pseudolegal_move_count++;
            *move_list++ = BISHOP   + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));pseudolegal_move_count++;
            *move_list++ = ROOK     + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));pseudolegal_move_count++;
            *move_list++ = QUEEN    + PROMOTION + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 8));pseudolegal_move_count++;
            PMoves &= PMoves - 1;
        }
    }
    // En passant //
    game_state++;
    if((game_state->ep_state) && ((WP&Rank5) != 0ULL)) {
        // en passant right
        PMoves = (WP<<7)&(1ULL<<((game_state->ep_file)+40));
        if(PMoves){
            *move_list++ = ENPASSANT + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 7));pseudolegal_move_count++;
        }
        // en passant left
        PMoves = (WP<<9)&(1ULL<<((game_state->ep_file)+40));
        if(PMoves){
            *move_list++ = ENPASSANT + WHITEPAWN + (lsb(PMoves) << 6) + (lsb(PMoves >> 9));pseudolegal_move_count++;
        }
    }
    game_state--;
}
void genBPawn(){ // same as genWPawn just in opposite direction
    // Capture to the right //
    U64 PMoves = (BP >> 7) & (ENEMY_PIECES) & (~FileH) & (~Rank1); // (~Rank8) -> no promotion
    while (PMoves != 0){
        *move_list++ = CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7)); // search_list(from+to)
        pseudolegal_move_count++;
        PMoves&= PMoves - 1; //deletes the move from PMoves after adding it to the search_list
    }
    // Capture to the left //
    PMoves = (BP >> 9) & (ENEMY_PIECES) & (~FileA) & (~Rank1);
    while (PMoves != 0){
        *move_list++ = CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));
        pseudolegal_move_count++;
        PMoves&= PMoves - 1;
    }
    // one forward //
    PMoves = (BP >> 8) & (EMPTY) & (~Rank1);
    while (PMoves != 0){
        *move_list++ = BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));
        pseudolegal_move_count++;
        PMoves&= PMoves - 1;
    }
    // two forward //
    PMoves = (BP >> 16) & (EMPTY) & (EMPTY >> 8) & (Rank5);
    while (PMoves != 0){
        *move_list++ = DOUBLEPAWNPUSH + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 16));
        pseudolegal_move_count++;
        PMoves&= PMoves - 1;
    }
    // promotions //
    if ((WP&Rank2)!=0) { // optimized: only generates Promotion if Pawns are on the seventh Rank
        // promotion capture to the right //
        PMoves = (BP >> 7) & (ENEMY_PIECES) & (Rank1) & (~FileH);
        while (PMoves != 0) {
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));pseudolegal_move_count++;
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));pseudolegal_move_count++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));pseudolegal_move_count++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));pseudolegal_move_count++;
            PMoves &= PMoves - 1;
        }
        // promotion capture to the left //
        PMoves = (BP >> 9) & (ENEMY_PIECES) & (Rank1) & (~FileA); // &(~FileA) cannot be excluded
        while (PMoves != 0) {
            *move_list++ = QUEEN    + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));pseudolegal_move_count++;
            *move_list++ = KNIGHT   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));pseudolegal_move_count++;
            *move_list++ = BISHOP   + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));pseudolegal_move_count++;
            *move_list++ = ROOK     + PROMOTION + CAPTURE + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));pseudolegal_move_count++;
            PMoves &= PMoves - 1;
        }
        // promotion one forward //
        PMoves = (BP >> 8) & (EMPTY) & (Rank1);
        while (PMoves != 0) {
            *move_list++ = QUEEN    + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));pseudolegal_move_count++;
            *move_list++ = KNIGHT   + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));pseudolegal_move_count++;
            *move_list++ = BISHOP   + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));pseudolegal_move_count++;
            *move_list++ = ROOK     + PROMOTION + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 8));pseudolegal_move_count++;
            PMoves &= PMoves - 1;
        }
    }
    // En passant //
    game_state++;
    if((game_state->ep_state) && (BP & Rank4)) {
        // en passant right
        PMoves = (BP>>7)&(1ULL<<(game_state->ep_file+16));
        if(PMoves){
            *move_list++ = ENPASSANT + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 7));pseudolegal_move_count++;
        }
        // en passant left
        PMoves = (WP>>9)&(1ULL<<(game_state->ep_file+16));
        if(PMoves){
            *move_list++ = ENPASSANT + BLACKPAWN + (lsb(PMoves) << 6) + (lsb(PMoves << 9));pseudolegal_move_count++;
        }
    }
    game_state--;
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
    U64 RMoves;
    U64 move;
    U64 R = RBB&~(RBB-1);   // takes the LSB from RBB
    while (R != 0) {
        int from = lsb(R);
        RMoves = genStraight(R) & NOT_MY_PIECES;
        move = RMoves&~(RMoves-1);      // takes the LSB from RMoves

        while(move != 0){
            *move_list++ =  ((move&OCCUPIED)?CAPTURE:NONE) + ((WHITE)?WHITEROOK:BLACKROOK) + (lsb(move) << 6) + from;
            pseudolegal_move_count++;
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
        int from = lsb(B);
        BMoves = genDiagonal(B) & NOT_MY_PIECES;
        move = BMoves&~(BMoves-1);
        while(move != 0){
            *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + ((WHITE)?WHITEBISHOP:BLACKBISHOP) + (lsb(move) << 6) + from;
            pseudolegal_move_count++;
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
        int from = lsb(Q);
        QMoves = (genStraight(Q)|genDiagonal(Q)) & NOT_MY_PIECES;
        move = QMoves & ~(QMoves - 1);
        while(move != 0){
            *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + ((WHITE)?WHITEQUEEN:BLACKQUEEN) + (lsb(move) << 6) + from;
            pseudolegal_move_count++;
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
        int from = lsb(N);
        NMoves = genKnightSpan(N) & NOT_MY_PIECES;
        move = NMoves & ~(NMoves-1);
        while(move != 0){
            *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + ((WHITE)?WHITEKNIGHT:BLACKKNIGHT) + (lsb(move) << 6) + from;
            pseudolegal_move_count++;
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
    int from = lsb(KBB);
    while(move != 0){
        *move_list++ = ((move&OCCUPIED)?CAPTURE:NONE) + ((WHITE)?WHITEKING:BLACKKING) + (lsb(move) << 6) + from;
        pseudolegal_move_count++;
        KMoves &= ~move;
        move = KMoves & ~(KMoves-1);
    }
}
void genWCastling(){
    int Castlestate = (game_state+=2)->castling_rights; game_state-=2;
    if(Castlestate!=0){
        if((Castlestate!=1) && ((OCCUPIED&F1G1) == 0)){
            *move_list++ = CASTLING + WHITEROOK + (2<<6) + 0;
            pseudolegal_move_count++;
        }
        if((Castlestate!=2) && ((OCCUPIED&B1C1D1) == 0)){
            *move_list++ = CASTLING+ QUEEN_SIDE + WHITEROOK + (4<<6) + 7;
            pseudolegal_move_count++;
        }
    }
}
void genBCastling(){
    int Castlestate = (game_state+=2)->castling_rights; game_state-=2;
    if(Castlestate!=0){
        if((Castlestate!=1) && ((OCCUPIED&F8G8) == 0)){
            *move_list++ = CASTLING + BLACKROOK + (58<<6) + 56;
            pseudolegal_move_count++;
        }
        if((Castlestate!=2) && ((OCCUPIED&B8C8D8) == 0)){
            *move_list++ = CASTLING + QUEEN_SIDE + BLACKROOK + (60<<6) + 63;
            pseudolegal_move_count++;
        }
    }
}
bool square_attacked(U64 square){
    // Queen - Rook
    U64 straight_attacks = genStraight(square);
    if(straight_attacks & ((*Bitboard[3 + op_side * 6]) | (*Bitboard[4 + op_side * 6]))) {
        return true;
    }
    // Queen - Bishop
    U64 diagonal_attacks = genDiagonal(square);
    if(diagonal_attacks & ((*Bitboard[2 + op_side * 6]) | (*Bitboard[4 + op_side * 6]))){
        return true;
    }
    // Knight
    U64 knight_attacks = genKnightSpan(square);
    if(knight_attacks & (*Bitboard[1 + op_side * 6])){
        return true;
    }
    // Pawn
    U64 pawn_attacks = (WHITE)? (square << 7) | (square << 9) : (square >> 7) | (square >> 9);
    if(pawn_attacks & (*Bitboard[op_side * 6])){
        return true;
    }else{}
    // King
    U64 king_attacks = genKingSpan(square);
    if(king_attacks & (*Bitboard[5 + op_side * 6])){
        return true;
    }
    return false;
}