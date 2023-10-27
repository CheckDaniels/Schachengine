#include <stdio.h>
#include "benchmarking.h"
#include "converter.h"
#include "board.h"


/////////////////////
/// THE BITBOARDS ///
/////////////////////
U64 *Bitboard[12] = {&BP, &BN, &BB, &BR, &BQ, &BK, &WP, &WN, &WB, &WR, &WQ, &WK};

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


void init_BitBoard() {
    //temp baord
    char board[8][8] = {//  63Bit : end <-------------------
                            {'r','n',' ',' ','k',' ',' ','r'},
                            {'p','p','p','p','p','p','p','p'},
                            {' ',' ',' ',' ',' ',' ',' ',' '},
                            {' ',' ',' ',' ',' ',' ',' ',' '},
                            {' ',' ',' ',' ',' ',' ',' ',' '},
                            {' ',' ',' ',' ',' ',' ',' ',' '},
                            {'P','P','P','P','P','P','P','P'},
                            {'R','B','B','B','K',' ',' ','R'}
                        //   <------------------ start : 0Bit
    };
    arrayToBitBoard(8, 8,board);
    printBinary(*(Bitboard[0]), 64);
    printBinary(B1C1D1, 64);
    printf("%d\n",lsb(WR));
    printBoard();

}

/// Just some fucking constants ///

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

const U64 mainSDiagonal = 0x102040810204080ULL;
const U64 mainADiagonal = 0x8040201008040201ULL;

// Standard-Diagonals //
const U64 SD1 = mainSDiagonal >> (8 * 7);
const U64 SD2 = mainSDiagonal >> (8 * 6);
const U64 SD3 = mainSDiagonal >> (8 * 5);
const U64 SD4 = mainSDiagonal >> (8 * 4);
const U64 SD5 = mainSDiagonal >> (8 * 3);
const U64 SD6 = mainSDiagonal >> (8 * 2);
const U64 SD7 = mainSDiagonal >> (8 * 1);
//(D8 = mainSDiagonal)
const U64 SD9 = (mainSDiagonal & mainSDiagonal - 1) << 1;
const U64 SD10 = (SD9&SD9-1)<<1;
const U64 SD11 = (SD10&SD10-1)<<1;
const U64 SD12 = (SD11&SD11-1)<<1;
const U64 SD13 = (SD12&SD12-1)<<1;
const U64 SD14 = (SD13&SD13-1)<<1;
const U64 SD15 = (SD14&SD14-1)<<1;

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
const U64 V_mask[8] = {FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH};
const U64 H_mask[8] = {Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8};
const U64 SD_mask[15] = {SD1, SD2, SD3, SD4, SD5, SD6, SD7, mainSDiagonal, SD9, SD10, SD11, SD12, SD13, SD14, SD15};
const U64 AD_mask[15] = {AD1, AD2, AD3, AD4, AD5, AD6, AD7, mainADiagonal, AD9, AD10, AD11, AD12, AD13, AD14, AD15};

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

// Squares //

const U64 B1 = 0x40ULL;
const U64 C1 = 0x20ULL;
const U64 D1 = 0x10ULL;
const U64 E1 = 0x10ULL;
const U64 F1 = 0x4ULL;
const U64 G1 = 0x2ULL;

const U64 B1C1D1 = B1|C1|D1;
const U64 F1G1 = F1|G1;
const U64 C1D1E1 = C1|D1|E1;
const U64 E1F1G1 = E1|F1|G1;

const U64 B8 = 0x4000000000000000ULL;
const U64 C8 = 0x2000000000000000ULL;
const U64 D8 = 0x1000000000000000ULL;
const U64 E8 = 0x800000000000000ULL;
const U64 F8 = 0x400000000000000ULL;
const U64 G8 = 0x200000000000000ULL;

const U64 B8C8D8 = B8|C8|D8;
const U64 F8G8 = F8|G8;
const U64 C8D8E8 = C8|D8|E8;
const U64 E8F8G8 = E8|F8|G8;