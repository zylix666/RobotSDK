// libgt.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "libgt.h"


// This is an example of an exported variable
LIBGT_API int nlibgt=0;

// This is an example of an exported function.
LIBGT_API int fnlibgt(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see libgt.h for the class definition
Clibgt::Clibgt()
{
	return;
}
