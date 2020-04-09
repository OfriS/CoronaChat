
/* 
 * This module implements the input and output with the client.
 * It prints the messages to the user, and get the messages to send.
 */

#include <stdio.h>


/*** Functions ***/

bool UI__get_message(char **input_buffer, size_t *input_size)
{
	bool result = false;

	result = true;

l_cleanup:
	return result;
}

bool UI__print_message(char *message, size_t message_length)
{
	bool result = false;

	result = true;

l_cleanup:
	return result;
}

bool UI__init_screen(int *max_x, int *max_y)
{
	bool result = false;

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

bool create_empty_line(int *y, int max_y)
{
	bool result = false;

	result = true;

l_cleanup:
	return result;
}

bool print_message(int *y, int max_y, char *message)
{
	bool result = false;

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
