#include "board.h"
#include "move_gen.h"
#include "converter.h"
#include "benchmarking.h"
#include "search.h"

int main(){
    init_BitBoard();
    printf("Array elements: \n");
    //genWPawn(WP);
    //genBPawn();
    //genDiagonal(WB);
    //genQueen(WQ);
    //genKnight(WN);
    //genKing(WK);
    //start();
    search();
    //stop();
    return 0;
}
