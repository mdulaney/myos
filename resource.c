#include "error.h"
#include "resource.h"
#include "syscall.h"

extern error_t syscall_enter(uint64_t syscall_num, uint64_t num_args, ...);

error_t create_mailslot(char *name,
                        rsrc_id_t *id) {
    error_t result = ERROR_SUCCESS;

    result = syscall_enter(SYSCALL_CREATE_RESOURCE,
                           SYSCALL_CREATE_RESOURCE_ARGS,
                           RESOURCE_TYPE_MAILSLOT,
                           name,
                           id);


    return result;
}


error_t delete_mailslot(rsrc_id_t id) {
    error_t result = ERROR_SUCCESS;

    result = syscall_enter(SYSCALL_DELETE_RESOURCE,
                           SYSCALL_DELETE_RESOURCE_ARGS,
                           id);


    return result;
}

error_t acquire(rsrc_id_t id) {
    error_t result = ERROR_SUCCESS;

    result = syscall_enter(SYSCALL_ACQUIRE_RESOURCE,
                           SYSCALL_ACQUIRE_RESOURCE_ARGS,
                           id);


    return result;
}

error_t release(rsrc_id_t id) {
    error_t result = ERROR_SUCCESS;

    result = syscall_enter(SYSCALL_RELEASE_RESOURCE,
                           SYSCALL_RELEASE_RESOURCE_ARGS,
                           id);


    return result;
}

