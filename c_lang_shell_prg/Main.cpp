//including library headers..
#include <iostream>
using namespace std;
//including user-defined functions...
#include "my_shell_kernel.h"
#include "my_shell_utility_fns.h"

int main()
{
	//If required print the initialization banner..
	My_Shell* shellObj = My_Shell::initializeMyShellObject();

	shellObj->startShell();
	return 0;
}