#include "board.h"
#include "move_gen.h"
#include "converter.h"
#include "benchmarking.h"


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
    move_generator();
    //stop();
    //printf("%d",square_attacked(F1));
    //int listsize = sizeof(search_list)/sizeof(search_list[0]);
    for (int i = 0; i < move_index; i++) {
        //printf("%d, ", search_list[i]);
        move_to_string(search_list[ply][i]);
        printf("\n");
        make_move(&search_list[ply][i]);
        printBoard();
        make_move(&search_list[ply][i]);
    }printf("\n");
    return 0;
}
