#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common/common.h"
#include "coronachat_status.h"

#include "utils/linked_list/linked_list.h"

enum coronachat_status heapfree(void *buf) {
    (void)free(buf);
    return CORORNACHAT_STATUS_SUCCESS;
}

int main() {
    enum coronachat_status status = CORORNACHAT_STATUS_UNINITIALIZED;
    int main_return = EXIT_FAILURE;

    struct LINKED_LIST_context *list = NULL;
    struct LINKED_LIST_node *nxt = NULL;
    struct LINKED_LIST_node *prv = NULL;
    size_t cnt = 0;

    status = LINKED_LIST_init(&list);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_init failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_append(list, NULL, strdup("1"), heapfree);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_append failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_append(list, NULL, strdup("2"), heapfree);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_append failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_append(list, NULL, strdup("3"), heapfree);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_append failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_next(list, NULL, &nxt, NULL);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_next failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_prepend(list, nxt, strdup("0"), heapfree);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_prepend failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_append(list, nxt, strdup("1.5"), heapfree);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_append failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_prev(list, NULL, &prv, NULL);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_prev failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_prev(list, prv, &prv, NULL);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_prev failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_prev(list, prv, &prv, NULL);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_prev failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_remove(list, prv);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_remove failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_prev(list, NULL, &prv, NULL);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_prev failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_remove(list, prv);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_remove failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_next(list, NULL, &nxt, NULL);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_next failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_remove(list, nxt);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_remove failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_get_node_count(list, &cnt);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_get_node_count failed with status %d", status);
        goto l_cleanup;
    }

    status = LINKED_LIST_free(list);
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        DEBUG_PRINT("LINKED_LIST_free failed with status %d", status);
        goto l_cleanup;
    }

l_cleanup:
    if (true == CORONACHAR_TEST_FAILURE(status)) {
        main_return = EXIT_FAILURE;
    }
    else {
        main_return = EXIT_SUCCESS;
    }

    return main_return;
}
