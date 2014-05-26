#include "task.h"
#include "scheduler.h"

int64_t task_runner(struct _task_t *t) {
    t->tf(t->tf_param);
    scheduler_remove_task(t);
}

int64_t resume() {
}

error_t create_task(task_t *t,
                    char *name,
                    task_id_t id,
                    uint64_t stack_start,
                    uint64_t stack_end,
                    task_func tf,
                    void *tf_param) {

    t->name = name;
    t->id = id;
    t->stack_start = stack_start;
    t->stack_end = stack_end;
    t->tf = tf;
    t->tf_param = tf_param;
    t->resume = resume;
    t->task_runner = task_runner;
    return ERROR_SUCCESS;
}

error_t delete_task(task_t *t) {

}

