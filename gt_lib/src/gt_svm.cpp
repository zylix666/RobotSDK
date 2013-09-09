/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_svm.h"

#include "svm.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtSvmIsInit = 0;

int ConvertProblem(struct svm_problem *problem, double **data, int m, int n);
int FreeProblem(struct svm_problem *problem);
int DoCrossValidation(struct svm_problem *problem, struct svm_parameter *parameter, int fold);

int GtSvm_InitLib()  {
	if (gtSvmIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtSvmIsInit = 1;

	return GT_OK;
}

GtSvm_t *GtSvm_New() {
	if (gtSvmIsInit == 0) return NULL;
	
	GtSvm_t *svm = (GtSvm_t *)gt_calloc(1, sizeof(GtSvm_t));
	if (svm == NULL) {
		return NULL;
	}

    svm->problem = calloc(1, sizeof(struct svm_problem));
    if (svm->problem == NULL) {
		return NULL;
	}

    svm->parameter = calloc(1, sizeof(struct svm_parameter));
    if (svm->parameter == NULL) {
		return NULL;
	}

	//Set default parameter values
	struct svm_parameter *parameter = (struct svm_parameter *)svm->parameter;
    parameter->svm_type = C_SVC;
    parameter->kernel_type = RBF;
    parameter->degree = 0;
    parameter->gamma = 0.01;    // 1/num_features
    parameter->coef0 = 0;
    parameter->nu = 0.5;
    parameter->cache_size = 100;
    parameter->C = 10;
    parameter->eps = 1e-3;
    parameter->p = 0.1;
    parameter->shrinking = 1;
    parameter->probability = 0;
    parameter->nr_weight = 0;
    parameter->weight_label = NULL;
    parameter->weight = NULL;
	
	svm->magic = GT_MAGIC_SVM;

	return svm;
}

int GtSvm_Free(GtSvm_t *svm) {
	if (svm == NULL) return GT_ERROR_NULL_POINTER;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_MAGIC;

	svm->magic = GT_MAGIC_UNKNOWN;

	if (svm->model!= NULL) {
    	svm_free_model_content((struct svm_model *)svm->model);
    	struct svm_model *model = (struct svm_model *)svm->model;
    	svm_free_and_destroy_model(&model);

    	svm->model = NULL;
    }

    if (svm->problem != NULL) {
    	FreeProblem((struct svm_problem *)svm->problem);

    	svm->problem = NULL;
	}

	if (svm->parameter != NULL) {
		svm_destroy_param((struct svm_parameter *)svm->parameter);

		svm->parameter = NULL;
	}	

    GtLock_Free((GtLock_t *)svm->lock);
    
    gt_free(svm);
    
    return GT_OK;
}

int GtSvm_Lock(GtSvm_t *svm) {
	if (svm == NULL) return GT_ERROR_NULL_POINTER;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_MAGIC;

	if (svm->lock == NULL) {
		svm->lock = GtLock_New();
		if (svm->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)svm->lock);
}

int GtSvm_Unlock(GtSvm_t *svm) {
	if (svm == NULL) return GT_ERROR_NULL_POINTER;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)svm->lock);
}

int GtSvm_Test(GtSvm_t *svm) {
	if (svm == NULL) return GT_ERROR_NULL_POINTER;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_MAGIC;

	return GT_OK;
}

int GtSvm_LoadCsv(GtSvm_t *svm, gt_utf8 *path, int column, int row) {
	if (svm == NULL) return GT_ERROR_PARAMETER_0;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_PARAMETER_0;
	if (path == NULL) return GT_ERROR_PARAMETER_1;

	//Reading data from "iris.csv"
    FILE *fin;
    fin = fopen((const char*)path, "r");
    if (!fin) {
        GT_LOGE("Open file error!");
        return GT_ERROR_OPEN_FILE;
    }
    //Malloc matrix for data
    int i,j;
    char temp;
    double **data = (double **)malloc(sizeof(double*) * row);
    for (i = 0; i < row; i++) {
        data[i] = (double *)malloc(sizeof(double) * column);
        for (j = 0; j < column; j++) {
            fscanf(fin, "%lf", &data[i][j]);
            fscanf(fin, "%c", &temp);
        }
    }

    fclose(fin);

    //Convert data matrix to svm_problem
    if (ConvertProblem((struct svm_problem *)svm->problem, data, 150, 5) <= 0) {
        GT_LOGE("ConvertProblem error!!!");
        return GT_ERROR;
    }

	return GT_OK;
}

int GtSvm_DoCrossValidation(GtSvm_t *svm, int fold) {
	if (svm == NULL) return GT_ERROR_PARAMETER_0;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_PARAMETER_0;
	if (fold <= 0) return GT_ERROR_PARAMETER_1;

	//Do cross validation to evaluation our training result
    DoCrossValidation((struct svm_problem *)svm->problem, (struct svm_parameter *)svm->parameter, fold);

	return GT_OK;
}

int GtSvm_Train(GtSvm_t *svm, gt_utf8 *modelPath) {
	if (svm == NULL) return GT_ERROR_NULL_POINTER;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_MAGIC;

	//Build model for future
    svm->model = svm_train((struct svm_problem *)svm->problem, (struct svm_parameter *)svm->parameter);

    //Save model
    if (svm_save_model((const char *)modelPath, (struct svm_model *)svm->model) < 0) {
        GT_LOGE("Saving file error!!!");
        return GT_ERROR;
    }

	return GT_OK;
}

int GtSvm_LoadModel(GtSvm_t *svm, gt_utf8 *modelPath) {
	if (svm == NULL) return GT_ERROR_NULL_POINTER;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_MAGIC;

	//Load model from file
    svm->model = svm_load_model((const char *)modelPath);
    if (svm->model == NULL) {
        GT_LOGE("Loading file error");
        return GT_ERROR;
    }

	return GT_OK;
}

int GtSvm_Predict(GtSvm_t *svm, int num, double *value) {
	if (svm == NULL) return GT_ERROR_PARAMETER_0;
	if (svm->magic != GT_MAGIC_SVM) return GT_ERROR_PARAMETER_0;
	if (num <= 0) return GT_ERROR_PARAMETER_1;
	if (value == NULL) return GT_ERROR_PARAMETER_2;

	struct svm_problem *problem = (struct svm_problem *) svm->problem;

	if (svm->model == NULL) {
		return GT_ERROR;
	}

	if (svm->problem == NULL) {
		return GT_ERROR;
	}

	//Predict a new data and give it a classification
    *value = svm_predict((struct svm_model *)svm->model, problem->x[num]);

	return GT_OK;
}

int ConvertProblem(struct svm_problem *problem, double **data, int m, int n) {
    //Svm_porblem is special structure for storing data
    //Data is a mxn matrix which contain training data and include "target" at first column
    //M is number of rows
    //N is number of columns

    int i, j, count;
    problem->y = (double *)malloc(sizeof(double) * m);

    if (problem->y == NULL) {
        GT_LOGE("Malloc error in =ConvertProblem=!!!");
        return 0;
    }

    problem->l = m;
    problem->x = (struct svm_node **)malloc(sizeof(struct svm_node *) * m);

    if (problem->x == NULL) {
        GT_LOGE("Malloc error in =ConvertProblem=!!!");
        return 0;
    }

    for (i = 0; i < m; i++) {
        //Count number of non-zero elements
        count = 0;
        for (j = 0; j < n; j++) {
            if (data[i][j] != 0) {
                count += 1;
            }
        }

        //Pass data to "problem"    
        problem->x[i] = (struct svm_node*)malloc(sizeof(struct svm_node) * count);
        if (problem->x[i] == NULL) {
        	GT_LOGE("Malloc error in =ConvertProblem=!!!");
        	return 0;
        }

        for (j = 0; j < n; j++) {
            problem->x[i][j].index = j + 1;  
            problem->x[i][j].value = data[i][j+1];  
        }

        //Specify the last node' index be -1
        problem->x[i][j].index = -1;

        //Pass the target value to "y"
        problem->y[i] = data[i][0];
    }
    return 1;
}

int FreeProblem(struct svm_problem *problem) {
    int i=0;

    while(i < problem->l) {
        free(problem->x[i++]);
    }

    free(problem->x);
    free(problem->y);

    return 1;
}

int DoCrossValidation(struct svm_problem *problem, struct svm_parameter *parameter, int nr_fold) {
    int i;
    int total_correct = 0;
    double total_error = 0;
    double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
    double *target = (double*)malloc(sizeof(double) * problem->l);

    svm_cross_validation(problem, parameter, nr_fold, target);
    if (parameter->svm_type == EPSILON_SVR ||
       parameter->svm_type == NU_SVR) {
        for (i = 0; i < problem->l; i++) {
            double y = problem->y[i];
            double v = target[i];
            total_error += (v - y) * (v - y);
            sumv += v;
            sumy += y;
            sumvv += v * v;
            sumyy += y * y;
            sumvy += v * y;
        }
        GT_LOGE("Cross Validation Mean squared error = %g", total_error/problem->l);
        GT_LOGE("Cross Validation Squared correlation coefficient = %g",
            ((problem->l * sumvy - sumv * sumy) * (problem->l * sumvy - sumv * sumy))/
            ((problem->l * sumvv - sumv * sumv) * (problem->l * sumyy-sumy * sumy))
            );
    }
    else {
        for (i = 0; i < problem->l; i++)
            if (target[i] == problem->y[i])
                ++total_correct;
        GT_LOGE("Cross Validation Accuracy = %g%%", 100.0 * total_correct / problem->l);
    }

    free(target);

    return 1;
}

#ifdef __cplusplus
}
#endif
