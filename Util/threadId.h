#ifndef UTIL_THREAD_ID_H
#define UTIL_THREAD_ID_H

#include <sys/types.h>
#include <stdio.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace util
{

inline pid_t gettid()
{
    return syscall( __NR_gettid );
}

} // namespace util
#endif