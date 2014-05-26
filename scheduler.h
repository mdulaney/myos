#include "list.h"

#include "error.h"
#include "resource.h"
#include "task.h"

typedef struct _scheduler_ctx {
    task_t ready_tasks;
    task_t suspended_tasks;
    rsrc_t resources;
    rsrc_req_t resource_reqs;
} scheduler_ctx;

scheduler_ctx *scheduler_get_ctx();

error_t scheduler_init();

void scheduler_start();

error_t scheduler_add_task(task_t *t);

error_t scheduler_remove_task(task_t *t);

error_t scheduler_add_resource(rsrc_t *r);

error_t scheduler_remove_resource(rsrc_t *r);

void scheduler_handle_interrupt();

void scheduler_do_round();

void scheduler_do_syscall();

error_t scheduler_close();

task_t *scheduler_get_current_task();

void scheduler_print_ready_tasks();
