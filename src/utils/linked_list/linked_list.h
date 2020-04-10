/**
 * @file linked_list.h 
 * @author Ofri Snir
 * @date 10.04.2020
 * @beirf Doubly linked list implementation.
 *
 *  This module exports a doubly linked list object and function which allow operating on it.
 *
 *  Usage:
 *   In order to initialize a new list object, LINKED_LIST_init() must be called.
 *   Now the caller may call any of the LINKED_LIST_append, _prepend, _get_next _get_prev... functions in order to
 *   perform operation on the list.
 *   Once the caller is done using the list, LINKED_LIST_free() must be called in order to free the list object and all
 *   data held by it.
 */

#ifndef __CORONACHAT_LINKED_LIST_H__
#define __CORONACHAT_LINKED_LIST_H__

#include "coronachat_status.h"

/**
 * @brief Function type for freeing node data.
 *
 * @param[in, out] to_free - req - Data to be freed.
 *
 * @return coronachat_status
 */
typedef enum coronachat_status (LINKED_LIST_freeing_method_callback_t)(void *to_free);

/** @brief  Doubly linked list. */
struct LINKED_LIST_context;

/** @brief Linked list node containing a pointer to some data. */
struct LINKED_LIST_node;


/**
 * @brief Initialize a new doubly linked list.
 *
 * @param[in] to_init - req - List to be initialized.
 *
 * @return coronachat_status
 *
 * @note Must later be freed by calling LINKED_LIST_free().
 */
enum coronachat_status
LINKED_LIST_init(struct LINKED_LIST_context **to_init);


/**
 * @brief Free the doubly linked list and all the nodes held by it.
 *
 * @param[in] to_init - req - List to be freed.
 *
 * @return coronachat_status
 *
 * @note After this function is called to_free becomes invalid.
 */
enum coronachat_status
LINKED_LIST_free(struct LINKED_LIST_context *to_free);


/**
 * @brief Appends a new node to reference node.
 *
 * @param[in] list - req - List to be appended.
 * @param[in] reference - opt - Node to reference when positioning the new node. If not provided the node is appended
 *  to the list's tail.
 * @param[in] data - req - Data to be held by the node.
 * @param[in] freeing_method - req - Callback function for freeing the data.
 *
 * @return coronachat_status
 */
enum coronachat_status
LINKED_LIST_append(struct LINKED_LIST_context *list,
                   struct LINKED_LIST_node *reference,
                   void *data,
                   LINKED_LIST_freeing_method_callback_t *freeing_method);


/**
 * @brief Prepends a new node to the reference node.
 *
 * @param[in] list - req - List to be prepended.
 * @param[in] reference - opt - Node to reference when positioning the new node. If not provided the node is prepended
 *  to the list's head.
 * @param[in] data - req - Data to be held by the node.
 * @param[in] freeing_method - req - Callback function for freeing the data.
 *
 * @return coronachat_status
 */
enum coronachat_status
LINKED_LIST_prepend(struct LINKED_LIST_context *list,
                    struct LINKED_LIST_node *reference,
                    void *data,
                    LINKED_LIST_freeing_method_callback_t *freeing_method);


/**
 * @brief Returns the next node relative to the reference node and the data held by it.
 *
 * @param[in] list - req - List containing the node to be retrieved.
 * @param[in] reference - req - Node to reference when searching for the desired node. If not provided the list's head
 *  node is returned.
 * @param[out] next - opt - The retrieved node.
 * @param[out] next_data - opt - The data held by the retrieved node.
 *
 * @return coronachat_status
 */
enum coronachat_status
LINKED_LIST_get_next(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     struct LINKED_LIST_node **next,
                     void **next_data);


/**
 * @brief Returns the previous node relative to the reference node and the data held by it.
 *
 * @param[in] list - req - List containing the node to be retrieved.
 * @param[in] reference - req - Node to reference when searching for the desired node. If not provided the list's tail
 *  node is returned.
 * @param[out] next - opt - The retrieved node.
 * @param[out] next_data - opt - The data held by the retrieved node.
 *
 * @return coronachat_status
 */
enum coronachat_status
LINKED_LIST_get_prev(struct LINKED_LIST_context *list,
                     struct LINKED_LIST_node *reference,
                     struct LINKED_LIST_node **prev,
                     void **prev_data);


/**
 * @brief Returns the list's current node count.
 *
 * @param[in] list - req - List to be assessed.
 * @param[out] node_count - req - Node count of the provided list.
 *
 * @return coronachat_status
 */
enum coronachat_status
LINKED_LIST_get_node_count(struct LINKED_LIST_context *list,
                           size_t *node_count);


/**
 * @brief Remove the provided node from the list and frees it's data.
 *
 * @param[in] list - req - List containing the node to be removed.
 * @param[in, out] to_remove - req - Node to be removed.
 *
 * @return coronachat_status
 *
 * @note Once this function is called, the data held by the node is freed and becomes invalid.
 */
enum coronachat_status
LINKED_LIST_remove(struct LINKED_LIST_context *list,
                   struct LINKED_LIST_node *to_remove);

#endif /* !__CORONACHAT_LINKED_LIST_H__ */