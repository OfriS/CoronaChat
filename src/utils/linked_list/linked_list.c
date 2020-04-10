/**
 * @file linked_list.c 
 * @author Ofri Snir
 * @date 10.04.2020
 * @copybeirf linked_list.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "common/common.h"

#include "linked_list.h"
#include "linked_list_internals.h"

enum coronachat_status
LINKED_LIST_init(struct LINKED_LIST_context **to_init)
{
    enum coronachat_status internal_status = CORORNACHAT_STATUS_UNINITIALIZED;
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_context *local_to_init = NULL;

    /* Validate parameters. */
    if (NULL == to_init) {
        status = CORORNACHAT_STATUS_LINKED_LIST_INIT_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. to_init=%p",
                    (int)status,
                    (void *)to_init);
        goto l_cleanup;
    }

    /* Allocate a new list context. */
    local_to_init = calloc(sizeof(*local_to_init),
                           1);
    if (NULL == local_to_init) {
        status = CORORNACHAT_STATUS_LINKED_LIST_INIT_ALLOC_LIST_CONTEXT_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to allocate list context with size %zu with status %d",
                               (sizeof(*local_to_init)),
                               (int)status);
        goto l_cleanup;
    }

    /* Transfer of ownership. */
    *to_init = local_to_init;
    local_to_init = NULL;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    if (NULL != local_to_init) {
        internal_status = LINKED_LIST_free(local_to_init);
        if (true == CORONACHAR_TEST_FAILURE(internal_status)) {
            DEBUG_PRINT("LINKED_LIST_free failed with status %d", internal_status);
            BEST_EFFORT_UPDATE_STATUS(internal_status, status);
        }
    }
    return status;
}


enum coronachat_status
LINKED_LIST_free(struct LINKED_LIST_context *to_free)
{
    enum coronachat_status internal_status = CORORNACHAT_STATUS_UNINITIALIZED;
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    /* Validate parameters. */
    if (NULL == to_free) {
        status = CORORNACHAT_STATUS_LINKED_LIST_FREE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. to_free=%p",
                    (int)status,
                    (void *)to_free);
        goto l_cleanup;
    }

    /* Iterate over the list, freeing each node. */
    while (NULL != to_free->head) {

        /* Remove the list's tail node. */
        internal_status = LINKED_LIST_remove(to_free, to_free->tail);
        if (true == CORONACHAR_TEST_FAILURE(internal_status)) {
            DEBUG_PRINT("LINKED_LIST_remove failed with status %d", internal_status);
            BEST_EFFORT_UPDATE_STATUS(internal_status, status);
        }
    }

    /* Free the list context. */
    (void)free(to_free);

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
LINKED_LIST_append(struct LINKED_LIST_context *list,
                   struct LINKED_LIST_node *reference,
                   void *data,
                   LINKED_LIST_freeing_method_callback_t *freeing_method)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    /* Validate parameters. */
    if ((NULL == list) || (NULL == data) || (NULL == freeing_method)) {
        status = CORORNACHAT_STATUS_LINKED_LIST_APPEND_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p, data=%p, freeing_method=%zu",
                    (int)status,
                    (void *)list,
                    data,
                    (size_t)freeing_method);
        goto l_cleanup;
    }

    /* Add the node to the list. */
    status = linked_list_add_node(list, reference, LINKED_LIST_NODE_POSITION_NEXT, data, freeing_method);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("linked_list_add_node failed with status %d", status);
        goto l_cleanup;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
LINKED_LIST_prepend(struct LINKED_LIST_context *list,
                    struct LINKED_LIST_node *reference,
                    void *data,
                    LINKED_LIST_freeing_method_callback_t *freeing_method)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    /* Validate parameters. */
    if ((NULL == list) || (NULL == data) || (NULL == freeing_method)) {
        status = CORORNACHAT_STATUS_LINKED_LIST_PREPEND_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p, data=%p, freeing_method=%zu",
                    (int)status,
                    (void *)list,
                    data,
                    (size_t)freeing_method);
        goto l_cleanup;
    }

    /* Add the node to the list. */
    status = linked_list_add_node(list, reference, LINKED_LIST_NODE_POSITION_PREV, data, freeing_method);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("linked_list_add_node failed with status %d", status);
        goto l_cleanup;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
LINKED_LIST_get_next(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     struct LINKED_LIST_node **next,
                     void **next_data)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_node *local_next = NULL;
    void *local_next_data = NULL;

    /* Validate parameters. */
    if (NULL == list) {
        status = CORORNACHAT_STATUS_LINKED_LIST_GET_NEXT_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p",
                    (int)status,
                    (void *)list);
        goto l_cleanup;
    }

    status = linked_list_get_node(list, reference, LINKED_LIST_NODE_POSITION_NEXT, &local_next, &local_next_data);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("linked_list_get_node failed with status %d", status);
        goto l_cleanup;
    }

    /* Transfer of ownership. */
    if (NULL != next) {
        *next = local_next;
    }
    if (NULL != next_data) {
        *next_data = local_next_data;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
LINKED_LIST_get_prev(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     struct LINKED_LIST_node **prev,
                     void **prev_data)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_node *local_prev = NULL;
    void *local_prev_data = NULL;

    /* Validate parameters. */
    if (NULL == list) {
        status = CORORNACHAT_STATUS_LINKED_LIST_GET_PREV_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p",
                    (int)status,
                    (void *)list);
        goto l_cleanup;
    }

    status = linked_list_get_node(list, reference, LINKED_LIST_NODE_POSITION_PREV, &local_prev, &local_prev_data);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("linked_list_get_node failed with status %d", status);
        goto l_cleanup;
    }

    /* Transfer of ownership. */
    if (NULL != prev) {
        *prev = local_prev;
    }
    if (NULL != prev_data) {
        *prev_data = local_prev_data;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
LINKED_LIST_get_node_count(struct LINKED_LIST_context *list,
                           size_t *node_count)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    size_t local_node_count = 0;

    /* Validate parameters. */
    if ((NULL == list) || (NULL == node_count)) {
        status = CORORNACHAT_STATUS_LINKED_LIST_GET_PREV_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p, node_count=%p",
                    (int)status,
                    (void *)list,
                    (void *)node_count);
        goto l_cleanup;
    }

    /* Get the list's node count. */
    local_node_count = list->node_count;

    /* Transfer of ownership. */
    *node_count = local_node_count;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
LINKED_LIST_remove(struct LINKED_LIST_context *list,
                   struct LINKED_LIST_node *to_remove)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_node *next = NULL;
    struct LINKED_LIST_node *prev = NULL;

    /* Validate parameters. */
    if ((NULL == list) || (NULL == to_remove)) {
        status = CORORNACHAT_STATUS_LINKED_LIST_REMOVE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p, to_remove=%p",
                    (int)status,
                    (void *)list,
                    (void *)to_remove);
        goto l_cleanup;
    }

    /* Remove the node from the list. */
    next = to_remove->next;
    prev = to_remove->prev;

    if (NULL == next) {
        list->tail = prev;
    }
    else {
        next->prev = prev;
    }

    if (NULL == prev) {
        list->head = next;
    }
    else {
        prev->next = next;
    }

    --list->node_count;

    /* Destroy the node to be removed. */
    status = linked_list_destroy_node(to_remove);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("linked_list_destroy_node failed with status %d", (int)status);
        goto l_cleanup;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
linked_list_add_node(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     enum linked_list_node_position position,
                     void *data,
                     LINKED_LIST_freeing_method_callback_t *freeing_method)
{
    enum coronachat_status internal_status = CORORNACHAT_STATUS_UNINITIALIZED;
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_node *new_node = NULL;
    struct LINKED_LIST_node *next = NULL;
    struct LINKED_LIST_node *prev = NULL;

    /* Validate parameters. */
    if ((NULL == list) || (NULL == data) || (NULL == freeing_method)) {
        status = CORORNACHAT_STATUS_LINKED_LIST_ADD_NODE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. "
                    "list=%p, position=%d, data=%p, freeing_method=%zu",
                    (int)status,
                    (void *)list,
                    (int)position,
                    data,
                    (size_t)freeing_method);
        goto l_cleanup;
    }

    /* Set up relations in the list. */
    switch (position) {
        case (LINKED_LIST_NODE_POSITION_NEXT):
            if (NULL == reference) {
                next = NULL;
                prev = list->tail;
            }
            else {
                next = reference->next;
                prev = reference;
            }
            break;

        case (LINKED_LIST_NODE_POSITION_PREV):
            if (NULL == reference) {
                next = list->head;
                prev = NULL;
            }
            else {
                next = reference;
                prev = reference->prev;
            }
            break;

        default:
            status = CORORNACHAT_STATUS_LINKED_LIST_ADD_NODE_INVALID_NODE_POSITION;
            DEBUG_PRINT("Failed due to invalid node position with status %d. position=%d",
                        (int)status,
                        (int)position);
            goto l_cleanup;
    }

    /* Create the new node. */
    status = linked_list_create_node(data, freeing_method, &new_node);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_remove failed with status %d", status);
        goto l_cleanup;
    }

    /* Set up relations in the new node. */
    new_node->next = next;
    new_node->prev = prev;

    /* Transfer of ownership. */
    if (NULL == next) {
        list->tail = new_node;
    }
    else {
        next->prev = new_node;
    }

    if (NULL == prev) {
        list->head = new_node;
    }
    else {
        prev->next = new_node;
    }
    new_node = NULL;

    ++list->node_count;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    if (NULL != new_node) {
        internal_status = linked_list_destroy_node(new_node);
        if (true == CORONACHAR_TEST_FAILURE(internal_status)) {
            DEBUG_PRINT("linked_list_destroy_node failed with status %d", internal_status);
            BEST_EFFORT_UPDATE_STATUS(internal_status, status);
        }
    }
    return status;
}


enum coronachat_status
linked_list_create_node(void *data,
                        LINKED_LIST_freeing_method_callback_t *freeing_method,
                        struct LINKED_LIST_node **new_node)
{
    enum coronachat_status internal_status = CORORNACHAT_STATUS_UNINITIALIZED;
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_node *local_new_node = NULL;

    /* Validate parameters. */
    if ((NULL == data) || (NULL == freeing_method) || (NULL == new_node)) {
        status = CORORNACHAT_STATUS_LINKED_LIST_CREATE_NODE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. data=%p, freeing_method=%zu, new_node=%p",
                    (int)status,
                    data,
                    (size_t)freeing_method,
                    (void *)new_node);
        goto l_cleanup;
    }

    /* Allocate the new node. */
    local_new_node = calloc(sizeof(*local_new_node), 1);
    if (NULL == local_new_node) {
        status = CORORNACHAT_STATUS_LINKED_LIST_CREATE_NODE_ALLOC_NODE_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to allocate node with size %zu with status %d",
                               (sizeof(*local_new_node)),
                               (int)status);
        goto l_cleanup;
    }

    /* Set the data and freeing method in the new node. */
    local_new_node->data = data;
    local_new_node->freeing_method = freeing_method;

    /* Transfer of ownership. */
    *new_node = local_new_node;
    local_new_node = NULL;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    if (NULL != local_new_node ) {
        internal_status = linked_list_destroy_node(local_new_node);
        if (true == CORONACHAR_TEST_FAILURE(internal_status)) {
            DEBUG_PRINT("linked_list_destroy_node failed with status %d", (int)internal_status);
            BEST_EFFORT_UPDATE_STATUS(internal_status, status);
        }
    }
    return status;
}


enum coronachat_status
linked_list_destroy_node(struct LINKED_LIST_node *to_destroy)
{
    enum coronachat_status internal_status = CORORNACHAT_STATUS_UNINITIALIZED;
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    /* Validate parameters. */
    if (NULL == to_destroy) {
        status = CORORNACHAT_STATUS_LINKED_LIST_DESTROY_NODE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. to_destroy=%p",
                    (int)status,
                    (void *)to_destroy);
        goto l_cleanup;
    }

    /* Validate the node has a freeing method. */
    if (NULL == to_destroy->freeing_method) {
        internal_status = CORORNACHAT_STATUS_LINKED_LIST_DESTROY_NODE_INVALID_FREEING_METHOD;
        DEBUG_PRINT("Current node contains no freeing method callback. status %d",
                    (int)internal_status);
        BEST_EFFORT_UPDATE_STATUS(internal_status, status);
    }

    /* Free the node's data. */
    else {
        internal_status = to_destroy->freeing_method(to_destroy->data);
        if (true == CORONACHAR_TEST_FAILURE(internal_status)) {
            DEBUG_PRINT("freeing_method failed with status %d", (int)internal_status);
            BEST_EFFORT_UPDATE_STATUS(internal_status, status);
        }
    }

    /* Free the node. */
    (void)free(to_destroy);

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
linked_list_get_node(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     enum linked_list_node_position position,
                     struct LINKED_LIST_node **node,
                     void **node_data)
{
    enum coronachat_status internal_status = CORORNACHAT_STATUS_UNINITIALIZED;
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct LINKED_LIST_node *local_node = NULL;
    void *local_node_data = NULL;

    /* Validate parameters. */
    if (NULL == list) {
        status = CORORNACHAT_STATUS_LINKED_LIST_GET_NODE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. list=%p, node=%p, node_data=%p",
                    (int)status,
                    (void *)list,
                    (void *)node,
                    (void *)node_data);
        goto l_cleanup;
    }

    /* Set up relations in the list. */
    switch (position) {
        case (LINKED_LIST_NODE_POSITION_NEXT):
            if (NULL == reference) {
                local_node = list->head;
            }
            else {
                local_node = reference->next;
            }
            break;

        case (LINKED_LIST_NODE_POSITION_PREV):
            if (NULL == reference) {
                local_node = list->tail;
            }
            else {
                local_node = reference->prev;
            }
            break;

        default:
            status = CORORNACHAT_STATUS_LINKED_LIST_GET_NODE_INVALID_NODE_POSITION;
            DEBUG_PRINT("Failed due to invalid node position with status %d. position=%d",
                        (int)status,
                        (int)position);
            goto l_cleanup;
    }

    local_node_data = local_node->data;

    /* Transfer of ownership. */
    if (NULL != node) {
        *node = local_node;
    }
    if (NULL != node_data) {
        *node_data = local_node_data;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}