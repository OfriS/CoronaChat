/**
 * @file network_utils.c
 * @author Ofri Snir
 * @date 11.04.2020
 * @copybeirf network_utils.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "common/common.h"
#include "coronachat_status.h"

#include "network_utils_internals.h"

enum coronachat_status
NETWORK_UTILS_setup_listener(const char *host,
                             const char *port,
                             int *listener)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct network_utils_socket local_listener = {0};
    struct addrinfo *server_info = NULL;
    struct addrinfo hints = {0};
    int getaddrinfo_result = C_FAILURE;
    int listen_result = C_FAILURE;

    /* Validate parameters. */
    if ((NULL == port) || (NULL == listener)) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_SETUP_LISTENER_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. port=%p, listener=%p",
                    (int)status,
                    (void *)port,
                    (void *)listener);
        goto l_cleanup;
    }

    /* Set up hints for finding server's address information. */
    (void)memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* If no host IP was provided, use the localhost IP. */
    if (NULL == host) {
        hints.ai_flags = AI_PASSIVE;
    }

    /* Get the server's address information. */
    getaddrinfo_result = getaddrinfo(host, port, &hints, &server_info);
    if (0 != getaddrinfo_result) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_SETUP_LISTENER_GETADDRINFO_FAILURE;
        DEBUG_PRINT("Failed to get server info with status %d. %s", (int)status, gai_strerror(getaddrinfo_result));
        goto l_cleanup;
    }

    status = network_utils_iterate_address_list(server_info,
                                                (network_utils_address_list_iterator_callback_t *)network_utils_bind_to_address,
                                                (void *)&local_listener);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("network_utils_iterate_address_list failed with status %d", (int)status);
        goto l_cleanup;
    }

    if (false == local_listener.is_set_up) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_SETUP_LISTENER_BIND_LISTENER_FAILURE;
        DEBUG_PRINT("Failed to find address to bind listener on with status %d", (int)status);
        goto l_cleanup;
    }

    /* Mark the socket as a listener socket. */
    listen_result = listen(local_listener.descriptor, NETWORK_UTILS_BACKLOG);
    if (C_FAILURE == listen_result) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_SETUP_LISTENER_LISTEN_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to mark socket %d as listener with status %d",
                               local_listener.descriptor,
                               (int)status);
        goto l_cleanup;
    }

    /* Transfer of ownership. */
    *listener = local_listener.descriptor;
    local_listener.descriptor = INVALID_FILE_DESCRIPTOR;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    CLOSE(local_listener.descriptor);
    (void)freeaddrinfo(server_info);

    return status;
}


enum coronachat_status
NETWORK_UTILS_accept_connection(int listener,
                                int *client_socket,
                                struct sockaddr_storage **client_address)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct sockaddr_storage *local_client_address = NULL;
    socklen_t local_client_address_size = 0;
    int local_client_socket = INVALID_FILE_DESCRIPTOR;

    /* Validate parameters. */
    if ((INVALID_FILE_DESCRIPTOR == listener) || (NULL == client_socket) || (NULL == client_address)) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_ACCEPT_CONNECTION_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. "
                    "listener=%d, client_socket=%p, client_address=%p",
                    (int)status,
                    listener,
                    (void *)client_socket,
                    (void *)client_address);
        goto l_cleanup;
    }

    /* Allocate buffer to hold the connecting client's address. */
    local_client_address = calloc(1, sizeof(*local_client_address));
    if (NULL == local_client_address) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_ACCEPT_CONNECTION_ALLOC_CLIENT_ADDRESS_BUFFER_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to allocate hold client address buffer with size %zu with status %d",
                               (sizeof(*local_client_address)),
                               (int)status);
        goto l_cleanup;
    }

    /* Accept the connecting client. */
    local_client_socket = accept(listener, (struct sockaddr *)&local_client_address, &local_client_address_size);
    if (INVALID_FILE_DESCRIPTOR == local_client_socket) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_ACCEPT_CONNECTION_ACCEPT_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to accept connection from socket %d with status %d", listener, (int)status);
        goto l_cleanup;
    }

    /* Transfer of ownership. */
    *client_socket = local_client_socket;
    local_client_socket = INVALID_FILE_DESCRIPTOR;
    *client_address = local_client_address;
    local_client_address = NULL;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    CLOSE(local_client_socket);
    HEAPFREE(local_client_address);
    return status;
}


enum coronachat_status
NETWORK_UTILS_connect(const char *server_ip,
                      const char *server_port,
                      int *server_socket)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct network_utils_socket local_server_socket = {0};
    int getaddrinfo_result = C_FAILURE;
    struct addrinfo *server_address_list = NULL;
    struct addrinfo hints = {0};

    /* Validate parameters. */
    if ((NULL == server_ip) || (NULL == server_port) || (NULL == server_socket)) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_CONNECT_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. server_ip=%p, server_port=%p, server_socket=%p",
                    (int)status,
                    (void *)server_ip,
                    (void *)server_port,
                    (void *)server_socket);
        goto l_cleanup;
    }

    /* Set up hints for finding server's address information. */
    (void)memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* Get the server's address. */
    getaddrinfo_result = getaddrinfo(server_ip, server_port, &hints, &server_address_list);
    if (0 != getaddrinfo_result) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_CONNECT_GETADDRINFO_FAILURE;
        DEBUG_PRINT("Failed to get server info with status %d. %s", (int)status, gai_strerror(getaddrinfo_result));
        goto l_cleanup;
    }

    /* Open a socket and connect it to the server. */
    status = network_utils_iterate_address_list(server_address_list,
                                                (network_utils_address_list_iterator_callback_t *)network_utils_connect_to_address,
                                                (void *)&local_server_socket);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("network_utils_iterate_address_list failed with status %d", (int)status);
        goto l_cleanup;
    }

    /* Validate that a connection has been established. */
    if (false == local_server_socket.is_set_up) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_CONNECT_CONNECT_FAILURE;
        DEBUG_PRINT("Failed to connect to server with status %d", (int)status);
        goto l_cleanup;
    }

    /* Transfer of ownership. */
    *server_socket = local_server_socket.descriptor;
    local_server_socket.descriptor = INVALID_FILE_DESCRIPTOR;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    CLOSE(local_server_socket.descriptor);
    (void)freeaddrinfo(server_address_list);

    return status;
}


enum coronachat_status
NETWORK_UTILS_send_message(int receiver,
                           const uint8_t *message,
                           size_t message_size)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    int send_result = C_FAILURE;

    /* Validate parameters. */
    if ((INVALID_FILE_DESCRIPTOR == receiver) || (NULL == message)) {
        status = CORONACHAT_STATUS_NETWORK_UTILS_SEND_MESSAGE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. receiver=%d, message=%p",
                    (int)status,
                    receiver,
                    (void *)message);
        goto l_cleanup;
    }

    /* Send the message over the socket. */
    send_result = send(receiver, (void *)message, message_size, 0);
    if (C_FAILURE == send_result) {
        status = CORONACHAT_STATUS_NETWORK_UTILS_SEND_MESSAGE_SEND_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to send message of size %zu over socket %d with status %d",
                               message_size,
                               receiver,
                               (int)status);
        goto l_cleanup;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


enum coronachat_status
NETWORK_UTILS_receive_message(int sender,
                              uint8_t **message,
                              size_t *message_size)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    ssize_t recv_result = C_FAILURE;
    uint8_t *local_message = NULL;
    size_t local_message_size = 0;
    int ioctl_result = C_FAILURE;

    /* Validate parameters. */
    if ((INVALID_FILE_DESCRIPTOR == sender) || (NULL == message)) {
        status = CORONACHAT_STATUS_NETWORK_UTILS_RECEIVE_MESSAGE_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. sender=%d, message=%p",
                    (int)status,
                    sender,
                    (void *)message);
        goto l_cleanup;
    }

    /* Check how many bytes are available to be received. */
    ioctl_result = ioctl(sender, FIONREAD, &local_message_size);
    if (C_FAILURE == ioctl_result) {
        status = CORONACHAT_STATUS_NETWORK_UTILS_RECEIVE_MESSAGE_IOCTL_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to check number of bytes available with status %d", (int)status);
        goto l_cleanup;
    }

    /* Allocate a buffer to hold the message. */
    local_message = calloc(local_message_size, sizeof(*local_message));
    if (NULL == local_message) {
        status = CORONACHAT_STATUS_NETWORK_UTILS_RECEIVE_MESSAGE_ALLOC_MESSAGE_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to allocate message buffer with size %zu with status %d",
                               (local_message_size * sizeof(*local_message)),
                               (int)status);
        goto l_cleanup;
    }

    /* Receive the message over the socket. */
    recv_result = recv(sender, (void *)local_message, local_message_size, 0);
    if (C_FAILURE == recv_result) {
        status = CORONACHAT_STATUS_NETWORK_UTILS_RECEIVE_MESSAGE_RECV_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to receive message of size %zu over socket %d with status %d",
                               local_message_size,
                               sender,
                               (int)status);
        goto l_cleanup;
    }

    /* Transfer of ownership */
    *message = local_message;
    local_message = NULL;
    *message_size = local_message_size;

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    HEAPFREE(local_message);

    return status;
}


static enum coronachat_status
network_utils_iterate_address_list(struct addrinfo *address_list,
                                   network_utils_address_list_iterator_callback_t *iteration_action,
                                   void *iteration_action_parameter)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct addrinfo *current_address = NULL;
    bool should_break;

    /* Validate parameters. */
    if ((NULL == address_list) || (NULL == iteration_action) || (NULL == iteration_action_parameter)) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_ITERATE_ADDRESS_LIST_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. "
                    "address_list=%p, iteration_action=%zu, iteration_action_parameter=%p",
                    (int)status,
                    (void *)address_list,
                    (size_t)iteration_action,
                    iteration_action_parameter);
        goto l_cleanup;
    }

    /* Iterate over the address list. */
    for (current_address = address_list; NULL != current_address; current_address = current_address->ai_next) {

        /* Call the provided callback with the current address. */
        status = iteration_action(current_address, iteration_action_parameter, &should_break);
        if (true == CORONACHAR_TEST_FAILURE(status)) {
            DEBUG_PRINT("iteration_action failed with status %d", (int)status);
            goto l_cleanup;
        }

        /* Break the iteration if required. */
        if (true == should_break) {
            break;
        }
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    return status;
}


static enum coronachat_status
network_utils_bind_to_address(struct addrinfo *address,
                              struct network_utils_socket *to_bind,
                              bool *is_bound)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct network_utils_socket local_to_bind = {0};
    int setsockopt_result = C_FAILURE;
    int bind_result = C_FAILURE;
    int yes = 0;
    bool local_is_bound = false;

    /* Validate parameters. */
    if ((NULL == address) || (NULL == to_bind) || (NULL == is_bound)) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_BIND_TO_ADDRESS_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. address=%p, to_bind=%p, is_bound=%p",
                    (int)status,
                    (void *)address,
                    (void *)to_bind,
                    (void *)is_bound);
        goto l_cleanup;
    }

    /* Attempt to open a socket for the current server. */
    local_to_bind.descriptor = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (INVALID_FILE_DESCRIPTOR == local_to_bind.descriptor) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_BIND_TO_ADDRESS_SOCKET_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to open socket with status %d", (int)status);
        goto l_return;
    }

    /* Allow the socket to reuse the address it is about to be bound on. */
    yes = 1;
    setsockopt_result = setsockopt(local_to_bind.descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (C_FAILURE == setsockopt_result) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_BIND_TO_ADDRESS_SETSOCKOPT_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to set socket to reuse address with status %d", (int)status);
        goto l_cleanup;
    }

    /* Attempt to bind the socket to the current address. */
    bind_result = bind(local_to_bind.descriptor, address->ai_addr, address->ai_addrlen);
    if (C_FAILURE == bind_result) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_BIND_TO_ADDRESS_BIND_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to bind socket to address with status %d", (int)status);
        goto l_return;
    }

    /* Mark that the socket was successfully bound. */
    local_is_bound = true;

l_return:
    /* Transfer of ownership */
    *is_bound = local_is_bound;

    if (true == local_is_bound) {
        to_bind->descriptor = local_to_bind.descriptor;
        local_to_bind.descriptor = INVALID_FILE_DESCRIPTOR;
        to_bind->is_set_up = true;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    CLOSE(local_to_bind.descriptor);
    return status;
}


static enum coronachat_status
network_utils_connect_to_address(struct addrinfo *address,
                                 struct network_utils_socket *to_connect,
                                 bool *is_connected)
{
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;

    struct network_utils_socket local_to_connect = {0};
    int connect_result = C_FAILURE;
    bool local_is_connected = false;

    /* Validate parameters. */
    if ((NULL == address) || (NULL == to_connect) || (NULL == is_connected)) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_CONNECT_TO_ADDRESS_INVALID_PARAMETERS;
        DEBUG_PRINT("Failed due to invalid parameters with status %d. address=%p, to_connect=%p, is_connected=%p",
                    (int)status,
                    (void *)address,
                    (void *)to_connect,
                    (void *)is_connected);
        goto l_cleanup;
    }

    /* Attempt to open a socket for the current server. */
    local_to_connect.descriptor = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (INVALID_FILE_DESCRIPTOR == local_to_connect.descriptor) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_CONNECT_TO_ADDRESS_SOCKET_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to open socket with status %d", (int)status);
        goto l_return;
    }

    /* Attempt to connect the socket to the current address. */
    connect_result = connect(local_to_connect.descriptor, address->ai_addr, address->ai_addrlen);
    if (C_FAILURE == connect_result) {
        status = CORORNACHAT_STATUS_NETWORK_UTILS_CONNECT_TO_ADDRESS_CONNECT_FAILURE;
        DEBUG_PRINT_WITH_ERRNO("Failed to connect socket to address with status %d", (int)status);
        goto l_return;
    }

    /* Mark that the socket was successfully bound. */
    local_is_connected = true;

l_return:
    /* Transfer of ownership */
    *is_connected = local_is_connected;

    if (true == local_is_connected) {
        to_connect->descriptor = local_to_connect.descriptor;
        local_to_connect.descriptor = INVALID_FILE_DESCRIPTOR;
        to_connect->is_set_up = true;
    }

    /* End of function. */
    status = CORORNACHAT_STATUS_SUCCESS;

l_cleanup:
    CLOSE(local_to_connect.descriptor);
    return status;
}
