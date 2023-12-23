#pragma once

#include "sudoku.h"

typedef struct NakedPairs_impl
{
    Cell **p_cells;
    int index1;
    int index2;
} NakedPairs;


int check_hidden_single_values(Cell **p_cells, int i, int j);
void find_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs,
                        int *p_counter);
int check_solved_pairs(NakedPairs p_naked_pair, NakedPairs* solved_pairs, int solved_count);
int naked_pairs(SudokuBoard *p_board);