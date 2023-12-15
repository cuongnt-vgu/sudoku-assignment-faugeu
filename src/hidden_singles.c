#include "hidden_singles.h"
#include <stdlib.h>


int check_hidden_single_values(Cell **p_cells, int possible, int index)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (i == index || p_cells[i]->num_candidates == 1) continue;
        if (is_candidate(p_cells[i], possible)) return 0;
    }
    return 1;
}

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles,
                        int *p_counter)
{  
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_cells[i]->num_candidates > 1 && p_cells[i]->candidates[j] == 1)
            {
                if (check_hidden_single_values(p_cells, j+1, i))
                {
                    p_hidden_singles[(*p_counter)++] = (HiddenSingle){p_cells[i], j+1};
                    break;
                }
            }
        }
    }
}

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board)
{
    int counter = 0;
    HiddenSingle p_hidden_singles[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_rows[i], p_hidden_singles, &counter);
        find_hidden_single(p_board->p_cols[i], p_hidden_singles, &counter);
        find_hidden_single(p_board->p_boxes[i], p_hidden_singles, &counter);
    }
    int offset = 0;
    for (int i = 0; i < counter; i++)
    {
        if (p_hidden_singles[i].p_cell->num_candidates == 1) offset += 1;
        set_candidates(p_hidden_singles[i].p_cell,  (int *)(int[]){p_hidden_singles[i].value}, 1);
    }
    return counter - offset;
}