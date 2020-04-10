
/* 
 * This module implements the input and output with the client.
 * It prints the messages to the user, and get the messages to send.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ui_internals.h"

/*** Functions ***/

bool UI__get_message(char **input_buffer, size_t *input_size)
{
	bool result = false;

	result = true;

l_cleanup:
	return result;
}

bool UI__print_message(char *message, size_t message_length, int max_y, int *y)
{
	bool result = false;

	/* Check if there is an empty line. */
	if ((max_y - INPUT_LINES_NUMBER) < *y) {
		--(*y);

		/* Create an empty line. */
		create_empty_line(*y);
		//TODO - check return value
	}

	print_message(&g_current_line, max_y, message);
	//TODO - check return value

	result = true;

l_cleanup:
	return result;
}

bool UI__init_screen(int *max_x, int *max_y)
{
	bool result = false;

	initscr();
	//TODO check result
	
	noecho();

	/* Do not see where the curs is. */
	curs_set(FALSE);

	/* Clear the screen. */
	clear();

	/* Print the input box. */
	//TODO
	
	/* Get the window sizes. */
	getmaxyx(stdscr, *max_y, *max_x);

	/* The max string size. */
	g_max_message_length = *max_y - MAX_NAME_LENGTH;

	result = true;

l_cleanup:
	return result;
}

bool UI__destroy_screen(void)
{
	bool result = false;
	int function_result = C_FAILURE;

	function_result = clear();
	if (C_SUCCESS != function_result) {
		goto l_cleanup;
	}

	function_result = endwin();
	if (C_SUCCESS != function_result) {
		goto l_cleanup;
	}

	result = true;

l_cleanup:
	return result;
}

bool create_empty_line(int y)
{
	bool result = false;
	int current_line = 0;
	char *current_message = NULL;

	//TODO - check the argument

	/* Allocate place to the already written messages. */
	current_message = calloc(sizeof(char), g_max_message_length);
	//TODO - normal calloc 

	while (current_line < y) {
		++current_line;
		
		/* Get the message in the current line. */
		mvinnstr(current_line - 1, 0, (char *)current_message, g_max_message_length);

		/* Copy the message to the above line. */
		move(current_line, 0);
		clrtoeol();
		mvprintw(current_line, 0, current_message);
	}

	/* Erase the last line. */
	move(y, 0);
	clrtoeol();

	result = true;

l_cleanup:
	return result;
}

bool print_message(int *y, int max_y, char *message)
{
	bool result = false;

	//TODO - check arguments
	
	/* (*y) is the number of the line I want to write into. */
	//TODO - add the checking of the message length

	/* Print the message in the empty line. */
	mvprintw(*y, 0, message);
	refresh();
	//TODO - check return values

	/* Change the next line number to write into. */
	++(*y);

	result = true;

l_cleanup:
	return result;
}

bool print_input_line(int line_length)
{
	bool result = false;

	

	result = true;

l_cleanup:
	return result;
}
