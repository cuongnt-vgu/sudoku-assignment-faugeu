#include "naked_triples.h"
#include <stdlib.h>
#include <stdio.h>

int is_in_triples(int triples[3], int value)
{
    for (int i = 0; i < 3; i++)
    {
        if (triples[i] == value) return 1;
    }
    return 0;
}

int check_cell_in_triples(int triples[3], Cell* p_cell)
{
    int* candidates = get_candidates(p_cell);
    for (int i = 0; i < p_cell->num_candidates; i++)
    {
        if (!is_in_triples(triples, candidates[i]))
        {
            free(candidates);
            return 0;
        }
    }
    free(candidates);
    return 1;
}

int check_naked_triples(Cell **p_cells, int possible_triples[], int *indices)
{
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates == 1 || p_cells[i]->num_candidates > 3) continue;
        else if (check_cell_in_triples(possible_triples, p_cells[i])) indices[cell_counter++] = i;
    }
    if (cell_counter == 3) return 1;
    return 0;
}

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int unit)
{
    int candidate_counter[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        candidate_counter[i] = 0;
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int* candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            candidate_counter[candidates[j]-1] += 1;
        }
        free(candidates);
    }

    int possible_triples[BOARD_SIZE];
    int possible_triples_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidate_counter[i] >= 2)
        {
            possible_triples[possible_triples_count++] = i+1;
        }
    }

    if (possible_triples_count < 3) return;

    for (int i = 0; i < possible_triples_count - 2; i++)
    {
        for (int j = i+1; j < possible_triples_count - 1; j++)
        {
            for (int k = j+1; k < possible_triples_count; k++)
            {
                int values[3] = {possible_triples[i], possible_triples[j], possible_triples[k]};
                int indices[BOARD_SIZE];
                if (check_naked_triples(p_cells, values, indices))
                {
                    if (unit == 1)
                    {
                        if ((indices[0] == 0 || indices[0] == 3 || indices[0] == 6) && indices[1] == indices[0]+1 && indices[2] == indices[0]+2) continue;
                        else if ((indices[0] >= 0 && indices[0] <= 2) && indices[1] == indices[0]+3 && indices[2] == indices[0]+6) continue;
                    }
                    p_naked_triples[(*p_counter)++] = (NakedTriples){p_cells, {indices[0], indices[1], indices[2]}, {possible_triples[i], possible_triples[j], possible_triples[k]}};
                }
            }
        }
    }
}

int naked_triples(SudokuBoard *p_board)
{
    int counter = 0;

    NakedTriples p_naked_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triples(p_board->p_rows[i], p_naked_triples, &counter, 0);
        find_naked_triples(p_board->p_cols[i], p_naked_triples, &counter, 0);
        find_naked_triples(p_board->p_boxes[i], p_naked_triples, &counter, 1);
    }
    for (int i = 0; i < counter; i++)
    {
        Cell** p_cells = p_naked_triples[i].p_cells;
        int triples_values[3];
        for (int j = 0; j < 3; j++)
        {
            triples_values[j] = p_naked_triples[i].values[j];
        }
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (!check_cell_in_triples(triples_values, p_cells[j]))
            {
                for (int k = 0; k < 3; k++)
                {
                    if (is_candidate(p_cells[j], triples_values[k]))
                    {
                        unset_candidate(p_cells[j], triples_values[k]);
                    }
                }
            }
        }

    }
    return counter;
}