#ifndef __MINIOMP_WD_C__
#define __MINIOMP_WD_C_
#include <stdlib.h>
#include "workdescriptor.h"
#include "libminiomp.h"
#include "specifickey.h"
#include "thread.h"

miniomp_wd_t *new_miniomp_wd_t(void (*fn) (void *), void *fn_data, miniomp_wd_t *parent, miniomp_wd_kind_t type) {
   miniomp_wd_t * ret = (miniomp_wd_t *)(malloc(sizeof(miniomp_wd_t)));
   miniomp_wd_init(ret, fn, fn_data, parent, type);
   return ret;
}

void destroy_miniomp_wd_t(miniomp_wd_t *wd) {
   free(wd->fn_data);
   free(wd);
}

void miniomp_wd_init(miniomp_wd_t *wd, void (*fn) (void *), void *fn_data, miniomp_wd_t *parent, miniomp_wd_kind_t type) {
   wd->fn = fn;
   wd->fn_data = fn_data;
   wd->parent = parent;
   wd->components = 0;
   wd->executed_components = 0;
   wd->type = type;
   wd->status = CREATED;
   if (parent != NULL) {
      miniomp_wd_new_component(parent);
   }
}

void miniomp_wd_finish(miniomp_wd_t *wd) {
   if (wd == NULL) return;
   int executed = __sync_add_and_fetch(&wd->executed_components, 1);
   if (executed == wd->components && wd->type == TASK && wd->status == EXECUTED) {
      destroy_miniomp_wd_t(wd);
   }
   __sync_synchronize();
}

void miniomp_wd_new_component(miniomp_wd_t *wd) {
   __sync_add_and_fetch(&wd->components, 1);
}

bool miniomp_wd_has_components(miniomp_wd_t *wd) {
   return wd->components > wd->executed_components;
}

void miniomp_wd_run(miniomp_wd_t *wd) {
   miniomp_thread_t *self = miniomp_get_self_thread();
   miniomp_wd_t *old = miniomp_thread_get_wd(self);
   DEBUG("Starting WD execution");
   if (old != NULL) {
      old->status = BLOCKED;
   }
   wd->status = RUNNING;
   miniomp_thread_set_wd(self, wd);
   wd->fn(wd->fn_data);
   miniomp_thread_set_wd(self, old);
   miniomp_wd_finish(wd->parent);
   if (old != NULL) {
      old->status = RUNNING;
   }
   DEBUG("WD executed");

   if (wd->components == 0  && wd->type == TASK) {
      destroy_miniomp_wd_t(wd);
   } else if (wd->components == wd->executed_components && wd->type == TASK) {
      destroy_miniomp_wd_t(wd);
   } else {
      wd->status = EXECUTED;
   }
}

#endif
