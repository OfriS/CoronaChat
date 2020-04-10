/**
 * @file coronachat_status.h
 * @author Ofri Snir
 * @date 10.04.2020
 * @beirf All return value of coronachat functions.
 */

#ifndef __CORONACHAT_CORONACHAT_STATUS_H__
#define __CORONACHAT_CORONACHAT_STATUS_H__

enum coronachat_status {
    CORORNACHAT_STATUS_UNINITIALIZED = -1,
    CORORNACHAT_STATUS_SUCCESS = 0,

    /* linked_list */
    CORORNACHAT_STATUS_LINKED_LIST_INIT_INVALID_PARAMETERS = 1000,
    CORORNACHAT_STATUS_LINKED_LIST_INIT_ALLOC_LIST_CONTEXT_FAILURE,

    CORORNACHAT_STATUS_LINKED_LIST_FREE_INVALID_PARAMETERS,

    CORORNACHAT_STATUS_LINKED_LIST_APPEND_INVALID_PARAMETERS,

    CORORNACHAT_STATUS_LINKED_LIST_PREPEND_INVALID_PARAMETERS,

    CORORNACHAT_STATUS_LINKED_LIST_GET_NEXT_INVALID_PARAMETERS,

    CORORNACHAT_STATUS_LINKED_LIST_GET_PREV_INVALID_PARAMETERS,

    CORORNACHAT_STATUS_LINKED_LIST_REMOVE_INVALID_PARAMETERS,

    CORORNACHAT_STATUS_LINKED_LIST_ADD_NODE_INVALID_PARAMETERS,
    CORORNACHAT_STATUS_LINKED_LIST_ADD_NODE_INVALID_NODE_POSITION,

    CORORNACHAT_STATUS_LINKED_LIST_CREATE_NODE_INVALID_PARAMETERS,
    CORORNACHAT_STATUS_LINKED_LIST_CREATE_NODE_ALLOC_NODE_FAILURE,

    CORORNACHAT_STATUS_LINKED_LIST_DESTROY_NODE_INVALID_PARAMETERS,
    CORORNACHAT_STATUS_LINKED_LIST_DESTROY_NODE_INVALID_FREEING_METHOD,

    CORORNACHAT_STATUS_LINKED_LIST_GET_NODE_INVALID_PARAMETERS,
    CORORNACHAT_STATUS_LINKED_LIST_GET_NODE_INVALID_NODE_POSITION,
};

#define CORONACHAR_TEST_FAILURE(__status) (CORORNACHAT_STATUS_SUCCESS != __status)

#define BEST_EFFORT_UPDATE_STATUS(__new_status, __status) \
do { \
    if ((CORORNACHAT_STATUS_UNINITIALIZED != __status) && (CORORNACHAT_STATUS_SUCCESS != __status)) { \
        __status = __new_status; \
    } \
} while(0)

#endif /* !__CORONACHAT_CORONACHAT_STATUS_H__ */