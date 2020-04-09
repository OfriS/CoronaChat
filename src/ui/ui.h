
/* The header of the ui module. */
/* This module implements the input and output with the client. */

#include <stdio.h>

/*** Constants ***/



/*** Functions ***/

/** @brief - The function gets input from the client, if there is.
 *
 * @param OUT input_buffer
 * @param OUT input_size
 *
 * @return - if the function succeed.
 *
 * @note - if there wasn't message - return True with Null arguments.
 */
bool get_message(char **input_buffer, size_t *input_size);

/** @brief - The function prints messages on the chat screen.
 *
 * @param IN message
 * @param IN message_length
 *
 * @return - if the function succeed.
*/
bool print_message(char *message, size_t message_length);

