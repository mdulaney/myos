#include "error.h"
#include "resource.h"
#include "scheduler.h"

#define RESOURCE_MAILSLOT ((rsrc_type_t)1)

error_t create_mailslot(rsrc_t *r,
                        char *name,
                        rsrc_id_t id) {
    error_t result = ERROR_SUCCESS;
    r->id = id;
    r->type = RESOURCE_MAILSLOT;
    r->name = name;
    r->owner = OWNER_NONE;

    INIT_LIST_HEAD(&r->list);
    result = scheduler_add_resource(r);

    return result;
}


error_t delete_mailslot(rsrc_id_t id) {
    return scheduler_remove_resource(id);
}

error_t acquire(rsrc_t *rsrc, task_t *owner, rsrc_req_id_t *id) {
    return scheduler_acquire_rsrc(rsrc, owner, id);
}

error_t release(rsrc_req_id_t id) {
    return scheduler_release_rsrc(id);
}

