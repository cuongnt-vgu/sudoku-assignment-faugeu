#include "naked_pairs.h"
#include <stdlib.h>

int check_naked_pairs(Cell **p_cells, int i, int j)
{
    for (int k = 0; k < BOARD_SIZE; k++)
    {
        if (p_cells[i]->candidates[k] != p_cells[j]->candidates[k]) return 0;
    }
    return 1;
}

// find hidden single cells in a row, in a collumn or in a box
void find_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs,
                        int *p_counter, int is_box)
{  
    for (int i = 0; i < BOARD_SIZE-1; i++)
    {
        if (p_cells[i]->num_candidates == 2)
        {
            for (int j = i+1; j < BOARD_SIZE; j++)
            {
                if (p_cells[j]->num_candidates == 2)
                {
                    if (check_naked_pairs(p_cells, i, j))
                    {
                        if (!is_box || (j != i + 3 && j != i + 1) )
                        {
                            p_naked_pairs[(*p_counter)++] = (NakedPairs){p_cells, i};
                            break;
                        }
                    }
                }
            }
        }
    }
}

int naked_pairs(SudokuBoard *p_board)
{
    int counter = 0;

    NakedPairs p_naked_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_pairs(p_board->p_rows[i], p_naked_pairs, &counter, 0);
        find_naked_pairs(p_board->p_cols[i], p_naked_pairs, &counter, 0);
        find_naked_pairs(p_board->p_boxes[i], p_naked_pairs, &counter, 1);
    }
    for (int i = 0; i < counter; i++)
    {
        Cell** p_cells = p_naked_pairs[i].p_cells;
        int* candidates = get_candidates(p_cells[p_naked_pairs[i].index]);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if ((p_cells[j]->num_candidates > 2) ||
                (is_candidate(p_cells[j], candidates[0]) ^ is_candidate(p_cells[j], candidates[1])))
            {
                for (int k = 0; k < 2; k++)
                {
                    if (is_candidate(p_cells[j], candidates[k]))
                    {
                        unset_candidate(p_cells[j], candidates[k]);
                    }
                }
            }
        }
        free(candidates);

    }
    return counter;
}