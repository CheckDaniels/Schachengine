#include "board.h"
#include "benchmarking.h"
#include "search.h"
#include "uci.h"
#include "converter.h"

#define MAX_PLY 6
#define debug true
#define startpos "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "
#define fritz "r3r1k1/1pq2pp1/2p2n2/1PNn4/2QN2b1/6P1/3RPP2/2R3KB b - -"

// position startpos moves e2e4 e7e5 g1f3 b8c6 f1b5 g8f6 e1g1 f8e7 f1e1 e8g8
// position startpos moves d2d4 d7d5 c1f4 c8f5 b1c3 b8c6 d1d2 d8d7 e1c1 e8c8
// position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -

int main(){
    if (debug) {
        // Debug //
        char fen[128] = startpos;
        FEN_to_position(fen);
        printBoard();

        start();
        think(MAX_PLY);
        stop();

        printf("\n\nnodes: %llu\n", perftNodeCounter);
        printf("Captures: %llu\n", perftCaptCounter);
        printf("En Passants: %d\n", perftEPCounter);
        printf("Castles: %d\n", perftCastleCounter);
        printf("Mates: %d\n", perftMateCounter);
        printf("Promotions: %d\n", perftPromotionCounter);

    }else {
        // UCI-Protocol //
        UCI_Protocol();
    }
    return 0;
}


/*
    r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -

    Depth	Nodes	        Captures	E.p.	Castles	    Promotions	Checks	    Discovery Checks	Double Checks	Checkmates
    1	    48	            8	        0	    2	        0	        0	        0           	    0               0
    2	    2039	        351	        1	    91	        0	        3	        0	        	    0               0
    3	    97862	        17102	    45	    3162	    0	        993	        0	        	    0               1
    4	    4085603	        757163	    1929	128013	    15172	    25523	    42	                6	            43
    5	    193690690	    35043416	73365	4993637	    8392	    3309887	    19883	            2637	        30171
    6	    8031647685	    1558445089	3577504	184513607	56627920	92238050	568417	            54948	        360003
*/