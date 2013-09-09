/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_matrix.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtMatrixIsInit = 0;

int GtMatrix_InitLib() {
	if (gtMatrixIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtMatrixIsInit = 1;

	return GT_OK;
}

GtMatrix_t *GtMatrix_New() {
	if (gtMatrixIsInit == 0) return NULL;
	
	GtMatrix_t *matrix = (GtMatrix_t *)gt_calloc(1, sizeof(GtMatrix_t));
	if (matrix == NULL) {
		return NULL;
	}
	
	matrix->magic = GT_MAGIC_MATRIX;

	return matrix;
}

int GtMatrix_Free(GtMatrix_t *matrix) {
	if (matrix == NULL) return GT_ERROR_NULL_POINTER;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_MAGIC;

	matrix->magic = GT_MAGIC_UNKNOWN;

	if (matrix->m > 0) {
		int i;
		int n = matrix->n;
		for (i = 0; i < n; i++) {
			gt_free(matrix->value[i]);
		}
		gt_free(matrix->value);
	}

	gt_free(matrix->value);
	
    GtLock_Free((GtLock_t *)matrix->lock);
    
    gt_free(matrix);
    
    return GT_OK;
}

int GtMatrix_Lock(GtMatrix_t *matrix) {
	if (matrix == NULL) return GT_ERROR_NULL_POINTER;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_MAGIC;

	if (matrix->lock == NULL) {
		matrix->lock = GtLock_New();
		if (matrix->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)matrix->lock);
}

int GtMatrix_Unlock(GtMatrix_t *matrix) {
	if (matrix == NULL) return GT_ERROR_NULL_POINTER;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)matrix->lock);
}

int GtMatrix_Test(GtMatrix_t *matrix) {
	if (matrix == NULL) return GT_ERROR_NULL_POINTER;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_MAGIC;

	return GT_OK;
}

int GtMatrix_Init(GtMatrix_t *matrix, int n, int m) {
	if (matrix == NULL) return GT_ERROR_PARAMETER_0;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_0;
	if (n <= 0) return GT_ERROR_PARAMETER_1;
	if (m <= 0) return GT_ERROR_PARAMETER_2;

	int i;

	if (matrix->value != NULL) {
		for (i = 0; i < n; i++) {
			gt_free(matrix->value[i]);
		}
		gt_free(matrix->value);
	}
	
	matrix->value = gt_calloc(n, sizeof(double *));
	if (matrix->value == NULL) {
		return GT_ERROR_NO_MEMORY;
	}

	for (i = 0; i < n; i++) {
		matrix->value[i] = gt_calloc(m, sizeof(double));
		if (matrix->value == NULL) {
			for (i = i; i >= 0; i--) {
				gt_free(matrix->value[i]);
			}
			gt_free(matrix->value);

			return GT_ERROR_NO_MEMORY;
		}
	}

	matrix->n = n;
	matrix->m = m;

	return GT_OK;
}

int GtMatrix_Set(GtMatrix_t *matrix, int n, int m, double value) {
	if (matrix == NULL) return GT_ERROR_PARAMETER_0;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_0;
	if (n < 0) return GT_ERROR_PARAMETER_1;
	if (n >= matrix->n) return GT_ERROR_PARAMETER_1;
	if (m < 0) return GT_ERROR_PARAMETER_2;
	if (m >= matrix->m) return GT_ERROR_PARAMETER_2;

	matrix->value[n][m] = value;

	return GT_OK;
}

int GtMatrix_Get(GtMatrix_t *matrix, int n, int m, double *value) {
	if (matrix == NULL) return GT_ERROR_PARAMETER_0;
	if (matrix->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_0;
	if (n < 0) return GT_ERROR_PARAMETER_1;
	if (n >= matrix->n) return GT_ERROR_PARAMETER_1;
	if (m < 0) return GT_ERROR_PARAMETER_2;
	if (m >= matrix->m) return GT_ERROR_PARAMETER_2;
	if (value == NULL) return GT_ERROR_PARAMETER_3;

	*value = matrix->value[n][m];

	return GT_OK;
}

int GtMatrix_Add(GtMatrix_t *matrix_a, GtMatrix_t *matrix_b, GtMatrix_t *matrix_ab) {
	if (matrix_a == NULL) return GT_ERROR_PARAMETER_0;
	if (matrix_a->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_0;
	if (matrix_b == NULL) return GT_ERROR_PARAMETER_1;
	if (matrix_b->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_1;
	if (matrix_ab == NULL) return GT_ERROR_PARAMETER_2;
	if (matrix_ab->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_2;
	if (matrix_a->n < 0)  return GT_ERROR_PARAMETER_0;
	if (matrix_a->m < 0)  return GT_ERROR_PARAMETER_0;
	if (matrix_b->n < 0)  return GT_ERROR_PARAMETER_1;
	if (matrix_b->m < 0)  return GT_ERROR_PARAMETER_1;

	if (matrix_a->n != matrix_b->n) return GT_ERROR_PARAMETER_1;
	if (matrix_a->m != matrix_b->m) return GT_ERROR_PARAMETER_1;

	int n = matrix_a->n;
	int m = matrix_a->m;
	int i;
	int j;

	GtMatrix_Init(matrix_ab, n, m);

	for (j = 0; j < m; j++) {
		for (i = 0; i < n; i++) {
			matrix_ab->value[i][j] = matrix_a->value[i][j] + matrix_b->value[i][j];
		}
	}
	return GT_OK;
}

int GtMatrix_Mul(GtMatrix_t *matrix_a, GtMatrix_t *matrix_b, GtMatrix_t *matrix_ab) {
	if (matrix_a == NULL) return GT_ERROR_PARAMETER_0;
	if (matrix_a->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_0;
	if (matrix_b == NULL) return GT_ERROR_PARAMETER_1;
	if (matrix_b->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_1;
	if (matrix_ab == NULL) return GT_ERROR_PARAMETER_2;
	if (matrix_ab->magic != GT_MAGIC_MATRIX) return GT_ERROR_PARAMETER_2;
	if (matrix_a->n < 0)  return GT_ERROR_PARAMETER_0;
	if (matrix_a->m < 0)  return GT_ERROR_PARAMETER_0;
	if (matrix_b->n < 0)  return GT_ERROR_PARAMETER_1;
	if (matrix_b->m < 0)  return GT_ERROR_PARAMETER_1;
	if (matrix_a->m != matrix_b->n) return GT_ERROR_PARAMETER_1;

	int n = matrix_a->n;
	int m = matrix_b->m;
	int i;
	int j;
	int k;

	GtMatrix_Init(matrix_ab, n, m);

	for (i = 0; i < n; i++) {
		for (k = 0; k < matrix_a->m; k++) {
			for (j=0; j<matrix_b->m; j++) {
				matrix_ab->value[i][j] += matrix_a->value[i][k] * matrix_b->value[k][j];
			}
		}
	}

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
