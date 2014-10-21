#include "input.cpp"
#include <unistd.h>
#include "split.cpp"
#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
	unsigned argsmax = 11;
    char* cinput;                           //Cstring line to be inputted
	char** args = new char*[argsmax+1];     //Allocate list of arguments
    char* mult = getInput(&cinput);         //mult is type of connector, getInput creates cinput

    split(cinput,args,mult);                //Will fork processes if necessary and create args


	// for (unsigned l = 0; l < argsmax; ++l) //This group prints out args values for testing
    // {
        // cout << "in for loop" << endl;
	    // cout << args[l] << endl;
	// }


	int test = execvp(args[0],args);        //Run args, and print error if applicable
	if (test != 0)
	{
	    perror("execvp");
	    exit(1);
	}
	return 0;
}
