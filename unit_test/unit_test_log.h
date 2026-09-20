#ifndef __UNIT_TEST_LOG__
#define __UNIT_TEST_LOG__

#include <stdio.h>
#include <time.h>

#define UT_OUTPUT stderr
#define TIME_BUF_SZ 30

#define UT_LOG(format, ...)                                                    \
    do {                                                                       \
        char formatted_time[TIME_BUF_SZ];                                      \
        get_formatted_time(formatted_time);                                    \
        fprintf(UT_OUTPUT,                                                     \
                "\033[42m[LOG %s]\033[m \033[32m" format "\033[m\n",           \
                formatted_time, ##__VA_ARGS__);                                \
    } while (0)

#define UT_WARN(format, ...)                                                   \
    do {                                                                       \
        char formatted_time[TIME_BUF_SZ];                                      \
        get_formatted_time(formatted_time);                                    \
        fprintf(UT_OUTPUT,                                                     \
                "\033[43m[WRN %s]\033[m \033[33m" format "\033[m\n",           \
                formatted_time, ##__VA_ARGS__);                                \
    } while (0)

#define UT_FATAL(format, ...)                                                  \
    do {                                                                       \
        char formatted_time[TIME_BUF_SZ];                                      \
        get_formatted_time(formatted_time);                                    \
        fprintf(UT_OUTPUT,                                                     \
                "\033[41m[FAT %s]\033[m \033[31m" format "\033[m\n",           \
                formatted_time, ##__VA_ARGS__);                                \
    } while (0)

void get_formatted_time(char buf[TIME_BUF_SZ])
{
    time_t time_stamp_now;
    struct tm *cur_time;

    time_stamp_now = time(NULL);
    cur_time = localtime(&time_stamp_now);

    strftime(buf, TIME_BUF_SZ, "%Y/%m/%d %H:%M:%S", cur_time);
}

void flush_output(void) { fflush(UT_OUTPUT); }

#endif // __UNIT_TEST_LOG__
