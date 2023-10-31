#pragma once

extern const char pieceChars[];

void read_FEN(const char *fen, char board[8][8]);

int UCI_Protocol();
