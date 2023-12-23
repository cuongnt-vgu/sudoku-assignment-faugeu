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

int check_cell_in_triples(Cell* p_cell, int triples[3])
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
        else if (check_cell_in_triples(p_cells[i], possible_triples)) indices[cell_counter++] = i;
    }
    return cell_counter == 3;
}

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter)
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
                    p_naked_triples[(*p_counter)++] = (NakedTriples){p_cells, {indices[0], indices[1], indices[2]}, 
                                                        {possible_triples[i], possible_triples[j], possible_triples[k]}};
                }
            }
        }
    }
}

int check_solved_triples(NakedTriples p_naked_triple, NakedTriples *solved_triples, int solved_counter)
{
    Cell **p_cells = p_naked_triple.p_cells;
    for (int i = 0; i < solved_counter; i++)
    {
        Cell **solved_cells = solved_triples[i].p_cells;
        if (p_cells[p_naked_triple.indices[0]] == solved_cells[solved_triples[i].indices[0]] &&
            p_cells[p_naked_triple.indices[1]] == solved_cells[solved_triples[i].indices[1]] &&
            p_cells[p_naked_triple.indices[2]] == solved_cells[solved_triples[i].indices[2]])
        {
            return 1;
        }
    }
    return 0;
}

int naked_triples(SudokuBoard *p_board)
{
    int counter = 0;

    NakedTriples p_naked_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triples(p_board->p_rows[i], p_naked_triples, &counter);
        find_naked_triples(p_board->p_cols[i], p_naked_triples, &counter);
        find_naked_triples(p_board->p_boxes[i], p_naked_triples, &counter);
    }

    NakedTriples solved_triples[counter];
    int solved_counter = 0;
    int offset = 0;

    for (int i = 0; i < counter; i++)
    {
        if (check_solved_triples(p_naked_triples[i], solved_triples, solved_counter)) offset++;

        Cell** p_cells = p_naked_triples[i].p_cells;
        solved_triples[solved_counter++] = p_naked_triples[i];
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (j != p_naked_triples[i].indices[0] && j != p_naked_triples[i].indices[1] && j != p_naked_triples[i].indices[2])
            {
                for (int k = 0; k < 3; k++)
                {
                    if (is_candidate(p_cells[j], p_naked_triples[i].values[k]))
                    {
                        unset_candidate(p_cells[j], p_naked_triples[i].values[k]);
                    }
                }
            }
        }

    }
    return counter - offset;
}