/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_LIB_H__
#define __GT_LIB_H__

#define GT_LIB_VERSION "1.10.0"
#define GT_LIB_DATETIME (201308260036ul)

#include "gt_config.h"

/* Common libraries ++ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <wctype.h>
#include <math.h>

#if defined(GT_CONFIG_LINUX)
	#include <sys/types.h>
	#include <unistd.h>
	#include <sys/stat.h> 
	#include <fcntl.h>
#endif

#if defined(GT_CONFIG_ANDROID)
	#include <sys/types.h>
	#include <unistd.h>
	#include <sys/stat.h> 
	#include <fcntl.h>
#endif

#if defined(GT_CONFIG_WINDOWS)
	#include <windows.h>
#else
	#include <unistd.h>
	#include <semaphore.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#if defined(GT_CONFIG_ANDROID)
		#include <linux/sem.h>
		#include <linux/shm.h>
		#include <linux/msg.h>
	#else
		#include <sys/sem.h>
		#include <sys/shm.h>
		#include <sys/msg.h>
#endif

#endif
/* Common libraries -- */

/* Core libraries ++ */
#include "gt_ret.h"
#include "gt_magic.h"
#include "gt_type.h"
#include "gt_mem.h"
#include "gt_lock.h"
#include "gt_math.h"
#include "gt_buffer.h"
#include "gt_array.h"
#include "gt_hash.h"
#include "gt_hash_map.h"
#include "gt_semaphore.h"
#include "gt_utf8.h"
#include "gt_string.h"
#include "gt_file.h"
#include "gt_log.h"
#include "gt_runtime.h"
#include "gt_timer_us.h"
#include "gt_color.h"
/* Core libraries -- */

#ifdef __cplusplus
extern "C"
{
#endif
    
int GtLib_InitLib();

#ifdef __cplusplus
}
#endif

#endif /* __GT_LIB_H__ */
