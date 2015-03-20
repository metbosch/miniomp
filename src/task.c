#include "libminiomp.h"

/* Called when encountering an explicit task directive.  If IF_CLAUSE is
   false, then we must not delay in executing the task. */
void
GOMP_task (void (*fn) (void *), void *data, void (*cpyfn) (void *, void *),
           long arg_size, long arg_align, bool if_clause, unsigned flags,
           void **depend)
{
    printf("TBI: a task has been encountered, I am executing it immediately\n");
    if (__builtin_expect (cpyfn != NULL, 0))
        {
	  char * buf =  malloc(sizeof(char) * (arg_size + arg_align - 1));
          char *arg = (char *) (((uintptr_t) buf + arg_align - 1)
                                & ~(uintptr_t) (arg_align - 1));
          cpyfn (arg, data);
          fn (arg);
        }
    else
        fn (data);
}

void
GOMP_taskwait (void)
{
    printf("TBI: Entered in taskwait, there should be no pending tasks, so I proceed\n");
}
