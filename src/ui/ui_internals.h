
/* The internal header of the ui module. */

#ifndef __CORONACHAT_UI_INTERNALS_H__
#define __CORONACHAT_UI_INTERNALS_H__

#include <stdio.h>
#include <ncurses.h>

#include "./ui.h" //TODO - include <...>
#include "../coronachat_status.h"
#include "../utils/linked_list/linked_list.h"

/*** Constants ***/

/* The max name length. */
#define MAX_NAME_LENGTH (8)

/* The chars on the end of a message. */
#define SEND_MESSAGE_CHAR 	('\r')
#define END_MESSAGE_CHAR 	('\0')

/* The input part of the screen.
 *  Looks something like this: 
 *  -------------------------
 *  Hi                      |
 *  -------------------------
 */
#define INPUT_LINES_NUMBER (3)
#define INPUT_LINE_FRAME_GRAPHIC ("----------------------------------------------")
#define INPUT_LINE_END_GRAPHIC ("|")
#define INPUT_LINE_END_X (sizeof(INPUT_LINE_FRAME_GRAPHIC) - sizeof(INPUT_LINE_END_GRAPHIC))

/* Colors */
#define BOX_COLOR (1)

/*** Globals ***/

size_t g_max_message_length = 0;
int g_current_line = 0;

/*** Functions ***/  

/** @brief - The function add a new message to the messages list.
 * 			 If there is not enough lines - it removes the oldest message.
 *
 * @param IN max_lines - The max number of messages.
 * @param IN new_message - The new message (followd by Null-terminator).
 * @param IN OUT chat_messages - All of the chat messages.
 *
 * @return - if the function succeed.
 */
enum coronachat_status
add_message(int max_lines, char *new_message, struct LINKED_LIST_context *chat_messages);

/** @brief - The function prints one-line message.
 *
 * @param IN OUT y - The line number
 * @param IN max_y - The max number of lines
 * @param IN message - The line to print
 *
 * @return - if the function succeed.
 */
enum coronachat_status
print_all_lines(int lines_number, struct LINKED_LIST_context *messages);

/** @brief - The function prints the input line box on the screen.
 *
 * @param IN line_length - The message box x value.
 *
 * @return - if the function succeed. 
 */
enum coronachat_status
print_input_box(int line_length);

/** @brief - The function frees buffer.
*
* @param IN OUT buffer 
*
* @return - if the function succeed.
*/
enum coronachat_status
heap_free(void *buffer);

#endif /* __CORONACHAT_UI_INTERNALS_H__ */
