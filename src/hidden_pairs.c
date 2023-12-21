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

void find_hidden_pairs_value(Cell** p_cells, int count, int possible_hidden_values[], 
                            int index, HiddenPairs *p_hidden_pairs, int* p_counter)
{
    for (int i = 0; i < count-1; i++)
    {
        for (int j = i+1; j < count; j++)
        {
            for (int k = index+1; k < BOARD_SIZE; k++)
            {
                if (is_candidate(p_cells[k], possible_hidden_values[i]) && is_candidate(p_cells[k], possible_hidden_values[j]))
                {
                    p_hidden_pairs[(*p_counter)++] = (HiddenPairs){p_cells[index], p_cells[k], {possible_hidden_values[i], possible_hidden_values[j]}};
                }
            }
        }
    }
}

// find hidden pairs in a row, in a collumn or in a box
void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs,
                        int *p_counter)
{  
    for (int i = 0; i < BOARD_SIZE-1; i++)
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

    int offset = counter;
    for (int i = 0; i < counter; i++)
    {
        int is_change = 0;
        Cell* p_cell_1 = p_hidden_pairs[i].p_cell_1;

        if (p_cell_1->num_candidates > 2)
        {
            set_candidates(p_cell_1, p_hidden_pairs[i].values, 2);
            is_change = 1;
        }
        Cell* p_cell_2 = p_hidden_pairs[i].p_cell_2;

        if (p_cell_2->num_candidates > 2)
        {
            set_candidates(p_cell_2, p_hidden_pairs[i].values, 2);
            is_change = 1;
        }
        offset -= is_change;
    }

    return counter - offset;
}
