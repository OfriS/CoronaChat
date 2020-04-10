
/* The header of the ui module. */
/* This module implements the input and output with the client. */

#include <stdio.h>

#include "../common/common.h"

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
bool UI__get_message(char **input_buffer, size_t *input_size);

/** @brief - The function prints messages on the chat screen.
 *
 * @param IN message
 * @param IN message_length
 * //TODO
 *
 * @return - if the function succeed.
*/
bool UI__print_message(char *message, size_t message_length, int max_y, int *y);

/** @brief - The function initiates the output screen.
 *
 * @param OUT max_x - The window x length
 * @param OUT max_y - The window y length
 *
 * @return - if the function succeed.
*/
bool UI__init_screen(int *max_x, int *max_y);

/** @brief - The function close the new window.
 *
 * @return - if the function succeed.
 */
bool UI__destroy_screen(void);

