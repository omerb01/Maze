/*
 * main.c
 *
 *  Created on: 15 בספט׳ 2017
 *      Author: Omer
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/********** Define maze HERE ***************/
#define ROWS 200
#define COLUMNS 20
#define DO_STEP_BY_STEP_GENERATING 0
#define DO_STEP_BY_STEP_SOLVING 0
/*******************************************/

#define ROWS_FOR_CODE (ROWS*2+1)
#define COLUMNS_FOR_CODE (COLUMNS*2+1)
#define STARTING_POINT_INDEX ROWS_FOR_CODE*COLUMNS_FOR_CODE-(COLUMNS_FOR_CODE)+1

// MACRO ANSI COLORS
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct
{
	int array[ROWS_FOR_CODE][COLUMNS_FOR_CODE];
	int rows;
	int columns;
	int is_solved;
} Maze;

typedef struct
{
	int currect_index;
	int up_index;
	int right_index;
	int down_index;
	int left_index;
} Location;

enum maze_values
{
	DEAD_END = -1, BLANK, BLOCK, SEEKER, BUILDER, START, END
};
enum directions
{
	UP, RIGHT, DOWN, LEFT
};
enum indicators
{
	ERROR = -1
};

// ######## MAZE INITIALIZE ############ //
void build_matrix_template(Maze* maze);

// ######## MAZE PRESENTATION ########## //
int print_maze(Maze* maze);
int refresh_screen(Maze* maze, int do_pause);
int update_maze(Maze* maze, int index, int value, int to_refresh);

// ######## MAZE SOLUTION ############## //
void solve_maze(Maze* maze, int currect_index, int do_step_by_step);

// ######## MAZE GENERATION ############ //
int* find_all_locations_by_maze_value(Maze* maze, Location* location, int value,
		int* counter);
int randInRange(int min, int max);
int random_location_by_maze_value(Maze* maze, Location* location, int value,
		int* next_index, int* next_direction);
int build_path(Maze* maze, Location* location, int next_direction, int value);
void build_maze_recursion(Maze* maze, int currect_index, int do_step_by_step);
void build_maze(Maze* maze, int currect_index, int do_step_by_step);

// ######## MAZE SOLUTION & GENERATION ######### //
int build_currect_location(Maze* maze, Location* location, int index);

int print_maze(Maze* maze)
{
	// purpose: prints the maze matrix into the console screen
	// returns ERROR when detects illegal value in maze matrix

	printf("\n");

	for (int i = 0; i < maze->rows; i++)
	{
		for (int j = 0; j < maze->columns; j++)
		{
			printf(" ");

			if (maze->array[i][j] == DEAD_END || maze->array[i][j] == BLANK)
				printf(" ");
			else if (maze->array[i][j] == BLOCK)
				printf("#");
			else if (maze->array[i][j] == SEEKER)
				printf(ANSI_COLOR_YELLOW "@" ANSI_COLOR_RESET);
			else if (maze->array[i][j] == BUILDER)
				printf(ANSI_COLOR_CYAN "@" ANSI_COLOR_RESET);
			else if (maze->array[i][j] == START)
				printf(ANSI_COLOR_GREEN "A" ANSI_COLOR_RESET);
			else if (maze->array[i][j] == END)
				printf(ANSI_COLOR_GREEN "B" ANSI_COLOR_RESET);
			else
				return ERROR; // illegal maze
		}
		printf("\n");
	}

	printf("\n");

	return 0;
}

int refresh_screen(Maze* maze, int do_pause)
{
	// purpose: clears the console screen and prints the maze matrix once again
	// returns ERROR when detects illegal value in maze matrix

	system("cls"); // clears console screen
	if (print_maze(maze) == ERROR)
		return ERROR;

	if (do_pause)
		system("pause");

	return 0;
}

int update_maze(Maze* maze, int index, int value, int to_refresh)
{
	// purpose: updates a value in the maze matrix and refreshes the console screen
	// returns ERROR when detects illegal value in maze matrix

	maze->array[0][index] = value;

	if (to_refresh)
		if (refresh_screen(maze, 1) == ERROR)
			return ERROR;

	return 0;
}

int build_currect_location(Maze* maze, Location* location, int index)
{
	// purpose: builds directions array which include indexes UP, RIGHT, DOWN and LEFT of a specific index

	location->currect_index = index;
	location->up_index = index - (maze->columns);
	location->right_index = index + 1;
	location->down_index = index + (maze->columns);
	location->left_index = index - 1;

	return 0;
}

void solve_maze(Maze* maze, int currect_index, int do_step_by_step)
{
	// purpose: solves the maze recursively with backtracking using "Depth First Search" algorithm
	// 			"currect_index" represents number between 0 to rows*columns

	Location currect_location;
	build_currect_location(maze, &currect_location, currect_index);

#define MAZE_VALUE_FOR_SOLVE_MAZE(index_parameter) \
		(maze->array[0][currect_location.index_parameter])

	// recursion termination
	if ((MAZE_VALUE_FOR_SOLVE_MAZE(up_index) == END
			|| MAZE_VALUE_FOR_SOLVE_MAZE(right_index) == END
			|| MAZE_VALUE_FOR_SOLVE_MAZE(down_index) == END
			|| MAZE_VALUE_FOR_SOLVE_MAZE(left_index) == END)
		&& MAZE_VALUE_FOR_SOLVE_MAZE(currect_index) != START)
	{
		maze->is_solved = 1;
		return;
	}

	if (maze->is_solved == 0 && MAZE_VALUE_FOR_SOLVE_MAZE(up_index) == BLANK)
	{
		int next_index = currect_location.up_index;
		update_maze(maze, next_index, SEEKER, do_step_by_step);
		solve_maze(maze, next_index, do_step_by_step);
	}
	if (maze->is_solved == 0 && MAZE_VALUE_FOR_SOLVE_MAZE(right_index) == BLANK)
	{
		int next_index = currect_location.right_index;
		update_maze(maze, next_index, SEEKER, do_step_by_step);
		solve_maze(maze, next_index, do_step_by_step);
	}
	if (maze->is_solved == 0 && MAZE_VALUE_FOR_SOLVE_MAZE(down_index) == BLANK)
	{
		int next_index = currect_location.down_index;
		update_maze(maze, next_index, SEEKER, do_step_by_step);
		solve_maze(maze, next_index, do_step_by_step);
	}
	if (maze->is_solved == 0 && MAZE_VALUE_FOR_SOLVE_MAZE(left_index) == BLANK)
	{
		int next_index = currect_location.left_index;
		update_maze(maze, next_index, SEEKER, do_step_by_step);
		solve_maze(maze, next_index, do_step_by_step);
	}

	if (maze->is_solved == 0)
		update_maze(maze, currect_location.currect_index, DEAD_END,
				do_step_by_step);
}

int* find_all_locations_by_maze_value(Maze* maze, Location* location, int value,
		int* counter)
{
	// purpose: builds an array of all direction by a specific maze_value,
	//			updates "counter" for how many directions found,
	//			completes the array by "ERROR"s

	// returns NULL and counter=0 when there's no direction
	// return NULL and counter=ERROR when something got wrong with the memory

	int* result = malloc(4 * sizeof(int));
	if (result == NULL)
	{
		*counter = ERROR;
		return NULL;
	}

	for (int i = 0; i < 4; i++) // init array
		result[i] = ERROR;

	*counter = 0;

	if (maze->array[0][location->up_index] == value)
	{
		result[*counter] = location->up_index;
		(*counter)++;
	}
	if (maze->array[0][location->right_index] == value)
	{
		result[*counter] = location->right_index;
		(*counter)++;
	}
	if (maze->array[0][location->down_index] == value)
	{
		result[*counter] = location->down_index;
		(*counter)++;
	}
	if (maze->array[0][location->left_index] == value)
	{
		result[*counter] = location->left_index;
		(*counter)++;
	}

	if (*counter == 0)
	{
		free(result);
		return NULL;
	}

	return result;
}

int randInRange(int min, int max)
{
	// purpose: returns a random number between "min" and "max"
	// NOTE: rand() function need to be seeded first, before use

	return min + rand() % (max + 1 - min);
}

int random_location_by_maze_value(Maze* maze, Location* location, int value,
		int* next_index, int* next_direction)
{
	// purpose: finds a random direction by a specific maze value
	//			updates "nex_index" for the maze index that has been chosen
	//			updates "next_direction" for the direction code that has been chosen
	// returns ERROR when can't find a direction

	int counter;
	int* temp_array = find_all_locations_by_maze_value(maze, location, value,
			&counter);
	if (temp_array == NULL)
	{
		*next_index = ERROR;
		*next_direction = ERROR;
		return ERROR;
	}

	int result = temp_array[randInRange(0, counter - 1)];
	free(temp_array);

	if (location->up_index == result)
		*next_direction = UP;
	else if (location->right_index == result)
		*next_direction = RIGHT;
	else if (location->down_index == result)
		*next_direction = DOWN;
	else if (location->left_index == result)
		*next_direction = LEFT;

	*next_index = result;
	return 0;
}

int build_path(Maze* maze, Location* location, int next_direction, int value)
{
	// purpose: builds BLOCKs path instead of inputed maze value
	// returns ERROR when something's wrong with the inputed direction

	if (next_direction == UP || next_direction == DOWN)
	{
		if (maze->array[0][location->right_index] == value)
			update_maze(maze, location->right_index, BLOCK, 0);
		if (maze->array[0][location->left_index] == value)
			update_maze(maze, location->left_index, BLOCK, 0);
	}
	else if (next_direction == RIGHT || next_direction == LEFT)
	{
		if (maze->array[0][location->up_index] == value)
			update_maze(maze, location->up_index, BLOCK, 0);
		if (maze->array[0][location->down_index] == value)
			update_maze(maze, location->down_index, BLOCK, 0);
	}
	else
		return ERROR;

	return 0;
}

void build_maze_recursion(Maze* maze, int currect_index, int do_step_by_step)
{
	// purpose: builds maze recursively with backtracking using "Depth First Search" algorithm

	Location currect_location;
	build_currect_location(maze, &currect_location, currect_index);

	int next_index;
	int next_direction;
	if (random_location_by_maze_value(maze, &currect_location, DEAD_END,
			&next_index, &next_direction) == ERROR)
		return;

	build_path(maze, &currect_location, next_direction, DEAD_END);
	update_maze(maze, next_index, BUILDER, do_step_by_step);
	build_maze_recursion(maze, next_index, do_step_by_step);
	update_maze(maze, next_index, BLANK, do_step_by_step);

	// another session for continue build paths after got stuck once
	if (random_location_by_maze_value(maze, &currect_location, DEAD_END,
			&next_index, &next_direction) == ERROR)
		return;

	build_path(maze, &currect_location, next_direction, DEAD_END);
	update_maze(maze, next_index, BUILDER, do_step_by_step);
	build_maze_recursion(maze, next_index, do_step_by_step);
	update_maze(maze, next_index, BLANK, do_step_by_step);

}

void build_maze(Maze* maze, int currect_index, int do_step_by_step)
{
	// purpose: defines END point after the recursive function is done;

	build_maze_recursion(maze, currect_index, do_step_by_step);

	for (int i = maze->columns; i > 0; i--)
	{
		if (maze->array[0][i + maze->columns] == BLANK)
		{
			maze->array[0][i] = END;
			break;
		}
	}
}

void build_matrix_template(Maze* maze)
{
	for (int i = 0; i < maze->rows; i++)
	{
		for (int j = 0; j < maze->columns; j++)
		{
			if (i == 0 || j == 0 || i == ROWS_FOR_CODE - 1
					|| j == COLUMNS_FOR_CODE - 1)
				maze->array[i][j] = BLOCK;
			else if (i % 2 == 0 && j % 2 == 0)
				maze->array[i][j] = BLOCK;
			else
				maze->array[i][j] = DEAD_END;
		}
	}

	maze->array[0][STARTING_POINT_INDEX] = START;

	srand(time(NULL)); // random function's seed
}

int main()
{
	// purpose: main function

	// Initializes maze struct
	Maze maze;
	maze.rows = ROWS_FOR_CODE;
	maze.columns = COLUMNS_FOR_CODE;
	maze.is_solved = 0;
	build_matrix_template(&maze);

	// builds the maze
	build_maze(&maze, STARTING_POINT_INDEX, DO_STEP_BY_STEP_GENERATING);
	refresh_screen(&maze, 0);

	printf("START\n\n");
	system("pause");

	// solves the maze
	solve_maze(&maze, STARTING_POINT_INDEX, DO_STEP_BY_STEP_SOLVING);
	refresh_screen(&maze, 1);

	return 0;
}
