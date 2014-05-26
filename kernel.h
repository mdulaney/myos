#include "resource.h"

#define SYSCALL_ACQUIRE_MAILSLOT (1)
#define SYSCALL_RELEASE_MAILSLOT (2)

typedef int64_t syscall_t;

// System call functionality for tasks to invoke
void syscall(syscall_t type, ...);

// Dispatches the system call based off the type
void syscall_dispatch(syscall_t type, ...);

