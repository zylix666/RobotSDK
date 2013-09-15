/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_template.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtTemplateIsInit = 0;

int GtTemplate_InitLib() {
	if (gtTemplateIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtTemplateIsInit = 1;

	return GT_OK;
}

GtTemplate_t *GtTemplate_New() {
	GtTemplate_t *template;

	if (gtTemplateIsInit == 0) return NULL;
	
	template = (GtTemplate_t *)gt_calloc(1, sizeof(GtTemplate_t));
	if (template == NULL) {
		return NULL;
	}

	template->data = gt_calloc(1, sizeof(int));
	if (template->data == NULL) {
		gt_free(template);
		return NULL;
	}
	
	template->magic = GT_MAGIC_TEMPLATE;

	return template;
}

int GtTemplate_Free(GtTemplate_t *template) {
	if (template == NULL) return GT_ERROR_NULL_POINTER;
	if (template->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	template->magic = GT_MAGIC_UNKNOWN;

	gt_free(template->data);
	
    GtLock_Free((GtLock_t *)template->lock);
    
    gt_free(template);
    
    return GT_OK;
}

int GtTemplate_Lock(GtTemplate_t *template) {
	if (template == NULL) return GT_ERROR_NULL_POINTER;
	if (template->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	if (template->lock == NULL) {
		template->lock = GtLock_New();
		if (template->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)template->lock);
}

int GtTemplate_Unlock(GtTemplate_t *template) {
	if (template == NULL) return GT_ERROR_NULL_POINTER;
	if (template->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)template->lock);
}

int GtTemplate_Test(GtTemplate_t *template) {
	if (template == NULL) return GT_ERROR_NULL_POINTER;
	if (template->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
