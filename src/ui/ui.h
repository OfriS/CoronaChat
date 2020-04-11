
/* The header of the ui module. */
/* This module implements the input and output with the client. */

#ifndef __CORONACHAT_UI_H__
#define __CORONACHAT_UI_H__

#include <stdio.h>

#include "../common/common.h"
#include "../coronachat_status.h"
#include "../utils/linked_list/linked_list.h"

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
 * @param IN max_y - The maximum lines number.
 * @param IN OUT messages - The messages list.
 *
 * @return - if the function succeed.
*/
enum coronachat_status
UI__print_message(char *message, size_t message_length, int max_y, struct LINKED_LIST_context *messages);

/** @brief - The function initiates the output screen.
 *
 * @param OUT max_x - The window x length
 * @param OUT max_y - The window y length
 * @param OUT messages - The chat messages.
 *
 * @return - if the function succeed.
*/
enum coronachat_status
UI__init_screen(int *max_x, int *max_y, struct LINKED_LIST_context **messages);

/** @brief - The function close the new window.
 *
 * @param IN messages - The messages list to free.
 *
 * @return - if the function succeed.
 */
enum coronachat_status
UI__destroy_screen(struct LINKED_LIST_context *messages);

#endif /* __CORONACHAT_UI_H__ */
