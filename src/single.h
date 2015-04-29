#ifndef __MINIOMP_SINGLE_H__
#define __MINIOMP_SINGLE_H__

/**
  * Returns true to the first caller and false to all others untill the number of threads
  * in parallel region of callers calls have been performed
  */
bool miniomp_single_first();

/**
  * Returns false untill the number of threads in parallel region of callers calls have
  * been performed. For last one caller returns true
  */
bool miniomp_single_last();

#endif
