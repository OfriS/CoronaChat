
/* The internal header of the ui module. */
#include <stdio.h>
#include <ncurses.h>

#include "./ui.h" //TODO - include <...>

/*** Constants ***/

/* The max name length. */
#define MAX_NAME_LENGTH (8)

/* The input part of the screen.
 *  Looks something like this: 
 *  -------------------------
 *  Hi                      |
 *  -------------------------
 */
#define INPUT_LINES_NUMBER (3)
#define INPUT_LINE_FRAME_GRAPHIC ("-------------------------")
#define INPUT_LINE_END_GRAPHIC ("|")
#define INPUT_LINE_END_X (sizeof(INPUT_LINE_FRAME_GRAPHIC) - sizeof(INPUT_LINE_END_GRAPHIC))

/*** Globals ***/

size_t g_max_message_length = 0;
int g_current_line = 0;

/*** Functions ***/  

/** @brief - The function moves all the lines one line up.
 *
 * @param IN y - The number of the line that will be empty.
 *
 * @return - if the function succeed.
 */
bool create_empty_line(int y);

/** @brief - The function prints one-line message.
 *
 * @param IN OUT y - The line number
 * @param IN max_y - The max number of lines
 * @param IN message - The line to print
 *
 * @return - if the function succeed.
 */
bool print_message(int *y, int max_y, char *message);

/** @brief - The function prints the input line box on the screen.
 *
 * @param IN line_length - The message box x value.
 *
 * @return - if the function succeed. 
 */
bool print_input_line(int line_length);

