#ifndef __COMPILE_H__
#define __COMPILE_H__

#include "trapno.h"

/**
 * Forces caller to check if there is any error occurred. If any error occurred,
 * trapno is set to nonzero and message is pointed to a c_str that's not in
 * heap.
 * */
#define RESULT(type) type *presult, int32_t *ptrapno, const char **ptrap_message

#define R_RESULT (*presult)
#define R_TRAPNO (*ptrapno)
#define R_TRAPMSG (*ptrap_message)

/**
 * Jump to the end of current function when trap is not zero. Depending on
 * R_TRAPNO, R_TRAPMSG and flag eocf.
 * */
#define STOP(no)                                                               \
    do {                                                                       \
        if ((R_TRAPNO = no) != TRAP_OK) {                                      \
            R_TRAPMSG = get_trap_message(R_TRAPNO);                            \
            goto eocf;                                                         \
        }                                                                      \
    } while (0);

/**
 * Equals to STOP(no); R_RESULT = result;
 * */
#define SET_RESULT(result, no)                                                 \
    do {                                                                       \
        STOP(no);                                                              \
        R_RESULT = result;                                                     \
        R_TRAPMSG = get_trap_message(R_TRAPNO);                                \
    } while (0);

// Cross platform compilation.
#if defined(_WIN32) || defined(_WIN64)

#define DLLEXPORT __declspec(dllexport)

#else

#define DLLEXPORT

#endif // if defined(_WIN32) || defined(_WIN64)

#endif // __COMPILE_H__
