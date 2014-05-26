#ifndef _ERROR_H
#define _ERROR_H

#include <stdint.h>

typedef uint64_t error_t;

#define ERROR_SUCCESS (0x0)
#define ERROR_BASE (0x8000000000000000L)
#define ERROR_TASK_EXISTS                               (ERROR_BASE + 0)
#define ERROR_RESOURCE_EXISTS                           (ERROR_BASE + 1)
#define ERROR_UNEXPECTED_RESOURCE                       (ERROR_BASE + 2)
#define ERROR_UNEXPECTED_RESOURCE_REQUEST               (ERROR_BASE + 3)
#define ERROR_UNEXPECTED_TASK                           (ERROR_BASE + 4)

error_t g_errno;

static char *error_table[] = {
    "task already exists",
    "resource already exists",
    "unexpected resource",
    "unexpected resource request",
    "unexpected task",
};

char *error_str(error_t err);

#endif
