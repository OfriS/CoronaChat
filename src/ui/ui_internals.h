
/* The internal header of the ui module. */
#include <stdio.h>
#include <ncurses.h>

#include "./ui.h" //TODO - include <...>

/*** Constants ***/

/* The input part of the screen.
 *  Looks something like this: 
 *  -------------------------
 *  Hi                      |
 *  -------------------------
 */
#define INPUT_LINES_NUMBER (3)
#define INPUT_LINE_FRAME_GRAPHIC ("-------------------------")
#define INPUT_LINE_END_GRAPHIC ("|")
#define INPUT_LINE_END_X (24)

/*** Functions ***/  

/** @brief - The function moves all the lines one line up.
 *
 * @param IN OUT y - The number of written lines
 * @param IN max_y - The max number of lines
 *
 * @return - if the function succeed.
 */
bool create_empty_line(int *y, int max_y);

/** @brief - The function prints one-line message.
 *
 * @param IN OUT y - The line number
 * @param IN max_y - The max number of lines
 * @param IN message - The line to print
 *
 * @return - if the function succeed.
 */
bool print_message(int *y, int max_y, char *message);

/** @brief - The function prints the input line to the screen.
 *
 * @param IN line_length - The message box x value.
 *
 * @return - if the function succeed. 
 */
bool print_input_line(int line_length);

