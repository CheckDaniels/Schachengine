#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "converter.h"
#include "search.h"

// Define piece characters for FEN
const char pieceChars[] = "PNBRQKpnbrqk";

// Function to convert FEN string to an array-board // with the help of Chat-GPT
void read_FEN(const char *fen, char board[8][8]) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board[rank][file] = ' ';
        }
    }

    int rank = 0;
    int file = 0;

    for (int i = 0; fen[i] != '\0' && rank < 8; i++) {
        if (fen[i] == ' ') break;

        if (fen[i] == '/') {
            rank++;
            file = 0;
        } else if (fen[i] >= '1' && fen[i] <= '8') {
            int emptySquares = fen[i] - '0';
            for (int j = 0; j < emptySquares; j++) {
                board[rank][file] = ' ';
                file++;
            }
        } else {
            for (int j = 0; j < sizeof(pieceChars); j++) {
                if (fen[i] == pieceChars[j]) {
                    board[rank][file] = fen[i];
                    file++;
                    break;
                }else{}
            }
        }
    }
    // create the Bitboards //
    arrayToBitBoard(8,8,board);
    printBoard();
    printf("%s\n",fen);
    // look for additional Information //
    strcat(fen, "                "); //16 safety-spaces
    int i = 0;
    while (fen[i] != '\0') {
        if (fen[i] == ' ') {
            i++;
            if(fen[i] == 'w'){
                WHITE = true;
            }else if(fen[i] == 'b'){
                WHITE = false;
            }else{
                printf("Invalid Board");
                break;
            }

            i+=2;
            // White-Castling_rights //
            if((fen[i]=='K')&&(fen[i+1]=='Q')){
                game_list[MAX_PLY+1+WHITE].castling_rights = 3; i+=2;
            }else if(fen[i]=='K'){
                game_list[MAX_PLY+1+WHITE].castling_rights = 2; i++;
            }else if(fen[i]=='Q'){
                game_list[MAX_PLY+1+WHITE].castling_rights = 1; i++;
            }else{
                game_list[MAX_PLY+1+WHITE].castling_rights = 0;
            }
            // Black-Castling_rights //
            if((fen[i]=='k')&&(fen[i+1]=='q')){
                game_list[MAX_PLY+2-WHITE].castling_rights = 3; i+=2;
            }else if(fen[i]=='k'){
                game_list[MAX_PLY+2-WHITE].castling_rights = 2; i++;
            }else if(fen[i]=='q'){
                game_list[MAX_PLY+2-WHITE].castling_rights = 1; i++;
            }else{
                game_list[MAX_PLY+2-WHITE].castling_rights = 0;
            }
            if(fen[i]=='-'){
                i+=2;
            }else{
                i++;
            }

            //En Passant//
            if(fen[i] != '-'){
                game_list[MAX_PLY+1].ep_state = 1;
                switch(fen[i]){
                    case 'a': printf("FileA\n");game_list[MAX_PLY+1].ep_file = 7; break;
                    case 'b': printf("FileB\n");game_list[MAX_PLY+1].ep_file = 6; break;
                    case 'c': printf("FileC\n");game_list[MAX_PLY+1].ep_file = 5; break;
                    case 'd': printf("FileD\n");game_list[MAX_PLY+1].ep_file = 4; break;
                    case 'e': printf("FileE\n");game_list[MAX_PLY+1].ep_file = 3; break;
                    case 'f': printf("FileF\n");game_list[MAX_PLY+1].ep_file = 2; break;
                    case 'g': printf("FileG\n");game_list[MAX_PLY+1].ep_file = 1; break;
                    case 'h': printf("FileH\n");game_list[MAX_PLY+1].ep_file = 0; break;
                }
                i++;
            }
            i++;
            break;
        }
        i++;
    }
}

int UCI_Protocol() {
    return 0;
}

