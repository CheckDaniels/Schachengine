#include <stdio.h>
#include "board.h"
#include "converter.h"
#include "move_gen.h"
#include "search.h"


bool WHITE;
int8_t SIGN;
bool EP_STATE;
int8_t EP_PAWN;
int8_t CASTLING_RIGHTS;
bool Checkmate;

void think(int ply){
    WHITE ^= 1; SIGN *= -1;
    search(ply);
}

void search(int ply) {
    WHITE ^= 1; SIGN *= -1;
    if (ply==0){
        // evaluate() //
        perftNodeCounter++;
    }else {

        // creates an empty move list
        int move_list[256];
        int move_count = 0;

        // move generator
        move_generator(move_list, &move_count);

        Checkmate = true;
        // copies all the bitboards
        copy_board()
        for (int* ptr = move_list; ptr < move_list + move_count; ptr++) {
            if (!make_move(*ptr)) {
                take_back()
                continue;
            }
            search(ply-1);
            //sets all the bitboards back to their copies
            take_back()
        }
        if (Checkmate){
            // return 10000000*SIGN //
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
    printf("\n     Performance test\n\n");
    // move list //
    int move_list[256];
    int move_count = 0;

    // move generator
    move_generator(move_list, &move_count);

    // loop over generated moves
    for (int* ptr = move_list; ptr < move_list + move_count; ptr++) {
        print_movestring(move_to_string(*ptr));
        copy_board()
        if (!make_move(*ptr)) {
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
        int move_count = 0;
        // move generator
        move_generator(move_list, &move_count);

        // loop over generated moves
        for (int* ptr = move_list; ptr < move_list + move_count; ptr++) {
            copy_board()
            if (!make_move(*ptr)) {
                take_back()
                continue;
            }
            perft_driver(ply - 1);
            take_back()
        }
    }
    WHITE^=1; SIGN *= -1;
}