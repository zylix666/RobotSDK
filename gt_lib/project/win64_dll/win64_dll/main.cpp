
//#include "gt_config.h"
#include "gt_lib.h"
//#include "cuda.h"

#ifdef  __cplusplus
extern  "C" {
#endif 

//----------------------------------------
__declspec(dllexport) void hello();
__declspec(dllexport) int test();



//----------------------------------------
void hello()
{
	printf("hello world!");
}

int test()
{
	return 199;
}




//--------------------------------------
#ifdef  __cplusplus
}
#endif