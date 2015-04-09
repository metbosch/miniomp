#ifndef __MINIOMP_TASK_C__
#define __MINIOMP_TASK_C__
#include "libminiomp.h"
#include "workdescriptor.h"
#include "specifickey.h"
#include "threadteam.h"
#include "thread.h"

void
GOMP_task (void (*fn) (void *), void *data, void (*cpyfn) (void *, void *),
           long arg_size, long arg_align, bool if_clause, unsigned flags,
           void **depend)
{
    void *new_data;
    if (__builtin_expect (cpyfn != NULL, 0)) {
	     char * buf =  malloc(sizeof(char) * (arg_size + arg_align - 1));
        char *arg = (char *) (((uintptr_t) buf + arg_align - 1)
                             & ~(uintptr_t) (arg_align - 1));
        cpyfn (arg, data);
        new_data = (void *)(arg);
    } else {
        new_data = malloc(arg_size);
    }
    miniomp_wd_t *new_wd = new_miniomp_wd_t(fn, new_data, miniomp_thread_get_wd(miniomp_get_self_thread()), TASK);
    miniomp_thread_team_t *team = miniomp_parallel_get_team(miniomp_get_parallel_region());
    miniomp_thread_team_push_task(team, new_wd);
}

void
GOMP_taskwait (void)
{
    miniomp_thread_taskwait(miniomp_get_self_thread());
}

#endif
