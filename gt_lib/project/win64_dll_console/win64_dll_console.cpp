

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <windows.h>


using namespace std;


typedef int (*testfunc)(void);


int main(){

//printf("123\n");



 testfunc test1 ;

 HINSTANCE hLibrary = LoadLibrary(L"win64_dll.dll");

 if(hLibrary)
 {
 test1 = (testfunc)GetProcAddress(hLibrary, "test");


 if(test1)
 {


 int a=test1();
 printf("%d",a);
 }

 FreeLibrary(hLibrary);
 }
 else
 {
 std::cout << "Failed To Load Library" << std::endl;
 }
 
system("pause");



}