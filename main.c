//
// Created by Checker_24 on 22.10.2023.
//
#include "board.h"
#include "move_gen.h"


int main(){
    BitBoard();
    printf("Array elements: \n");
    //genWPawn(WP);
    //genBPawn();
    //genDiagonal(WB);
    //genQueen(WQ);
    //genKnight(WN);
    //genKing(WK);
    move_generator(0);
    //int listsize = sizeof(movelist)/sizeof(movelist[0]);
    for (int i = 0; i < listindex; i++) {
        //printf("%d, ", movelist[i]);
        move_notation(movelist[i]);
    }

    return 0;
}