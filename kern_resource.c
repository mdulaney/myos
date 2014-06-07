#include "error.h"
#include "kern_resource.h"
#include "resource.h"
#include "scheduler.h"

error_t kern_create_resource(rsrc_t *r,
                             rsrc_type_t type,
                             char *name,
                             rsrc_id_t id) {
    error_t result = ERROR_SUCCESS;
    r->id = id;
    r->type = type;
    r->name = name;
    r->owner = OWNER_NONE;

    INIT_LIST_HEAD(&r->list);
    result = scheduler_add_resource(r);

    return result;
}


error_t kern_delete_resource(rsrc_id_t id) {
    return scheduler_remove_resource(id);
}

error_t kern_acquire_resource(rsrc_t *rsrc, task_t *owner, rsrc_req_id_t *id) {
    error_t result = ERROR_SUCCESS;
    result = scheduler_acquire_rsrc_immed(rsrc, owner);
    if(result == ERROR_RESOURCE_IN_USE) {
        result = scheduler_acquire_rsrc(rsrc, owner, id);
    }

    return result;
}

error_t kern_release_resource(rsrc_req_id_t id) {
    return scheduler_release_rsrc(id);
}

