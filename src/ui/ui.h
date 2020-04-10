
/* The header of the ui module. */
/* This module implements the input and output with the client. */

#ifndef __CORONACHAT_UI_H__
#define __CORONACHAT_UI_H__

#include <stdio.h>

#include "../common/common.h"
#include "../coronachat_status.h"

/*** Constants ***/



/*** Functions ***/

/** @brief - The function gets a char from the client, and prints it to him.
 *
 * @param OUT input_buffer
 * @param IN max_y - Number of lines in the current window size.
 * @param OUT is_done - If the user done writing the current message.
 * @param IN OUT char_counter - The number of chars that already read in the current message.
 *
 * @return - if the function succeed.
 *
 * @note - please call the function when there is input from STDIN (after using select()).
 */
enum coronachat_status
UI__get_message(char *input_buffer, int max_y, bool *is_done, size_t *char_counter);

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
enum coronachat_status
UI__init_screen(int *max_x, int *max_y);

/** @brief - The function close the new window.
 *
 * @return - if the function succeed.
 */
enum coronachat_status
UI__destroy_screen(void);

#endif /* __CORONACHAT_UI_H__ */
