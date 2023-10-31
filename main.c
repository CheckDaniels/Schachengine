#include "board.h"
#include "benchmarking.h"
#include "search.h"
#include "uci.h"

int main(){
    char board[8][8];
    char fen[128] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
    read_FEN(fen, board);


    start();
    WHITE ^= 1; SIGN *= -1;
    search(MAX_PLY);
    stop();


    printf("Nodes: %llu\n", perftNodeCounter);
    printf("Captures: %llu\n", perftCaptCounter);
    printf("En Passants: %d\n", perftEPCounter);
    printf("Castles: %d\n", perftCastleCounter);
    printf("Mates: %d\n", perftMateCounter);
    printf("Promotions: %d\n", perftPromotionCounter);
    /*for (int i=0;i<k;i++){
        printf("%llu ",COUNTERLIST[i]);
        move_to_string(LISTE[i]);
    }*/
    /*for (int j=0; j<k;j++){
        printf("%d", LISTE[j]);
    }*/
    return 0;
}
