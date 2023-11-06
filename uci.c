#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "converter.h"
#include "board.h"
#include "search.h"
#include "move_gen.h"
#include "uci.h"

char start_position[128] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// UCI protocol communication with the GUI // by Code Monkey King's -> https://www.youtube.com/@chessprogramming591
void UCI_Protocol(){
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    // GUI-input buffer //
    char input[2000];

    printf("id name Check Daniel's engine\n");
    printf("id name Daniel Richter\n");
    printf("uciok\n");
    while(1){
        // reset GUI-input
        memset(input, 0, sizeof(input));
        // makes sure the output reaches the GUI
        fflush(stdout);

        // get GUI-input
        if(!fgets(input, 2000, stdin))
            continue;
        if(input[0]=='\n')
            continue;

        // answer UCIs "isready" command
        if (strncmp(input, "isready", 7) == 0)
        {
            printf("readyok\n");
            continue;
        }
            // UCI "position" command
        else if (strncmp(input, "position", 8) == 0) {
            // call parse position function
            parse_position(input);

        }
            // UCI "ucinewgame" command
        else if (strncmp(input, "ucinewgame", 10) == 0) {
            // call parse position function
            parse_position("position startpos");
        }
            // UCI "go" command
        else if (strncmp(input, "go", 2) == 0)
            // call parse go function
            continue;

            // UCI "quit" command
        else if (strncmp(input, "quit", 4) == 0)
            // quit from the chess engine program execution
            break;

            // UCI "uci" command
        else if (strncmp(input, "uci", 3) == 0)
        {
            // print engine info
            printf("id name Check Daniel's engine\n");
            printf("id name Daniel Richter\n");
            printf("uciok\n");
        }

    }

}

// parse UCI "position" command // by Code Monkey King's -> https://www.youtube.com/@chessprogramming591 //
void parse_position(char *command){
    // shift pointer to the right where next token begins
    command += 9;

    // init pointer to the current character in the command string
    char *current_char = command;

    // parse UCI "startpos" command
    if (strncmp(command, "startpos", 8) == 0) {
        // init chess board with start position
        FEN_to_position(start_position);

    }else{
        // parse UCI "fen" command
        // make sure "fen" command is available within command string
        current_char = strstr(command, "fen");

        // if no "fen" command is available within command string
        if (current_char == NULL)
            // init chess board with start position
            FEN_to_position(start_position);

            // found "fen" substring
        else
        {
            // shift pointer to the right where next token begins
            current_char += 4;

            // init chess board with position from FEN string
            FEN_to_position(current_char);
        }
    }

    // parse moves after position
    current_char = strstr(command, "moves");

    // moves available
    if (current_char != NULL)
    {
        // shift pointer to the right where next token begins
        current_char += 6;

        // loop over moves within a move string
        while(*current_char){

            if ((current_char == NULL)||(current_char == ' '))
                break;

            // make next move
            if (!parse_move(current_char)){
                printf("illegal move\n");
                break;
            }

            // move current character pointer to the end of current move
            while (*current_char && *current_char != ' ') current_char++;

            // go to the next move
            current_char++;
        }
    }

    // print board
    printBoard();
}


bool parse_move(char move_input[5]){

    // creates an empty move list
    int move_list[256];
    int *movelist_ptr = move_list;
    int move_count = 0;

    if (move_input[4] == '\n')
        move_input[4] = ' ';

    // move generator //
    move_generator(movelist_ptr, &move_count);

    copy_board()
    // loop over generated moves
    for (int i = 0; i < move_count; i++) {

        // convert the move to a string
        char* string = move_to_string(*movelist_ptr++);
        //char gen_movestring[6];
        /*for(int j=0; j<6 ;j++)
            gen_movestring[j] = *string++;*/
        // check if the move is a generated move
        if(strncmp(string, move_input, 5) == 0) {
            // check if it is legal and make it
            if (!make_move(*(movelist_ptr-1))){
                take_back()
                return false;
            }
            WHITE ^=1; SIGN *= -1;
            return true;
        }
    }
    return false;
}

// Define piece characters for FEN
const char pieceChars[] = "PNBRQKpnbrqk";

// Function to convert FEN string to an array-board and thus update the Bitboards // made with the help of Chat-GPT
void FEN_to_position(char *fen) {
    // creates a temporary array-board
    char board[8][8];
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
                }
            }
        }
    }

    // creates the Bitboards //
    arrayToBitBoard(8,8,board);
    //printBoard();
    //printf("%s\n",fen);

    // looks for additional Information //
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


            // Castling-rights //
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

            if(fen[i]=='-'){
                i+=2;
            }else{
                i++;
            }

            //En Passant//
            if(fen[i] != '-'){
                EP_STATE = true;
                EP_PAWN = 7 - (fen[i] - 'a') + (fen[i + 1] - '0') * 8;
                i++;
            }
            i++;

            // fifty-move rule //
            break;
        }
        i++;
    }
}