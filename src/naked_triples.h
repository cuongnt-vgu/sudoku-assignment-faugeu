#pragma once

#include "sudoku.h"

typedef struct NakedTriples_impl
{
    Cell **p_cells;
    int indices[3];
    int values[3];
} NakedTriples;

int is_in_triples(int triples[3], int value);
int check_cell_in_triples(int triples[3], Cell* p_cell);
int check_naked_triples(Cell **p_cells, int possible_triples[], int *indices);
void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int unit);
int naked_triples(SudokuBoard *p_board);