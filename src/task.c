#include "libminiomp.h"
#include "workdescriptor.h"
#include "specifickey.h"
#include "threadteam.h"

/* Called when encountering an explicit task directive.  If IF_CLAUSE is
   false, then we must not delay in executing the task. */
void
GOMP_task (void (*fn) (void *), void *data, void (*cpyfn) (void *, void *),
           long arg_size, long arg_align, bool if_clause, unsigned flags,
           void **depend)
{
    //printf("TBI: a task has been encountered, I am executing it immediately\n");
    void *new_data;
    if (__builtin_expect (cpyfn != NULL, 0))
        {
	  char * buf =  malloc(sizeof(char) * (arg_size + arg_align - 1));
          char *arg = (char *) (((uintptr_t) buf + arg_align - 1)
                                & ~(uintptr_t) (arg_align - 1));
          cpyfn (arg, data);
          new_data = (void *)(arg);
        }
    else {
        new_data = malloc(arg_size);
    }
    miniomp_wd_t *new_wd = new_miniomp_wd_t(fn, new_data);
    miniomp_thread_team_t *team = miniomp_parallel_get_team(miniomp_get_thread_specifickey()->parallel_region);
    miniomp_thread_team_push_task(team, new_wd);
}

void
GOMP_taskwait (void)
{
//    printf("TBI: Entered in taskwait, there should be no pending tasks, so I proceed\n");
    miniomp_thread_team_t *team = miniomp_parallel_get_team(miniomp_get_thread_specifickey()->parallel_region);
    miniomp_thread_team_taskwait(team);
}
