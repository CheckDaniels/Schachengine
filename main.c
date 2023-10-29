#include "board.h"
#include "benchmarking.h"
#include "search.h"
#include "uci.h"

int main(){
    char board[8][8];
    char fen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
    read_FEN(fen, board);
    WHITE^=1;
    start();
    search(MAX_PLY);
    stop();
    printf("Nodes: %d\n", perftNodeCounter);
    //printf("Captures: %d\n",perftCaptCounter-perftEPCounter/2);
    //printf("En Passants: %d\n",perftEPCounter/2);
    //printf("Castles: %d\n",perftCastleCounter);
    printf("Mates: %d\n",perftMateCounter);
    /*for (int i=0;i<x;i++){
        printf("%d ",COUNTERLIST[i]);
    }
     */

    return 0;
}
