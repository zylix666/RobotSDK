
#include <cstdlib>
#include <cstdio>

__declspec(dllexport) void hello();


void hello()
{
	printf("hello world!");
}