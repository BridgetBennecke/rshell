#include <string.h>
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

void getInput(unsigned max, char* args[])
{
	string input;
	cout << "$ ";
    getline(cin,input);
    cout << flush;

	char* cinput = strdup(input.c_str());
	char delim[] = " ";

	args[0] = strtok(cinput, delim);
	for (unsigned k = 1; k < max; ++k)
	{
		args[k] = strtok(NULL,delim);
	}
	return;
}
