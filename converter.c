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