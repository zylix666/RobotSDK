/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */
 
#ifndef __GT_MEM_H__
#define __GT_MEM_H__

#include "gt_lib.h"

#ifdef  __cplusplus
extern "C" {
#endif

// Memory barrier for share memory multi-processors.
#if defined(GT_CONFIG_GCC) || defined(GT_CONFIG_LLVM)
  #include <unistd.h>
  
  #if defined(GT_CONFIG_X86)
    #define gt_mb()    asm volatile("mfence":::"memory")
  #elif defined(GT_CONFIG_ARM)
    #define gt_mb()    asm volatile ("dmb" ::: "memory")
  #elif defined(GT_CONFIG_SH4)
    #define gt_mb()    asm volatile("": : :"memory")
  #elif defined(GT_CONFIG_IXP435)
    #define gt_mb()    asm volatile("": : :"memory")
  #else
    #error CPU is not defined!
  #endif
#elif defined(GT_CONFIG_VC)
  #define gt_mb() __asm {mfence}
#else
  #error Compiler is not defined!
#endif

int GtMem_PrintInfo();

void *gt_malloc(int size);

void *gt_calloc(int num, int size);

void *gt_realloc(void *ptr, int size);

int   gt_free(void *ptr);

#ifdef  __cplusplus
}
#endif

#endif  /* __GT_MEM_H__ */
