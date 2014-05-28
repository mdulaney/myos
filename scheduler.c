#include <stdbool.h>

#include "error.h"
#include "scheduler.h"

#define LIST_PUSH_BACK(l, n) \

#define RSRC_REQ_POOL_SIZE ((uint32_t)256)

scheduler_ctx sched_ctx = { 0 };

scheduler_ctx *scheduler_get_ctx() { return &sched_ctx; }

rsrc_req_t rsrc_req_pool [RSRC_REQ_POOL_SIZE] = { 0 };

error_t scheduler_init() {
    INIT_LIST_HEAD(&sched_ctx.ready_tasks.list);
    INIT_LIST_HEAD(&sched_ctx.suspended_tasks.list);
    INIT_LIST_HEAD(&sched_ctx.resources.list);
    INIT_LIST_HEAD(&sched_ctx.resource_reqs.list);

    return ERROR_SUCCESS;
}

error_t rsrc_req_pool_create(rsrc_id_t id, task_t *owner, rsrc_req_t **req) {
    int32_t idx = 0;
    *req = NULL;
    for(idx = 0; idx < RSRC_REQ_POOL_SIZE; idx++) {
        if(rsrc_req_pool[idx].in_use)
            continue;

        rsrc_req_pool[idx].in_use = true;
        rsrc_req_pool[idx].id = idx;
        rsrc_req_pool[idx].rsrc_id = id;
        rsrc_req_pool[idx].owner = owner;

        INIT_LIST_HEAD(&rsrc_req_pool[idx].list);

        *req = &rsrc_req_pool[idx];

        break;
    }

    if(*req == NULL) {
        return ERROR_INSUFFICIENT_SYSTEM_RESOURCES;
    }

    return ERROR_SUCCESS;
}

error_t rsrc_req_pool_delete(rsrc_req_id_t id) {
    if(!rsrc_req_pool[id].in_use) {
        return ERROR_DOES_NOT_EXIST;
    }

    rsrc_req_pool[id].in_use = false;
    return ERROR_SUCCESS;
}

uint32_t rsrc_req_pool_size() {
    return RSRC_REQ_POOL_SIZE;
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

        if(rsrc->id == r->id && rsrc->is_active) {
            return ERROR_RESOURCE_EXISTS;
        }
    }

    rsrc->is_active = true;
    list_add_tail(&r->list, &sched_ctx.resources.list);

    return ERROR_SUCCESS;
}

error_t scheduler_remove_resource(rsrc_id_t id) {
    rsrc_t *rsrc = NULL, *rsrc_tmp = NULL;
    list_for_each_entry_safe(rsrc,
                             rsrc_tmp,
                             &sched_ctx.resources.list,
                             list) {
        if(rsrc->id != id)
            continue;

        rsrc->is_active = false;
        list_del(&rsrc->list);

        break;
    }

    return ERROR_UNEXPECTED_RESOURCE_REQUEST;
}

error_t scheduler_acquire_rsrc_immed(rsrc_t *r, task_t *acquirer) {
    rsrc_t *cur = NULL;
    list_for_each_entry(cur, &sched_ctx.resources.list, list) {
        if(cur->id == r->id) {
            if(cur->owner == OWNER_NONE) {
                cur->owner = acquirer;
                return ERROR_SUCCESS;
            } else {
                return ERROR_RESOURCE_IN_USE;
            }
        }
    }

    return ERROR_RESOURCE_NOT_FOUND;
    // TODO: if the resource isn't available then suspend and add the current
    // task to the list of suspended tasks
}

error_t scheduler_acquire_rsrc(rsrc_t *r, task_t * owner, rsrc_req_id_t *id) {
    error_t result = ERROR_SUCCESS;
    rsrc_req_t *req = NULL;

    result = rsrc_req_pool_create(r->id, owner, &req);

    if(result < 0) {
        return result;
    }

    list_add_tail(&req->list, &sched_ctx.resource_reqs.list);

    *id = req->id;
    return ERROR_SUCCESS;
}

// TODO: needs to release resources, not resource requests
error_t scheduler_release_rsrc(rsrc_req_id_t id) {
    error_t result = ERROR_SUCCESS;
    rsrc_req_t *req = NULL, *req_tmp = NULL;
    list_for_each_entry_safe(req, req_tmp, &sched_ctx.resource_reqs.list, list) {
        if(req->id == id) {
            list_del(&req->list);

            result = rsrc_req_pool_delete(req->id);
            if(result < 0) {
                return result;
            }
            return ERROR_SUCCESS;
        }
    }

    return ERROR_RESOURCE_NOT_FOUND;
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
