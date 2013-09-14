
#define GT_CONFIG_WINDOWS  
#define GT_CONFIG_VC

#include "gt_lib.h"



__declspec(dllexport) void hello();


void hello()
{
	printf("hello world!");
}