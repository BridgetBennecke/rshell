#include "input.cpp"
#include <unistd.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
	unsigned argsmax = 8;
	char* args[argsmax];
	getInput(argsmax, args);

	//for (unsigned l = 0; l < argsmax; ++l)
	//{
	//	cout << args[l] << endl;
	//}
	int test = execvp(args[0],args);
	if (test == -1)
	{
		perror("execvp");
		exit(1);
	}
	return 0;
}
