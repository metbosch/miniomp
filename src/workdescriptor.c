#ifndef __MINIOMP_WD_C__
#define __MINIOMP_WD_C__
#include "workdescriptor.h"
#include <stdlib.h>

miniomp_wd_t *new_miniomp_wd(void (*fn) (void *), void *fn_data) {
    miniomp_wd_t * ret = (miniomp_wd_t *)(malloc(sizeof(miniomp_wd_t)));
    miniomp_wd_init(ret, fn, fn_data);
    return ret;
}

void miniomp_wd_init(miniomp_wd_t *wd, void (*fn) (void *), void *fn_data) {
    wd->fn = fn;
    wd->fn_data = fn_data;
}

void miniomp_wd_run(miniomp_wd_t * wd) {
    wd->fn(wd->fn_data);
}

#endif
