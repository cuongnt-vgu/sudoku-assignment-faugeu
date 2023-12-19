#include <stdio.h>
#include <stdlib.h>
#include "hidden_pairs.h"


int check_hidden_pairs(Cell **p_cells, int value)
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (is_candidate(p_cells[i], value)) count += 1;
    }
    return (count == 2);
}

void find_hidden_pairs_value(Cell** p_cells, int count, int possible_hidden_values[], int index, HiddenPairs *p_hidden_pairs, int* p_counter)
{
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (j == index) continue;
            if (is_candidate(p_cells[j], possible_hidden_values[i]))
            {
                for (int k = i+1; k < count; k++)
                {
                    if (is_candidate(p_cells[j], possible_hidden_values[k]))
                    {
                        p_hidden_pairs[(*p_counter)++] = (HiddenPairs){p_cells[index], {possible_hidden_values[i], possible_hidden_values[k]}};
                        return;
                    }
                }
            }
        }
    }
}

// find hidden pairs in a row, in a collumn or in a box
void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs,
                        int *p_counter)
{  
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates < 2) continue;

        int count = 0;
        int possible_hidden_values[BOARD_SIZE];
        int* candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            if (check_hidden_pairs(p_cells, candidates[j]))
            {
                possible_hidden_values[count++] = candidates[j];
            }
        }
        if (count >= 2)
        {
            find_hidden_pairs_value(p_cells, count, possible_hidden_values, i, p_hidden_pairs, p_counter);
        }
        free(candidates);
    }

}

int hidden_pairs(SudokuBoard *p_board)
{
    int counter = 0;

    HiddenPairs p_hidden_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_pairs(p_board->p_rows[i], p_hidden_pairs, &counter);
        find_hidden_pairs(p_board->p_cols[i], p_hidden_pairs, &counter);
        find_hidden_pairs(p_board->p_boxes[i], p_hidden_pairs, &counter);
    }

    int offset = 0;
    for (int i = 0; i < counter; i++)
    {
        Cell* p_cell = p_hidden_pairs[i].p_cell;
        if (p_cell->num_candidates > 2)
        {
            set_candidates(p_cell, p_hidden_pairs[i].values, 2);
        }
        else offset++;
    }

    return (counter-offset)/2;
}
