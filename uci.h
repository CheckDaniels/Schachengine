#pragma once

extern const char pieceChars[];

void UCI_Protocol();
void parse_position(char *command);
bool parse_move(char move_input[]);
void FEN_to_position(char *fen);
