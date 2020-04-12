/**
 * @file network_utils_internals.h
 * @author Ofri Snir
 * @date 11.04.2020
 * @copybeirf network_utils.h
 */

#ifndef __CORONACHAT_NETWORK_UTILS_INTERNALS_H__
#define __CORONACHAT_NETWORK_UTILS_INTERNALS_H__

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "common/common.h"
#include "coronachat_status.h"

/** Maximum number of connections to be held on the listening socket's backlog. */
#define NETWORK_UTILS_BACKLOG (10)

/** @brief Pairing of a socket and a flag indicating whether it is bound to an address. */
struct network_utils_socket {

    /** Descriptor representing the socket. */
    int descriptor;

    /** Flag indicating whether the socket is bound. */
    bool is_set_up;
};


/**
 * @brief Function type for iterating over a list resulting from the call to getaddrinfo().
 *
 * @param[in] current_address - req - Current address in the address list.
 * @param[in, out] parameter - opt - General use parameter for the function.
 * @param[out] should_break - req - Flag indicating that the iteration should be stopped.
 *
 * @return coronachat_status
 */
typedef enum coronachat_status (network_utils_address_list_iterator_callback_t)(struct addrinfo *current_address,
                                                                                void *parameter,
                                                                                bool *should_break);


/**
 * @brief Iterate over an address list returned by getaddrinfo and perform an action in each iteration.
 *
 * @param[in] address_list - req - Address list to be iterated upon.
 * @param[in] iteration_action - req - Callback to be called during each iteration.
 * @param[in, out] iteration_action_parameter - req - General purpose parameter used by the action callback.
 *
 * @return coronachat_status
 */
static enum coronachat_status
network_utils_iterate_address_list(struct addrinfo *address_list,
                                   network_utils_address_list_iterator_callback_t *iteration_action,
                                   void *iteration_action_parameter);


/**
 * @brief Opens a socket and bind it with a provided address.
 *
 *  This function is of the type network_utils_address_list_iterator_callback_t and is intended to be used by
 *  network_utils_iterate_address_list() in order to iterate over an address list.
 *
 * @param[in] address - req - Address to bind to the socket.
 * @param[out] to_bind - req - Socket to be opened and bound to the provided address.
 * @param[out] is_bound - req - Flag indicating the socket was successfully bound. Used to communicate to the iterating
 *  function to break the iteration.
 *
 * @return coronachat_status
 */
static enum coronachat_status
network_utils_bind_to_address(struct addrinfo *address,
                              struct network_utils_socket *to_bind,
                              bool *is_bound);


/**
 * @brief Opens a socket and connects it to the provided address.
 *
 *  This function is of the type network_utils_address_list_iterator_callback_t and is intended to be used by
 *  network_utils_iterate_address_list() in order to iterate over an address list.
 *
 * @param[in] address - req - Address to bind to the socket.
 * @param[out] to_connect - req - Socket to be opened and connected to the provided address.
 * @param[out] is_connected - req - Flag indicating the socket was successfully connected. Used to communicate to the
 *  iterating function to break the iteration.
 *
 * @return coronachat_status
 */
static enum coronachat_status
network_utils_connect_to_address(struct addrinfo *address,
                                 struct network_utils_socket *to_connect,
                                 bool *is_connected);


#endif /* !__CORONACHAT_NETWORK_UTILS_INTERNALS_H__ */