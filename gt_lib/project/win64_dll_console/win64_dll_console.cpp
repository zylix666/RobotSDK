

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <windows.h>


using namespace std;

//------------dll import-----------------------

typedef int (*testfunc)(void);
typedef void (*hellofunc)(void);

//---------------------------------------------


int main(){

//define dll function name
 testfunc test1 ;
 hellofunc hello1;

 //call dll
 HINSTANCE hLibrary = LoadLibrary(L"win64_dll.dll");

 //check the function of dll
 if(hLibrary)
 {
 test1 = (testfunc)GetProcAddress(hLibrary, "test");
 hello1= (hellofunc)GetProcAddress(hLibrary, "hello");
 }
 else
 {
	 printf("Failed To Load Library\n");
 }


 //test
 if(test1)
 {

 printf("%d\n",test1());
 hello1();

 }

 
 

 FreeLibrary(hLibrary);
 system("pause");



}