#include "scheduler.h"

#define LIST_PUSH_BACK(l, n) \

scheduler_ctx sched_ctx = {
};

scheduler_ctx *scheduler_get_ctx() { return &sched_ctx; }

error_t scheduler_init() {
    INIT_LIST_HEAD(&sched_ctx.ready_tasks.list);
    INIT_LIST_HEAD(&sched_ctx.suspended_tasks.list);
    INIT_LIST_HEAD(&sched_ctx.resources.list);
    INIT_LIST_HEAD(&sched_ctx.resource_reqs.list);

    return ERROR_SUCCESS;
}

void scheduler_start() {
    task_t *cur = NULL;
    cur = list_entry(sched_ctx.ready_tasks.list.next,
                     task_t,
                     list);

    cur->task_runner(cur);
}

error_t scheduler_add_task(task_t *t) {
    struct _task_t *cur = NULL;

    list_for_each_entry(cur, &sched_ctx.ready_tasks.list, list) {
        if(cur->id == t->id) {
            return ERROR_TASK_EXISTS;
        }
    }

    list_add_tail(&t->list, &sched_ctx.ready_tasks.list);

    return ERROR_SUCCESS;
}

error_t scheduler_remove_task(task_t *t) {
    list_del(&t->list);

    rsrc_req_t *req = NULL, *req_tmp = NULL;
    list_for_each_entry_safe(req,
                             req_tmp,
                             &sched_ctx.resource_reqs.list,
                             list) {
        if(req->owner->id != t->id) {
            return ERROR_UNEXPECTED_RESOURCE_REQUEST;
        }
    }

    rsrc_t *rsrc = NULL, *rsrc_tmp = NULL;
    list_for_each_entry_safe(rsrc,
                             rsrc_tmp,
                             &sched_ctx.resource_reqs.list,
                             list) {
        if(rsrc->owner->id != t->id) {
            return ERROR_UNEXPECTED_RESOURCE;
        }
    }

    return ERROR_SUCCESS;
}

error_t scheduler_add_resource(rsrc_t *r) {
    rsrc_t *rsrc = NULL;
    list_for_each_entry(rsrc, &sched_ctx.resources.list, list) {

        if(rsrc->id == r->id) {
            return ERROR_RESOURCE_EXISTS;
        }
    }

    list_add_tail(&r->list, &sched_ctx.resources.list);

    return ERROR_SUCCESS;
}

error_t scheduler_remove_resource(rsrc_t *r) {
    list_del(&r->list);

    rsrc_req_t *req = NULL, *req_tmp = NULL;
    list_for_each_entry_safe(req,
                             req_tmp,
                             &sched_ctx.resource_reqs.list,
                             list) {
        if(req->owner->id != r->id) {
            return ERROR_UNEXPECTED_RESOURCE_REQUEST;
        }
    }

}

// TODO: make this an interrupt handler
void scheduler_handle_interrupt() {
    // TODO: save state for the current task
    scheduler_do_round();
    // TODO: restore state for the new task
}

// do round-robin scheduling
void scheduler_do_round() {
    rsrc_req_t *req = NULL, *req_tmp = NULL;

    if(!list_empty(&sched_ctx.ready_tasks.list)) {
        list_move_tail(sched_ctx.ready_tasks.list.next,
                       &sched_ctx.ready_tasks.list);
    }

    // Examine outstanding resource requests and for all fulfilled requests
    // add the corresponding task to the back of the "ready" list
    list_for_each_entry_safe(req,
                             req_tmp,
                             &sched_ctx.resource_reqs.list,
                             list) {
        rsrc_t *rsrc = NULL, *rsrc_tmp = NULL;
        list_for_each_entry_safe(rsrc,
                                 rsrc_tmp,
                                 &sched_ctx.resources.list,
                                 list) {
            if(req->rsrc_id == rsrc->id && rsrc->owner == OWNER_NONE) {
                // Remove the request from the pending list
                list_del(&req->list);

                // Set the owner to the owner for the current request
                rsrc->owner = req->owner;

                // move from suspended to ready list
                list_move_tail(&rsrc->owner->list,
                               &sched_ctx.ready_tasks.list);
            }
        }
    }

}

void scheduler_do_syscall() {
    scheduler_do_round();
}

error_t scheduler_close() {
    INIT_LIST_HEAD(&sched_ctx.ready_tasks.list);
    INIT_LIST_HEAD(&sched_ctx.suspended_tasks.list);
    INIT_LIST_HEAD(&sched_ctx.resources.list);
    INIT_LIST_HEAD(&sched_ctx.resource_reqs.list);
    return ERROR_SUCCESS;
}

task_t *scheduler_get_current_task() {
    if(list_empty(&sched_ctx.ready_tasks.list))
        return NULL;
    else
        return list_entry(sched_ctx.ready_tasks.list.next, task_t, list);
}

void scheduler_print_ready_tasks() {
    task_t *cur = NULL;
    printf("#################\n");
    printf("# Ready Tasks   #\n");
    printf("#################\n");
    printf("Name     |      #\n");
    printf("---------+      #\n");
    list_for_each_entry(cur, &sched_ctx.ready_tasks.list, list) {
        printf("%8s |\n", cur->name);
    }
    printf("---------|\n");
}
