#include <stdio.h>
#include "benchmarking.h"
#include "converter.h"
#include "board.h"

// Just a fucking file with constants //

// Files //
const U64 FileA = 0x8080808080808080ULL;
const U64 FileB = FileA >> 1;
const U64 FileC = FileA >> 2;
const U64 FileD = FileA >> 3;
const U64 FileE = FileA >> 4;
const U64 FileF = FileA >> 5;
const U64 FileG = FileA >> 6;
const U64 FileH = FileA >> 7;

const U64 File_AB = FileA | FileB;
const U64 File_GH = FileG | FileH;
const U64 Queenside = FileA|FileB|FileC|FileD;
const U64 Kingside = FileE|FileF|FileG|FileH;

// Ranks //
const U64 Rank1 = 0xFF;
const U64 Rank2 = Rank1 << (8 * 1);
const U64 Rank3 = Rank1 << (8 * 2);
const U64 Rank4 = Rank1 << (8 * 3);
const U64 Rank5 = Rank1 << (8 * 4);
const U64 Rank6 = Rank1 << (8 * 5);
const U64 Rank7 = Rank1 << (8 * 6);
const U64 Rank8 = Rank1 << (8 * 7);

const U64 Whiteside = Rank1|Rank2|Rank3|Rank4;
const U64 Blackside = Rank5|Rank6|Rank7|Rank8;

const U64 mainDiagonal = 0x102040810204080ULL;
const U64 mainADiagonal = 0x8040201008040201ULL;

// Diagonals //
const U64 D1 = mainDiagonal >> (8 * 7);
const U64 D2 = mainDiagonal >> (8 * 6);
const U64 D3 = mainDiagonal >> (8 * 5);
const U64 D4 = mainDiagonal >> (8 * 4);
const U64 D5 = mainDiagonal >> (8 * 3);
const U64 D6 = mainDiagonal >> (8 * 2);
const U64 D7 = mainDiagonal >> (8 * 1);
//(D8 = mainDiagonal)
const U64 D9 = (mainDiagonal & mainDiagonal - 1) << 1;
const U64 D10 = (D9&D9-1)<<1;
const U64 D11 = (D10&D10-1)<<1;
const U64 D12 = (D11&D11-1)<<1;
const U64 D13 = (D12&D12-1)<<1;
const U64 D14 = (D13&D13-1)<<1;
const U64 D15 = (D14&D14-1)<<1;

// Anti-Diagonals //
const U64 AD1 = mainADiagonal >> (8 * 7);
const U64 AD2 = mainADiagonal >> (8 * 6);
const U64 AD3 = mainADiagonal >> (8 * 5);
const U64 AD4 = mainADiagonal >> (8 * 4);
const U64 AD5 = mainADiagonal >> (8 * 3);
const U64 AD6 = mainADiagonal >> (8 * 2);
const U64 AD7 = mainADiagonal >> (8 * 1);
//(AD8 = mainADiagonal)
const U64 AD9 = (mainADiagonal & mainADiagonal - 1) >> 1;
const U64 AD10 = (AD9&AD9-1)>>1;
const U64 AD11 = (AD10&AD10-1)>>1;
const U64 AD12 = (AD11&AD11-1)>>1;
const U64 AD13 = (AD12&AD12-1)>>1;
const U64 AD14 = (AD13&AD13-1)>>1;
const U64 AD15 = (AD14&AD14-1)>>1;

// Ray-Masks //
// for sliding pieces: Queen, Rook, Bishop
const U64 vMask[8] = {FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH};
const U64 hMask[8] = {Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8};
const U64 dMask[15] = {D1, D2, D3, D4, D5, D6, D7, mainDiagonal, D9, D10, D11, D12, D13, D14, D15};
const U64 aMask[15] = {AD1, AD2, AD3, AD4, AD5, AD6, AD7, mainADiagonal, AD9, AD10, AD11, AD12, AD13, AD14, AD15};

/* KnightSpan =
 * 0 0 0 0 1 0 1 0
 * 0 0 0 1 0 0 0 1
 * 0 0 0 0 0 0 0 0
 * 0 0 0 1 0 0 0 1
 * 0 0 0 0 1 0 1 0
 * KingSpan =
 * 0 0 0 0 0 1 1 1
 * 0 0 0 0 0 1 0 1
 * 0 0 0 0 0 1 1 1
 *
 * Can be shifted to create Moves for the Knight/King.
*/

const U64 KnightSpan = 0xA1100110AULL;
const U64 KingSpan = 0x70507ULL;

// default: Pieces //
U64 BR = 0;
U64 BN = 0;
U64 BB = 0;
U64 BQ = 0;
U64 BK = 0;
U64 BP = 0;
U64 WR = 0;
U64 WN = 0;
U64 WB = 0;
U64 WQ = 0;
U64 WK = 0;
U64 WP = 0;


void BitBoard() {
    //temp baord
    char board[8][8] = {// end <-
            {'r','n','b','q','k','b','n','r'},
            {'p','p','p','p','p','p','p','p'},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {'P','P','P','P','P','P','P','P'},
            {'R','R','B','Q','K','B','R','R'}
            //<- Bitdirection <- start
    };
    arrayToBitBoard(8, 8,board);
    printBoard();
    printBinary(Rank1, 64);
    printBinary(FileA, 64);
    printf("%d\n",lsb(WR));
}