#ifndef __TRAPNO_H__
#define __TRAPNO_H__

typedef enum {
    TRAP_OK = 0,
    TRAP_RES_OVERFLOW = 1,
    TRAP_VARIETIES = 2,
} TrapNo;

static char *trap_messages[TRAP_VARIETIES];

char *get_trap_message(TrapNo no);

#endif // __TRAPNO_H__
