#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "scheduler.h"
#include "resource.h"
#include "task.h"

status_t tf1(void *param) {
    return 0;
}

status_t tf2(void *param) {
    return 0;
}

static void test_rsrc_req_pool(void **state) {
    task_t t1 = {};
    rsrc_req_t *req1 = NULL;
    int32_t i = 0;

    assert_int_equal(rsrc_req_pool_create(1, &t1, &req1), ERROR_SUCCESS);
    assert_int_equal(rsrc_req_pool_delete(req1->id), ERROR_SUCCESS);
    assert_int_equal(rsrc_req_pool_delete(req1->id), ERROR_DOES_NOT_EXIST);

    for(i = 0; i < rsrc_req_pool_size(); i++) {
        assert_int_equal(rsrc_req_pool_create(i, &t1, &req1), ERROR_SUCCESS);
    }

    assert_int_equal(rsrc_req_pool_create(1, &t1, &req1), ERROR_INSUFFICIENT_SYSTEM_RESOURCES);
}

static void test_simple_resources(void **state) {
    (void)state;
    rsrc_t rsrc1 = { 0 };
    task_t t1 = { 0 };
    rsrc_req_id_t req_id = 0;

    assert_int_equal(scheduler_init(), ERROR_SUCCESS);
    assert_int_equal(create_mailslot(&rsrc1, "my_resource", 1), ERROR_SUCCESS);
    assert_int_equal(acquire(&rsrc1, &t1, &req_id), ERROR_SUCCESS);
    assert_int_equal(release(rsrc1.id), ERROR_SUCCESS);
    assert_int_equal(delete_mailslot(rsrc1.id), ERROR_SUCCESS);

}

static void test_twotasks(void **state) {
    (void)state;
    task_t t1 = {}, t2 = {};

    assert_int_equal(scheduler_init(), ERROR_SUCCESS);
    assert_int_equal(create_task(&t1, "t1", 1, 0x1000, 0x2000, tf1, NULL), ERROR_SUCCESS);
    assert_int_equal(create_task(&t2, "t2", 2, 0x2000, 0x3000, tf2, NULL), ERROR_SUCCESS);

    assert_int_equal(scheduler_init(), ERROR_SUCCESS);

    assert_int_equal(scheduler_add_task(&t1), ERROR_SUCCESS);
    assert_true(scheduler_get_current_task() == &t1);

    assert_int_equal(scheduler_add_task(&t2), ERROR_SUCCESS);
    assert_true(scheduler_get_current_task() == &t1);

    scheduler_print_ready_tasks();
    scheduler_do_round();
    scheduler_print_ready_tasks();

    assert_true(scheduler_get_current_task() == &t2);

    assert_int_equal(scheduler_close(), ERROR_SUCCESS);
}

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_rsrc_req_pool),
        unit_test(test_simple_resources),
        unit_test(test_twotasks),
    };


    return run_tests(tests);
}

