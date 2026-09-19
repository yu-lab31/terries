#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

#include "trapno.h"

pthread_once_t done;

void init_trap_messages()
{
    trap_messages[TRAP_OK] = "(everything is fine)";
    trap_messages[TRAP_RES_OVERFLOW] =
        "result out of range defined by its type";
}

char *get_trap_message(TrapNo no)
{
    pthread_once(&done, init_trap_messages);
    if (no < 0 || no >= TRAP_VARIETIES) {
        return NULL;
    }
    return trap_messages[no];
}
