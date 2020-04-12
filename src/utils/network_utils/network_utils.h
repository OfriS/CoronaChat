/**
 * @file network_utils.h
 * @author Ofri Snir
 * @date 11.04.2020
 * @beirf Implementation of functions for creating and using TCP sockets for transmitting data.
 *
 *  The module exports some basic functions for setting up a TCP server and client and transmitting messages between
 *  them.
 *
 *  Server:
 *   In order to create a server, the caller must call NETWORK_UTILS_setup_listener() in order to receive a listening
 *   socket descriptor over which he can accept new client connections.
 *   Then the caller must call NETWORK_UTILS_accept_connection() and pass it the socket descriptor returned by
 *   NETWORK_UTILS_setup_listener() in order to accept any new clients.
 *
 *  Client:
 *   In order to connect to a TCP server the caller must simply call NETWORK_UTILS_connect() and pass the server's
 *   address information.
 *
 *  Once the client/server has been set up, they may call NETWORK_UTILS_send_message() and
 *  NETWORK_UTILS_receive_message() in order to transmit data.
 *
 *  When the client/server is done transmitting data, all sockets returned by this module's function must be closed by
 *  calling CLOSE().
 */

#ifndef __CORONACHAT_NETWORK_UTILS_H__
#define __CORONACHAT_NETWORK_UTILS_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>

#include "common/common.h"
#include "coronachat_status.h"

/**
 * @brief Sets up a listening TCP socket on the provided IP and port.
 *
 * @param[in] host - opt - IP on which to open the socket. If not supplied localhost is used.
 * @param[in] port - req - Port on which to open the socket.
 * @param[out] listener - req - Open listening socket set up to accept connections. Must later be close by calling
 *  CLOSE().
 *
 * @return coronachat_status
 */
enum coronachat_status
NETWORK_UTILS_setup_listener(const char *host,
                             const char *port,
                             int *listener);


/**
 * @brief Accept a client trying to connect to the listener socket and return it's address information.
 *
 * @param[in] listener - req - Listener socket through which to accept connections.
 * @param[out] client_socket - req - TCP socket connected to the client. Must later be closed by calling CLOSE().
 * @param[out] client_address - req - Client's address. Must later be freed by calling HEAPFREE().
 *
 * @return coronachat_status
 */
enum coronachat_status
NETWORK_UTILS_accept_connection(int listener,
                                int *client_socket,
                                struct sockaddr_storage **client_address);


/**
 * @brief Connect to the server running on the provided IP and port.
 *
 * @param[in] server_ip - req - Server's IP.
 * @param[in] server_port - req - Server's port.
 * @param[out] server_socket - req - TCP socket connected to the server. Must later be closed by calling CLOSE().
 *
 * @return coronachat_status
 */
enum coronachat_status
NETWORK_UTILS_connect(const char *server_ip,
                      const char *server_port,
                      int *server_socket);


/**
 * @brief Send a message over the provided socket.
 *
 * @param[in] receiver - req - Socket over which to send the message.
 * @param[in] message - req - Message to be sent.
 * @param[in] message_size - req - Size of message in bytes.
 *
 * @return coronachat_status
 */
enum coronachat_status
NETWORK_UTILS_send_message(int receiver,
                           const uint8_t *message,
                           size_t message_size);


/**
 * @brief Receive a message over the provided socket.
 *
 * @param[in] sender - req - Socket over which to receive the message.
 * @param[out] message - req - Message received over the socket. Must later be freed by calling HEAPFREE().
 * @param[out] message_size - req - Size of message in bytes.
 *
 * @return coronachat_status
 */
enum coronachat_status
NETWORK_UTILS_receive_message(int sender,
                              uint8_t **message,
                              size_t *message_size);

#endif /* !__CORONACHAT_NETWORK_UTILS_H__ */