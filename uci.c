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
                SIGN = 1;
            }else if(fen[i] == 'b'){
                WHITE = false;
                SIGN = -1;
            }else{
                printf("Invalid Board\n");
                break;
            }

            i+=2;
            int8_t white_castling;
            int8_t black_castling;

            // White-Castling_rights //
            if((fen[i]=='K')&&(fen[i+1]=='Q')){
                white_castling = 3; i+=2;
            }else if(fen[i]=='K'){
                white_castling = 1; i++;
            }else if(fen[i]=='Q'){
                white_castling = 2; i++;
            }else{
                white_castling = 0;
            }
            // Black-Castling_rights //
            if((fen[i]=='k')&&(fen[i+1]=='q')){
                black_castling = 3; i+=2;
            }else if(fen[i]=='k'){
                black_castling = 1; i++;
            }else if(fen[i]=='q'){
                black_castling = 2; i++;
            }else{
                black_castling = 0;
            }
            CASTLING_RIGHTS = (black_castling<<2) + white_castling;
            printf("Castle-state: %d\n", CASTLING_RIGHTS);
            if(fen[i]=='-'){
                i+=2;
            }else{
                i++;
            }

            //En Passant//
            if(fen[i] != '-'){
                EP_STATE = true;
                switch(fen[i]){
                    case 'a': printf("FileA\n"); EP_SQUARE = 7; break;
                    case 'b': printf("FileB\n"); EP_SQUARE = 6; break;
                    case 'c': printf("FileC\n"); EP_SQUARE = 5; break;
                    case 'd': printf("FileD\n"); EP_SQUARE = 4; break;
                    case 'e': printf("FileE\n"); EP_SQUARE = 3; break;
                    case 'f': printf("FileF\n"); EP_SQUARE = 2; break;
                    case 'g': printf("FileG\n"); EP_SQUARE = 1; break;
                    case 'h': printf("FileH\n"); EP_SQUARE = 0; break;
                }
                i++;
                EP_SQUARE += ((fen[i]-'0')-2)*8;
                printf("%d\n", EP_SQUARE);
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

