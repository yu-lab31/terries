#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "arithmetic/arithmetic.h"
#include "include/trapno.h"
#include "unit_test/unit_test_assert.h"
#include "unit_test/unit_test_log.h"

void test_add(void)
{
    UT_LOG("test case %s from %s start", __func__, __FILE__);

    const int32_t x1 = 42, y1 = 69;
    // static_assert(INT32_MAX - x1 >= y1,
    //               "illegal test case: number (x1 & y1) too big");

    const char *message;
    int32_t trapno, result;
    add(x1, y1, &result, &trapno, &message);
    ASSERT(trapno == TRAP_OK, "unexpected trapno");
    ASSERT(result == x1 + y1, "unexpected result");

    const int32_t x2 = 2e9, y2 = 2e9;
    // static_assert(INT32_MAX - x2 < y2,
    //               "illegal test case: number (x2 & y2) too small");

    add(x2, y2, &result, &trapno, &message);
    ASSERT(trapno == TRAP_RES_OVERFLOW, "unexpected trapno");
    ASSERT(!strcmp(message, get_trap_message(trapno)),
           "unexpected trap message");

    UT_LOG("test case %s from %s end successfully", __func__, __FILE__);
}

int main(void)
{
    test_add();
    return 0;
}
