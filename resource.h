#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "list.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "error.h"
#include "resource_common.h"
#include "task.h"

error_t create_mailslot(char *name,
                        rsrc_id_t *id);

error_t delete_mailslot(rsrc_id_t id);

error_t acquire(rsrc_id_t id);

error_t release(rsrc_id_t id);

#endif
