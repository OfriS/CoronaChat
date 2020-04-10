
/* 
 * This module implements the input and output with the client.
 * It prints the messages to the user, and get the messages to send.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "ui_internals.h"
#include "../coronachat_status.h"

/*** Functions ***/

enum coronachat_status
UI__get_message(char *input_buffer, int max_y, bool *is_done, size_t *char_counter)
{
	enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
	int function_result = C_FAILURE;

	/* Validates the arguments. */
	if ((NULL == is_done) ||
		(NULL == input_buffer) ||
		(NULL == char_counter)) {
		status = CORORNACHAT_STATUS_UI_GET_MESSAGE_INVALID_PARAMETERS;
        DEBUG_PRINT_WITH_ERRNO("UI__get_massage got invalid parameters :(\n");
        goto l_cleanup;
	}

	/* Read the input from the user. */
	function_result = read(STDIN_FILENO, input_buffer, sizeof(*input_buffer));
	if (sizeof(char) != function_result) {
		status = CORORNACHAT_STATUS_UI_GET_MESSAGE_READ_FAILED;
        DEBUG_PRINT_WITH_ERRNO("Reading %d bytes instead of %d\n",
                               function_result,
                               sizeof(char));
        goto l_cleanup;
	}

	/* Prints the input to the user in the appropriate place. */
	function_result = mvprintw(max_y - 2, *char_counter, input_buffer);
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_GET_MESSAGE_PRINT_INPUT_FAILED;
        DEBUG_PRINT_WITH_ERRNO("mvprintw returned %d\n",
                               function_result);
        goto l_cleanup;
	}

	function_result = refresh();
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_GET_MESSAGE_REFRESH_FAILED;
        DEBUG_PRINT_WITH_ERRNO("refresh returned %d\n",
                               function_result);
        goto l_cleanup;
	}
	//TODO - use the print function?

	++*char_counter;

	/* Check if the message is done - '\n' is the last char or there is not enough place in the input box. */
	if ((SEND_MESSAGE_CHAR == *input_buffer) ||
		(INPUT_LINE_END_X <= *char_counter)) {
		*is_done = true;
	}
	else {
		*is_done = false;
	}

	/* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
	return status;
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

enum coronachat_status
UI__init_screen(int *max_x, int *max_y)
{
	enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
	int function_result = C_FAILURE;
	WINDOW *initscr_result = NULL;

	initscr_result = initscr();
	if (NULL == initscr_result) {
		status = CORORNACHAT_STATUS_UI_INIT_SCREEN_INITSCR_FAILED;
		DEBUG_PRINT_WITH_ERRNO("initscr failed\n");
		goto l_cleanup;
	}
	
	function_result = noecho();
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_INIT_SCREEN_NOECHO_FAILED;
		DEBUG_PRINT_WITH_ERRNO("noecho failed\n");
		goto l_cleanup;
	}

	/* Do not see where the curs is. */
	/* If the window does not support this option - do NOT fail.*/
	curs_set(FALSE);

	/* Get the window sizes. */
	/* Returns void. */
	getmaxyx(stdscr, *max_y, *max_x);

	/* The max string size. */
	g_max_message_length = *max_y - MAX_NAME_LENGTH;

	/* Print the input box. */
	status = print_input_box(*max_y);
	if (CORORNACHAT_STATUS_SUCCESS != status) {
		goto l_cleanup;
	}

	status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
	return status;
}

enum coronachat_status
UI__destroy_screen(void)
{
	enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
	int function_result = C_FAILURE;

	function_result = clear();
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_DESTROY_SCREEN_CLEAR_FAILED;
		DEBUG_PRINT_WITH_ERRNO("clear screen failed.\n");
	}

	function_result = endwin();
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_DESTROY_SCREEN_ENDWIN_FAILED;
		DEBUG_PRINT_WITH_ERRNO("endwin failed\n");
		goto l_cleanup;
	}

	status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
	return status;
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

enum coronachat_status
print_input_box(int lines_number)
{
	enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
	int box_start_line = 0;
	int function_result = C_FAILURE;

	box_start_line = lines_number - INPUT_LINES_NUMBER;

	if (0 >= box_start_line) {
		status = CORORNACHAT_STATUS_UI_PRINT_INPUT_BOX_INVALID_PARAMETERS;
		DEBUG_PRINT_WITH_ERRNO("The lines number is invalid or too small.\n");
		goto l_cleanup;
	}

	/* Print the box up and down lines. */
	function_result = mvprintw(box_start_line, 0, INPUT_LINE_FRAME_GRAPHIC);
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_PRINT_INPUT_BOX_PRINT_FAILED;
        DEBUG_PRINT_WITH_ERRNO("mvprintw returned %d\n",
                               function_result);
        goto l_cleanup;
	}

	function_result = mvprintw(box_start_line + 2, 0, INPUT_LINE_FRAME_GRAPHIC);
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_PRINT_INPUT_BOX_PRINT_FAILED;
        DEBUG_PRINT_WITH_ERRNO("mvprintw returned %d\n",
                               function_result);
        goto l_cleanup;
	}

	/* Print the closing input line. */
	function_result = mvprintw(box_start_line + 1, INPUT_LINE_END_X, INPUT_LINE_END_GRAPHIC);
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_PRINT_INPUT_BOX_PRINT_FAILED;
        DEBUG_PRINT_WITH_ERRNO("mvprintw returned %d\n",
                               function_result);
        goto l_cleanup;
	}

	function_result = refresh();
	if (C_SUCCESS != function_result) {
		status = CORORNACHAT_STATUS_UI_PRINT_INPUT_BOX_REFRESH_FAILED;
        DEBUG_PRINT_WITH_ERRNO("refresh returned %d\n",
                               function_result);
        goto l_cleanup;
	}

	status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
	return status;
}
