#include <stddef.h>
#include <stdint.h>

#include "include/compile.h"
#include "include/trapno.h"

DLLEXPORT void add(int32_t x, int32_t y, RESULT(int32_t))
{
    if (y > INT32_MAX - x) {
        STOP(TRAP_RES_OVERFLOW);
    }

    SET_RESULT(x + y, TRAP_OK);

eocf:
    return;
}
