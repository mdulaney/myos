#ifndef _TASK_H
#define _TASK_H

#include "list.h"
#include <linux/types.h>

#include "error.h"

#define TASK_READY        (1)
#define TASK_EXECUTING    (2)
#define TASK_SUSPENDED    (4)
#define TASK_DORMANT      (8)

typedef int64_t status_t;
typedef status_t (*task_func)(void *param);
typedef uint32_t task_id_t;

typedef struct _task_t {
    task_id_t id;
    char *name;
    uint64_t stack_start;
    uint64_t stack_end;
    uint32_t task_state;
    task_func tf;
    void *tf_param;
    int64_t (*task_runner)(struct _task_t *t);
    int64_t (*resume)();
    struct list_head list;
} task_t;

error_t create_task(task_t *t,
                    char *name,
                    task_id_t id,
                    uint64_t stack_start,
                    uint64_t stack_end,
                    task_func tf,
                    void *tf_param);

error_t delete_task(task_t *t);
#endif
