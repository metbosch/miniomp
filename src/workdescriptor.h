#ifndef __MINIOMP_WD_H__
#define __MINIOMP_WD_H__

typedef struct {
    void (*fn) (void *);
    void *fn_data;
} miniomp_wd_t;

miniomp_wd_t *new_miniomp_wd_t(void (*fn) (void *), void *fn_data);

void destroy_miniomp_wd_t(miniomp_wd_t *wd);

void miniomp_wd_init(miniomp_wd_t *wd, void (*fn) (void *), void *fn_data);

void miniomp_wd_run(miniomp_wd_t * wd);

#endif
