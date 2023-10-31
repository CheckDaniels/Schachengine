#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "converter.h"
#include "move_gen.h"
#include "search.h"
#include "makemove.h"


bool WHITE;
int8_t SIGN;
bool EP_STATE;
int8_t EP_SQUARE;
int8_t CASTLING_RIGHTS;
bool Checkmate;



void search(int ply) {
    WHITE ^= 1; SIGN *= -1;
    if (ply==0){
        // evaluate //
        perftNodeCounter++;
    }else {
        // creates an empty move list
        int move_list[256];
        int *movelist_ptr = move_list;
        int move_count = 0;

        // move generator
        move_generator(movelist_ptr, &move_count);

        // copies all the bitboards
        copy_board()
        Checkmate = true;
        for (int i = 0; i < move_count; i++) {

            //move_to_string(*movelist_ptr);
            if (!make_move(movelist_ptr++)) {
                take_back()
                continue;
            }
            search(ply-1);
            //sets all the bitboards back to their copies
            take_back()
        }
        if (Checkmate){
            // return 10000000*side //
        }
    }
    WHITE^=1; SIGN *= -1;
}


////////////////////////
/// perft functions ///
///////////////////////

// Perft-Debugging //
unsigned long long perftNodeCounter = 0;
unsigned long long perftCaptCounter = 0;
int perftEPCounter = 0;
int perftCastleCounter = 0;
int perftMateCounter = 0;
int perftPromotionCounter = 0;

void perft_test(int ply){
    WHITE^=1; SIGN *= -1;
    printf("\n     Performance test\n\n");

    // move list //
    int move_list[256];
    int *movelist_ptr = move_list;
    int move_count = 0;

    // move generator
    move_generator(movelist_ptr, &move_count);

    // loop over generated moves
    for (int i = 0; i < move_count; i++) {
        move_to_string(*movelist_ptr);
        copy_board()
        if (!make_move(movelist_ptr++)) {
            take_back()
            continue;
        }
        // cummulative nodes
        U64 cummulative_nodes = perftNodeCounter;

        // call perft driver recursively
        perft_driver(ply - 1);

        // old nodes
        U64 old_nodes = perftNodeCounter - cummulative_nodes;

        // take back
        take_back()
        // print move
        printf("%llu\n",old_nodes);
    }
    // print results
    printf("\n    Depth: %d\n", ply);
    printf("    Nodes: %llu\n", perftNodeCounter);
}

// Perft driver //
void perft_driver(int ply){
    WHITE ^= 1; SIGN *= -1;

    // reccursion escape condition
    if (ply == 0){
        // increment nodes count (count reached positions)
        perftNodeCounter++;
    }else {

        // move list //
        int move_list[256];
        int *movelist_ptr = move_list;
        int move_count = 0;
        // move generator
        move_generator(movelist_ptr, &move_count);

        // loop over generated moves
        for (int i = 0; i < move_count; i++) {
            copy_board()
            if (!make_move(movelist_ptr++)) {
                take_back()
                continue;
            }
            perft_driver(ply - 1);
            take_back()
        }
    }
    WHITE^=1; SIGN *= -1;
}