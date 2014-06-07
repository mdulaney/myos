
#define RESOURCE_TYPE_MAILSLOT ((rsrc_type_t)1)

#define OWNER_NONE ((task_t *)NULL)

typedef uint32_t rsrc_type_t;
typedef uint32_t rsrc_id_t;
typedef uint32_t rsrc_req_id_t;

typedef struct _rsrc_t {
    rsrc_id_t id;
    char *name;
    bool is_active;
    rsrc_type_t type;
    task_t *owner;
    struct list_head list;
} rsrc_t;

typedef struct _rsrc_req_t {
    rsrc_req_id_t id;
    bool in_use;
    task_t *owner;
    rsrc_id_t rsrc_id;
    struct list_head list;
} rsrc_req_t;

error_t kern_create_resource(rsrc_t *r,
                             rsrc_type_t type,
                             char *name,
                             rsrc_id_t id);

error_t kern_delete_resource(rsrc_id_t id);

error_t kern_acquire_resource(rsrc_t *rsrc, task_t *owner, rsrc_req_id_t *id);

error_t kern_release_resource(rsrc_req_id_t id);
