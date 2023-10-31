#pragma once
#include <stdbool.h>
#define my_side WHITE
#define op_side (1-WHITE)
#define neg_SIGN (SIGN*(-1))
#define MAX_PLY 5
#define copy_board()                                                                                    \
    U64 tBP, tBN, tBB, tBR, tBQ, tBK, tWP, tWN, tWB, tWR, tWQ, tWK/*, tOCCUPIED*/;                      \
    tBP = BP; tBN = BN; tBB = BB; tBR = BR; tBQ = BQ; tBK = BK;                                         \
    tWP = WP; tWN = WN; tWB = WB; tWR = WR; tWQ = WQ; tWK = WK;                                         \
    int8_t tCASTLING_RIGHTS;                                                                            \
    bool tEP_STATE;                                                                                     \
    tCASTLING_RIGHTS = CASTLING_RIGHTS;                                                                 \
    tEP_STATE = EP_STATE;                                                                               \
    //tOCCUPIED = OCCUPIED;                                                                             \

#define take_back()                                                                 \
    BP = tBP; BN = tBN; BB = tBB; BR = tBR; BQ = tBQ; BK = tBK;                     \
    WP = tWP; WN = tWN; WB = tWB; WR = tWR; WQ = tWQ; WK = tWK;                     \
    CASTLING_RIGHTS = tCASTLING_RIGHTS;                                             \
    EP_STATE = tEP_STATE;                                                           \
    //OCCUPIED = tOCCUPIED;                                                         \


extern unsigned long long perftNodeCounter;
extern unsigned long long perftCaptCounter;
extern int perftEPCounter;
extern int perftCastleCounter;
extern int perftMateCounter;
extern int perftPromotionCounter;

extern bool WHITE;
extern int8_t SIGN;
extern bool EP_STATE;
extern int8_t EP_SQUARE;
extern int8_t CASTLING_RIGHTS;
extern bool Checkmate;


void perft_driver(int ply);
void perft_test(int ply);
void search(int ply);