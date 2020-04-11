
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
#include "../utils/linked_list/linked_list.h"

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

    ++*char_counter;

    /* Check if the message is done - '\n' is the last char or there is not enough place in the input box. */
    if ((SEND_MESSAGE_CHAR == *input_buffer) ||
        (INPUT_LINE_END_X <= *char_counter)) {
        *input_buffer = END_MESSAGE_CHAR;
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

enum coronachat_status
UI__print_message(char *message, size_t message_length, int max_y, struct LINKED_LIST_context *messages)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    /* Add the new message to the messages linked list. */
    status = add_message(max_y - INPUT_LINES_NUMBER, message, messages);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        goto l_cleanup;
    }

    /* Print all of the messages. */
    status = print_all_lines(max_y, messages);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        goto l_cleanup;
    }

    /* Print the input box.  */
    status = print_input_box(max_y);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        goto l_cleanup;
    }

    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}

enum coronachat_status
UI__init_screen(int *max_x, int *max_y, struct LINKED_LIST_context **messages)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
    int function_result = C_FAILURE;
    WINDOW *initscr_result = NULL;

    /* Init the chat window. */
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

    /* Init the messages list. */
    status = LINKED_LIST_init(messages);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO("Init the linked list failed.\n")
        goto l_cleanup;
    }

    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}

enum coronachat_status
UI__destroy_screen(struct LINKED_LIST_context *messages)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
    int function_result = C_FAILURE;

    /* Free the messages list. */
    status = LINKED_LIST_free(messages);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO("freeing the messages list failed\n");
        goto l_cleanup;
    }

    /* Clear the window (dont neccesary). */
    function_result = clear();
    if (C_SUCCESS != function_result) {
        status = CORORNACHAT_STATUS_UI_DESTROY_SCREEN_CLEAR_FAILED;
        DEBUG_PRINT_WITH_ERRNO("clear screen failed.\n");
    }

    /* Close the chat window. */
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

enum coronachat_status
print_all_lines(int lines_number, struct LINKED_LIST_context *messages)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
    int current_line = C_FAILURE;
    struct LINKED_LIST_node *current_message = NULL;
    char *current_data = NULL;
    size_t messages_counter = C_FAILURE;
    int function_result = C_FAILURE;

    /* Validates the arguments. */
    if ((NULL == messages) ||
        (lines_number <= 0)) {
        status = CORORNACHAT_STATUS_UI_PRINT_ALL_LINES_INVALID_PARAMETERS;
        DEBUG_PRINT_WITH_ERRNO("print_all_lines got invalid parameter.\n");
    }

    /* The number of messages. */
    status = LINKED_LIST_get_node_count(messages, &messages_counter);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        goto l_cleanup;
    }

    /* Clear the whole screen. */
    function_result = clear();
    if (C_SUCCESS != function_result) {
        status = CORORNACHAT_STATUS_UI_PRINT_ALL_LINES_CLEAR_FAILED;
        DEBUG_PRINT_WITH_ERRNO("clear screen failed.\n");
    }

    /* Print all of the messages. */
    for (current_line = 0; current_line < messages_counter; current_line++) {
        /* Get the current message node. */
        status = LINKED_LIST_get_next(messages, current_message, &current_message, (void **)&current_data);
        if (CORORNACHAT_STATUS_SUCCESS != status) {
            goto l_cleanup;
        }

        /* Print the message in the current line. */
        function_result = mvprintw(current_line, 0, current_data);
        if (C_SUCCESS != function_result) {
            status = CORORNACHAT_STATUS_UI_PRINT_ALL_LINES_PRINT_FAILED;
            DEBUG_PRINT_WITH_ERRNO("mvprintw returned %d\n",
                                   function_result);
            goto l_cleanup;
        }
    }

    function_result = refresh();
    if (C_SUCCESS != function_result) {
        status = CORORNACHAT_STATUS_UI_PRINT_ALL_LINES_REFRESH_FAILED;
        DEBUG_PRINT_WITH_ERRNO("refresh returned %d\n",
                               function_result);
        goto l_cleanup;
    }

    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}

enum coronachat_status
add_message(int max_lines, char *new_message, struct LINKED_LIST_context *chat_messages)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
    size_t messages_counter = C_FAILURE;
    struct LINKED_LIST_node *first_message = NULL;
    char *message_data = NULL;

    /* Copy the message (and allocate place for it). */
    message_data = strdup(new_message);

    /* Add the new message to the nodes. */
    status = LINKED_LIST_append(chat_messages, NULL, (void *)message_data, heap_free);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        goto l_cleanup;
    }

    /* Check if there are too many nodes. */
    status = LINKED_LIST_get_node_count(chat_messages, &messages_counter);
    if (CORORNACHAT_STATUS_SUCCESS != status) {
        goto l_cleanup;
    }

    /* Check if there are too many saved messages. */
    if (messages_counter > max_lines) {
        /* Remove the first message (oldest one). */
        status = LINKED_LIST_get_next(chat_messages, first_message, &first_message, NULL);
        if (CORORNACHAT_STATUS_SUCCESS != status) {
            goto l_cleanup;
        }

        status = LINKED_LIST_remove(chat_messages, first_message);
        if (CORORNACHAT_STATUS_SUCCESS != status) {
            goto l_cleanup;
        }
    }

    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
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

enum coronachat_status
heap_free(void *buffer)
{
    (void)free(buffer);

    return CORORNACHAT_STATUS_SUCCESS;
}