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
                        int *p_counter)
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
                        p_naked_pairs[(*p_counter)++] = (NakedPairs){p_cells, i, j};
                    }
                }
            }
        }
    }
}

int check_solved_pairs(NakedPairs p_naked_pair, NakedPairs* solved_pairs, int solved_count)
{
    Cell **p_cells = p_naked_pair.p_cells;
    int i = p_naked_pair.index1;
    int j = p_naked_pair.index2;
    for (int k = 0; k < solved_count; k++)
    {
        Cell **solved_cells = solved_pairs[k].p_cells;
        int i_solved = solved_pairs[k].index1;
        int j_solved = solved_pairs[k].index2;
        if (p_cells[i] == solved_cells[i_solved] && p_cells[j] == solved_cells[j_solved])
        {
            return 1;
        }
    }
    return 0;
}

int naked_pairs(SudokuBoard *p_board)
{
    int counter = 0;

    NakedPairs p_naked_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_pairs(p_board->p_rows[i], p_naked_pairs, &counter);
        find_naked_pairs(p_board->p_cols[i], p_naked_pairs, &counter);
        find_naked_pairs(p_board->p_boxes[i], p_naked_pairs, &counter);
    }

    NakedPairs solved_pairs[counter];
    int solved_count = 0;
    int offset = 0;
    for (int i = 0; i < counter; i++)
    {
        if (check_solved_pairs(p_naked_pairs[i], solved_pairs, solved_count)) offset++;
        else solved_pairs[solved_count++] = p_naked_pairs[i];

        Cell** p_cells = p_naked_pairs[i].p_cells;
        int* candidates = get_candidates(p_cells[p_naked_pairs[i].index1]);
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
    return counter - offset;
}