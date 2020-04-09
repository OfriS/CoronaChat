
/* The internal header of the ui module. */
#include <stdio.h>
#include <ncurses.h>

#include "./ui.h" //TODO - include <...>

/*** Constants ***/



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

