#pragma once
#include <stdio.h>
#include <stdint.h>
typedef uint_fast64_t U64;

// Files //
extern const U64 FileA;
extern const U64 FileB;
extern const U64 FileC;
extern const U64 FileD;
extern const U64 FileE;
extern const U64 FileF;
extern const U64 FileG;
extern const U64 FileH;

extern const U64 File_AB;
extern const U64 File_GH;
extern const U64 Kingside;
extern const U64 Queenside;

// Ranks //
extern const U64 Rank1;
extern const U64 Rank2;
extern const U64 Rank3;
extern const U64 Rank4;
extern const U64 Rank5;
extern const U64 Rank6;
extern const U64 Rank7;
extern const U64 Rank8;

extern const U64 Whiteside;
extern const U64 Blackside;

extern const U64 mainDiagonal;
extern const U64 mainADiagonal;

// Diagonals //
extern const U64 D1;
extern const U64 D2;
extern const U64 D3;
extern const U64 D4;
extern const U64 D5;
extern const U64 D6;
extern const U64 D7;
//(D8 = mainADiagonal)
extern const U64 D9;
extern const U64 D10;
extern const U64 D11;
extern const U64 D12;
extern const U64 D13;
extern const U64 D14;
extern const U64 D15;

// Anti-Diagonals //
extern const U64 AD1;
extern const U64 AD2;
extern const U64 AD3;
extern const U64 AD4;
extern const U64 AD5;
extern const U64 AD6;
extern const U64 AD7;
//(AD8 = mainDiagonal)
extern const U64 AD9;
extern const U64 AD10;
extern const U64 AD11;
extern const U64 AD12;
extern const U64 AD13;
extern const U64 AD14;
extern const U64 AD15;

extern const U64 vMask[];
extern const U64 hMask[];
extern const U64 dMask[];
extern const U64 aMask[];

extern const U64 KnightSpan;
extern const U64 KingSpan;
/*
     * 1000000100000000000000000000000000000000000000000000000000000000
     * 0100001000000000000000000000000000000000000000000000000000000000
     * 0010010000000000000000000000000000000000000000000000000000000000
     * 0001000000000000000000000000000000000000000000000000000000000000
     * 0000100000000000000000000000000000000000000000000000000000000000
     * 0000000011111111000000000000000000000000000000000000000000000000
     * 0000000000000000000000000000000000000000000000000000000010000001
     * 0000000000000000000000000000000000000000000000000000000001000010
     * 0000000000000000000000000000000000000000000000000000000000100100
     * 0000000000000000000000000000000000000000000000000000000000010000
     * 0000000000000000000000000000000000000000000000000000000000001000
     * 0000000000000000000000000000000000000000000000001111111100000000
     * 0000000000000000111111111111111111111111111111110000000000000000
*/
extern U64 BR;
extern U64 BN;
extern U64 BB;
extern U64 BQ;
extern U64 BK;
extern U64 BP;
extern U64 WR;
extern U64 WN;
extern U64 WB;
extern U64 WQ;
extern U64 WK;
extern U64 WP;

//U64 NO_PIECES;
//U64 WHITE_PIECES;
//U64 BLACK_PIECES;

void BitBoard();


//?int moveList[1024][2];