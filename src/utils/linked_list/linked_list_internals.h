/**
 * @file linked_list_internals.h 
 * @author Ofri Snir
 * @date 10.04.2020
 * @copybeirf linked_list.h
 */

#ifndef __CORONACHAT_LINKED_LIST_INTERNALS_H__
#define __CORONACHAT_LINKED_LIST_INTERNALS_H__

#include <unistd.h>

#include "coronachat_status.h"

#include "linked_list.h"

/** @copybrief LINKED_LIST_node */
struct LINKED_LIST_node {
    /** Pointer to previous node in the list. */
    struct LINKED_LIST_node *prev;

    /** Pointer to next node in the list. */
    struct LINKED_LIST_node *next;

    /** Data held by the node. */
    void *data;

    /** Freeing method for the data held by the node. */
    LINKED_LIST_freeing_method_callback_t *freeing_method;
};

/** @copybrief LINKED_LIST_context */
struct LINKED_LIST_context {
    /** Pointer to the first node in the list. */
    struct LINKED_LIST_node *head;

    /** Pointer to the last node in the list. */
    struct LINKED_LIST_node *tail;

    /** Number of nodes currently in the list. */
    size_t node_count;
};

/** @brief All valid node positions for a node in the doubly linked list. */
enum linked_list_node_position {
    LINKED_LIST_NODE_POSITION_INVALID = -1,

    LINKED_LIST_NODE_POSITION_PREV,
    LINKED_LIST_NODE_POSITION_NEXT,

    LINKED_LIST_NODE_POSITION_COUNT,
};


/**
 * @brief Add a node to the list. The node's locations is determined by the reference node and the position argument.
 *
 * @param[in] list - req - List to which to add the node.
 * @param[in] reference - opt - Reference node to be used when deciding the node's location in the list. If not
 *  provided the node's location is determined using only the position argument.
 * @param[in] position - req - Value indicating how to position the node relative the the reference node.
 * @param[in] data - req - Data to be stored by the node.
 * @param[in] freeing_method - req - Freeing method with which to free the provided data.
 *
 * @return coronachat_status
 */
enum coronachat_status
linked_list_add_node(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     enum linked_list_node_position position,
                     void *data,
                     LINKED_LIST_freeing_method_callback_t *freeing_method);


/**
 * @brief Get a node from the list. The node's locations is determined by the reference node and the position argument.
 *
 * @param[in] list - req - List from which to get the node.
 * @param[in] reference - opt - Reference node to be used when locating the node. If not provided the node's location
 *  is determined using only the position argument.
 * @param[in] position - req - Value indicating the location of the node to be gotten relative to the reference node.
 * @param[out] node - req - The node which has been retrieved.
 * @param[out] node_data - req - The data held by the retrieved node.
 * @return
 */
enum coronachat_status
linked_list_get_node(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     enum linked_list_node_position position,
                     struct LINKED_LIST_node **node,
                     void **node_data);


/**
 * @brief Create a new node and set the provided data in it.
 *
 * @param[in] data - req - Data to be placed within the new node.
 * @param[in] freeing_method - req - Callback function for freeing the provided data.
 * @param[out] new_node - req - Newly created node.
 *
 * @return coronachat_status
 */
enum coronachat_status
linked_list_create_node(void *data,
                        LINKED_LIST_freeing_method_callback_t *freeing_method,
                        struct LINKED_LIST_node **new_node);


/**
 * @brief Destroy the provided node and free all resources held by it.
 *
 * @param[in, out] to_destroy - req - Node to be destroyed.
 *
 * @return coronachat_status
 */
enum coronachat_status
linked_list_destroy_node(struct LINKED_LIST_node *to_destroy);

#endif /* !__CORONACHAT_LINKED_LIST_INTERNALS_H__ */