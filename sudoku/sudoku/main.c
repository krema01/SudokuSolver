#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define SUDOKU_ROW_COLUMN_SIZE 9
#define SUDOKU_SIZE (SUDOKU_ROW_COLUMN_SIZE * SUDOKU_ROW_COLUMN_SIZE)
#define SUDOKU_SUB_GRID_ROW_COLUMN_SIZE 3
#define SUDOKU_POSSIBLE_VALUES 9

const int SUDOKU_EMPTY_VALUE = 0;

typedef enum sudoku_range
{
    sudoku_range_row,
    sudoku_range_column,
    sudoku_range_sub_grid
} sudoku_range;

bool is_valid_sudoku_range(const int grid[], int rangePos, sudoku_range sudoku_range)
{
    bool rowValues[SUDOKU_POSSIBLE_VALUES] = { 0 };
    for (int i = 0; i < SUDOKU_ROW_COLUMN_SIZE; i++)
    {
        int pos;
        switch (sudoku_range)
        {
            case sudoku_range_row:
            {
                pos = i + rangePos * SUDOKU_ROW_COLUMN_SIZE;
                break;
            }
            case sudoku_range_column:
            {
                pos = i * SUDOKU_ROW_COLUMN_SIZE + rangePos;
                break;
            }
            case sudoku_range_sub_grid:
            {
                int subRow = i / SUDOKU_SUB_GRID_ROW_COLUMN_SIZE;
                int subColumn = i - subRow * SUDOKU_SUB_GRID_ROW_COLUMN_SIZE;

                int rowOffset = (rangePos / SUDOKU_SUB_GRID_ROW_COLUMN_SIZE) * SUDOKU_SUB_GRID_ROW_COLUMN_SIZE;
                int columnOffset =
                    (rangePos - (rowOffset / SUDOKU_SUB_GRID_ROW_COLUMN_SIZE) * SUDOKU_SUB_GRID_ROW_COLUMN_SIZE) *
                    SUDOKU_SUB_GRID_ROW_COLUMN_SIZE;
                pos = (rowOffset + subRow) * SUDOKU_ROW_COLUMN_SIZE + columnOffset + subColumn;
                break;
            }
            default:
            {
                assert(false);
                return false;
            }
        }
        int value = grid[pos];
        if (value != SUDOKU_EMPTY_VALUE)
        {
            if (rowValues[value - 1])
            {
                return false;
            }

            rowValues[value - 1] = true;
        }
    }

    return true;
}

bool is_valid_sudoku_row(const int grid[], int row)
{
    return is_valid_sudoku_range(grid, row, sudoku_range_row);
}

bool is_valid_sudoku_column(const int grid[], int row)
{
    return is_valid_sudoku_range(grid, row, sudoku_range_column);
}

bool is_valid_sudoku_sub_grid(const int grid[], int row)
{
    return is_valid_sudoku_range(grid, row, sudoku_range_sub_grid);
}

bool is_valid_sudoku(const int grid[])
{
    for (int i = 0; i < SUDOKU_ROW_COLUMN_SIZE; i++)
    {
        if (!is_valid_sudoku_row(grid, i) ||
            !is_valid_sudoku_column(grid, i) ||
            !is_valid_sudoku_sub_grid(grid, i)
        )
        {
            return false;
        }
    }

    return true;
}

bool sudoku_rec(int grid[], int pos)
{
    if (pos == SUDOKU_SIZE)
    {
        return is_valid_sudoku(grid);
    }

    if (grid[pos] == SUDOKU_EMPTY_VALUE)
    {
        for (int i = 1; i <= SUDOKU_POSSIBLE_VALUES; i++)
        {
            grid[pos] = i;
            if (is_valid_sudoku(grid))
            {
                if (sudoku_rec(grid, pos + 1))
                {
                    return true;
                }
            }
        }

        grid[pos] = SUDOKU_EMPTY_VALUE;
    }
    else
    {
        if (sudoku_rec(grid, pos + 1))
        {
            return true;
        }
        
    }

    return false;
}

void sudoku(int grid[])
{
    sudoku_rec(grid, 0);
}

void print_sudoku(int grid[])
{
    for (int row = 0; row < SUDOKU_ROW_COLUMN_SIZE; row++)
    {
        if (row != 0 && row % 3 == 0)
        {
            printf("\n");
        }

        for (int column = 0; column < SUDOKU_ROW_COLUMN_SIZE; column++)
        {
            if (column != 0 && column % 3 == 0)
            {
                printf(" ");
            }

            printf("%d ", grid[row * SUDOKU_ROW_COLUMN_SIZE + column]);
        }

        printf("\n");
    }
}

void test_solve_sudoku(int grid[])
{
    printf("Solve sudoku: \n");
    print_sudoku(grid);
    clock_t timer = clock();
    sudoku(grid);
    double durationMs = ((double)clock() - (double)timer) * 1000 / CLOCKS_PER_SEC;
    printf("\nSolved in %g ms:\n", durationMs);
    print_sudoku(grid);
    printf("\n\n");
}

void test_easy_sudoku()
{
    int sudokuEasy[SUDOKU_SIZE] =
    {
        3, 1, 6, 4, 0, 8, 0, 2, 5,
        0, 8, 9, 6, 1, 0, 3, 4, 7,
        7, 0, 2, 9, 5, 3, 0, 6, 1,
        2, 6, 8, 0, 9, 1, 4, 0, 3,
        1, 9, 0, 8, 4, 7, 2, 5, 0,
        4, 7, 5, 3, 0, 6, 1, 9, 8,
        0, 2, 4, 1, 6, 5, 7, 3, 9,
        6, 3, 7, 0, 8, 9, 5, 1, 0,
        9, 5, 0, 7, 3, 0, 6, 8, 2
    };
    printf("Test easy sudoku:\n");
    test_solve_sudoku(sudokuEasy);
}

void test_medium_sudoku()
{
    int sudokuMedium[SUDOKU_SIZE] =
    {
        3, 0, 0, 5, 0, 0, 9, 0, 8,
        0, 9, 2, 0, 4, 8, 0, 3, 0,
        5, 0, 6, 9, 3, 0, 4, 0, 1,
        0, 3, 1, 0, 9, 7, 5, 6, 0,
        2, 0, 0, 8, 1, 0, 0, 4, 9,
        0, 5, 9, 0, 0, 3, 0, 8, 0,
        9, 0, 0, 6, 0, 1, 7, 0, 3,
        1, 0, 5, 0, 8, 4, 0, 9, 6,
        0, 2, 3, 7, 5, 0, 8, 0, 0
    };
    printf("Test medium sudoku:\n");
    test_solve_sudoku(sudokuMedium);
}

void test_hard_sudoku()
{
    int sudokuHard1[SUDOKU_SIZE] =
    {
        4, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 1, 5, 0, 4, 0, 6, 0, 0,
        0, 0, 0, 0, 0, 7, 0, 0, 0,
        0, 0, 0, 2, 1, 0, 0, 0, 8,
        0, 0, 0, 7, 3, 4, 0, 0, 2,
        0, 3, 0, 0, 8, 0, 0, 0, 0,
        8, 0, 1, 9, 0, 0, 0, 4, 5,
        0, 4, 0, 0, 0, 0, 7, 0, 0,
        2, 0, 0, 0, 0, 3, 0, 0, 0
    };

    int sudokuHard2[SUDOKU_SIZE] =
    {
        9, 0, 0, 0, 0, 3, 0, 0, 0,
        2, 0, 7, 0, 9, 0, 8, 0, 0,
        0, 0, 0, 0, 0, 2, 5, 9, 0,
        3, 0, 0, 0, 0, 6, 0, 0, 0,
        0, 0, 0, 0, 5, 0, 7, 3, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 2,
        8, 0, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 8, 0, 3, 0, 0,
        0, 9, 0, 7, 3, 5, 0, 4, 0
    };

    printf("Test hard sudoku:\n");
    test_solve_sudoku(sudokuHard1);
    printf("Test hard sudoku2:\n");
    test_solve_sudoku(sudokuHard2);
}

void test_expert_sudoku()
{
    int sudokuExpert[SUDOKU_SIZE] =
    {
        0, 5, 0, 7, 6, 0, 0, 0, 9,
        0, 2, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 7, 9, 0, 0, 5, 0, 0,
        0, 1, 0, 0, 0, 2, 0, 0, 0,
        6, 0, 0, 0, 0, 1, 0, 0, 7,
        8, 7, 0, 5, 0, 0, 0, 0, 3,
        0, 0, 0, 3, 0, 0, 0, 4, 0,
        4, 0, 0, 0, 5, 0, 8, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 6
    };
    printf("Test expert sudoku:\n");
    test_solve_sudoku(sudokuExpert);
}

void test_invalid_sudoku()
{
    int sudokuInvalid[SUDOKU_SIZE] =
    {
        3, 0, 0, 5, 0, 0, 9, 0, 8,
        0, 9, 2, 0, 4, 8, 0, 3, 0,
        5, 0, 6, 9, 3, 0, 4, 0, 1,
        0, 3, 1, 0, 9, 7, 0, 5, 6,
        2, 0, 0, 8, 1, 0, 0, 4, 9,
        0, 5, 9, 0, 0, 3, 0, 8, 0,
        9, 0, 0, 6, 0, 1, 7, 0, 3,
        1, 0, 5, 0, 8, 4, 0, 9, 6,
        0, 2, 3, 7, 5, 0, 8, 0, 0
    };
    printf("Test invalid sudoku:\n");
    test_solve_sudoku(sudokuInvalid);
}

void test_already_solved_sudoku()
{
    int testSudoku[SUDOKU_SIZE] =
    {
        3, 0, 0, 5, 0, 0, 9, 0, 8,
        0, 9, 2, 0, 4, 8, 0, 3, 0,
        5, 0, 6, 9, 3, 0, 4, 0, 1,
        0, 3, 1, 0, 9, 7, 5, 6, 0,
        2, 0, 0, 8, 1, 0, 0, 4, 9,
        0, 5, 9, 0, 0, 3, 0, 8, 0,
        9, 0, 0, 6, 0, 1, 7, 0, 3,
        1, 0, 5, 0, 8, 4, 0, 9, 6,
        0, 2, 3, 7, 5, 0, 8, 0, 0
    };

    sudoku(testSudoku);
    printf("Test already solved sudoku:\n");
    test_solve_sudoku(testSudoku);
}

int main()
{
    test_easy_sudoku();
    test_medium_sudoku();
    test_hard_sudoku();
    test_expert_sudoku();
    test_invalid_sudoku();
    test_already_solved_sudoku();

    return EXIT_SUCCESS;
}
