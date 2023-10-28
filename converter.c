#include <stdlib.h>
#include "board.h"
#include "converter.h"

int lsb(U64 bit) { // Returns the number of trailing zeros of any 64 bit binary number
    if (bit == 0) {
        return 64;  // Number of bits in a 64-bit integer
    }
    int count = 0;
    while ((bit & 1) == 0) {
        bit >>= 1;
        count++;
    }
    return count;
}

void move_to_string(int move){
    int from = move & 63;
    int to = (move>>6) & 63;
    int piecetype = (move>>12)&15;
    int captured = (move>>16)&1;
    int movetype = (move>>17)&3;
    int promotion = (move>>19)&3;

    char file_from = (7-from%8)+97;
    char rank_from = (from/8)+49;
    char file_to = (7-to%8)+97;
    char rank_to = (to/8)+49;

    char promoted_to = ' ';
    char type_of_move;
    switch(movetype){
        case 0: type_of_move = 'N'; break;
        case 1: type_of_move = 'C'; break;
        case 2: type_of_move = 'E'; break;
        case 3: type_of_move = 'P';
            switch(promotion){
                case 0: promoted_to = 'q'; break;
                case 1: promoted_to = 'n'; break;
                case 2: promoted_to = 'b'; break;
                case 3: promoted_to = 'r'; break;
            }break;
    }
    char notated_move[6];
    sprintf(notated_move,"%c%c%c%c%c%c",file_from, rank_from, file_to, rank_to, promoted_to, type_of_move);
    printf("%s \n",notated_move);
}
void arrayToBitBoard(int rank, int file, char chessboard[rank][file]){
    BR = BN = BB = BQ = BK = BP = WR = WN = WB = WQ = WK = WP = 0ULL;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int Piece = chessboard[rank][file];
            if (Piece != (int)' ') {
                // Set the bit at the (rank, file) position for the corresponding piece type
                switch (Piece){
                    case (int)'r': BR |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'n': BN |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'b': BB |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'q': BQ |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'k': BK |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'p': BP |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'R': WR |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'N': WN |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'B': WB |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'Q': WQ |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'K': WK |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;
                    case (int)'P': WP |= 1ULL << ((7 - rank) * 8 + (7 - file)); break;

                }
            }
        }
    }
}
void printBinary(U64 bNum, int bitcount) {
    for (int i = bitcount-1; i >= 0; i--){
        printf("%llu", (bNum >> i) & 1);
    }
    printf("\n");
}
void printBoard(){
    for (int i = 7; i >= 0; i--) {
        printf("[");
        for (int j = 7; j >= 0; j--) {
            if((BR & (1ULL << (i * 8 + j))) != 0) { printf("r");}
            if((BN & (1ULL << (i * 8 + j))) != 0) { printf("n");}
            if((BB & (1ULL << (i * 8 + j))) != 0) { printf("b");}
            if((BQ & (1ULL << (i * 8 + j))) != 0) { printf("q");}
            if((BK & (1ULL << (i * 8 + j))) != 0) { printf("k");}
            if((BP & (1ULL << (i * 8 + j))) != 0) { printf("p");}
            if((WR & (1ULL << (i * 8 + j))) != 0) { printf("R");}
            if((WN & (1ULL << (i * 8 + j))) != 0) { printf("N");}
            if((WB & (1ULL << (i * 8 + j))) != 0) { printf("B");}
            if((WQ & (1ULL << (i * 8 + j))) != 0) { printf("Q");}
            if((WK & (1ULL << (i * 8 + j))) != 0) { printf("K");}
            if((WP & (1ULL << (i * 8 + j))) != 0) { printf("P");}
            if((~(BR|BN|BB|BQ|BK|BP|WR|WN|WB|WQ|WK|WP)&(1ULL<<(i*8+j)))!=0){ printf(" ");}
            //if((Rank3 & (1ULL << (i * 8 + j))) != 0) { printf("X");}
            if(j!=0){printf(", ");}
        }
        printf("]\n");
    }
}



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