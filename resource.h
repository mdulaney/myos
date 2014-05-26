#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "list.h"
#include <stdint.h>
#include <stdio.h>

#include "error.h"
#include "task.h"

#define RESOURCE_TYPE_MAILSLOT (1)

#define OWNER_NONE ((task_t *)NULL)

typedef uint32_t rsrc_type_t;
typedef uint32_t rsrc_id_t;

typedef struct _rsrc_t {
    rsrc_id_t id;
    char *name;
    rsrc_type_t type;
    task_t *owner;
    struct list_head list;
} rsrc_t;

typedef struct _rsrc_req_t {
    task_t *owner;
    rsrc_id_t rsrc_id;
    struct list_head list;
} rsrc_req_t;

rsrc_t *create_mailslot(rsrc_type_t type, char *name, rsrc_id_t id);

error_t delete_mailslot(rsrc_t *ms);

error_t acquire(rsrc_t *rsrc);

error_t release(rsrc_t *rsrc);

#endif