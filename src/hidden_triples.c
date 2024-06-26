#include "hidden_triples.h"
#include <stdlib.h>
#include <stdio.h>

int check_cell_in_hidden_triples(Cell* p_cell, int triples[3])
{
    for (int i = 0; i < 3; i++)
    {   
        if (is_candidate(p_cell, triples[i])) return 1;
    }
    return 0;
}

int check_hidden_triples(Cell **p_cells, int possible_triples[], int *indices)
{
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates == 1) continue;
        else if (check_cell_in_hidden_triples(p_cells[i], possible_triples)) indices[cell_counter++] = i;
    }
    return cell_counter == 3;
}

void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter)
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
        if (candidate_counter[i] == 2 || candidate_counter[i] == 3)
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
                if (check_hidden_triples(p_cells, values, indices))
                {
                    p_hidden_triples[(*p_counter)++] = (HiddenTriples){p_cells, {indices[0], indices[1], indices[2]}, {possible_triples[i], possible_triples[j], possible_triples[k]}};
                }
            }
        }
    }
}

int hidden_triples(SudokuBoard *p_board)
{
    int counter = 0;

    HiddenTriples p_hidden_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_triples(p_board->p_rows[i], p_hidden_triples, &counter);
        find_hidden_triples(p_board->p_cols[i], p_hidden_triples, &counter);
        find_hidden_triples(p_board->p_boxes[i], p_hidden_triples, &counter);
    }
    int offset = counter;
    for (int i = 0; i < counter; i++)
    {
        int change = 0;
        Cell** p_cells = p_hidden_triples[i].p_cells;
        int triples_values[3];
        for (int j = 0; j < 3; j++)
        {
            triples_values[j] = p_hidden_triples[i].values[j];
        }
        for (int j = 0; j < 3; j++)
        {
            int index = p_hidden_triples[i].indices[j];
            for (int k = 1; k < BOARD_SIZE+1; k++)
            {
                if (is_candidate(p_cells[index], k))
                {
                    if (k != triples_values[0] && k != triples_values[1] && k != triples_values[2])
                    {
                        unset_candidate(p_cells[index], k);
                        change = 1;
                    }                     
                }
            }
        }
        offset -= change;
    }
    return counter - offset;
}