


#include "gt_config.h"
#include "gt_lib.h"
#include "cuda.h"


__declspec(dllexport) void hello();


void hello()
{
	printf("hello world!");
}