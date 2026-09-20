#ifndef __UTIL_TEST_ASSERT__
#define __UTIL_TEST_ASSERT__

#include "unit_test/unit_test_log.h"

/**
 * ASSERTIONS
 *
 * ASSERTIONS check value which EXP evaluates; the value is either true(1) or
 * false(0). If it's false, log the failure to UT_OUTPUT (runtime) or
 * compilation log (compile time).
 * */

#define ASSERT(EXP, MSG)                                                       \
    do {                                                                       \
        int res = EXP;                                                         \
        if (res)                                                               \
            continue;                                                          \
        UT_FATAL("assertion failed at (%s:%u) %s: %s", __FILE__, __LINE__,     \
                 __func__, MSG);                                               \
        assert(0);                                                             \
    } while (0)

// ASSERTIONS

#endif // __UTIL_TEST_ASSERT__
