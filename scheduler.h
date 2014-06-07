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

error_t rsrc_req_pool_create(rsrc_id_t id, task_t *owner, rsrc_req_t **req);

error_t rsrc_req_pool_delete(rsrc_req_id_t id);

uint32_t rsrc_req_pool_size();

void scheduler_start();

error_t scheduler_add_task(task_t *t);

error_t scheduler_remove_task(task_t *t);

error_t scheduler_add_resource(rsrc_t *r);

error_t scheduler_remove_resource(rsrc_id_t id);

error_t scheduler_acquire_rsrc_immed(rsrc_t *r, task_t *acquirer);

error_t scheduler_acquire_rsrc(rsrc_t *r, task_t *owner, rsrc_req_id_t *id);

error_t scheduler_release_rsrc(rsrc_id_t id);

void scheduler_handle_interrupt();

void scheduler_syscall(task_t *cur);

void scheduler_do_round();

void scheduler_do_syscall();

error_t scheduler_close();

task_t *scheduler_get_current_task();

void scheduler_print_ready_tasks();
