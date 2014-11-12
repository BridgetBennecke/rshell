#include "input.cpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "split.cpp"
#include <stdio.h>
#include <iostream>

#include <cstdio>

using namespace std;

int main()
{
    while (1)
    {
        int init = fork();
        if (init != 0)
        {
            int* status;
            wait(&status);
            if (WEXITSTATUS(status) == 2)
            {
                return 0;
            }
        }
        else
        {
            break;
        }
    }
    unsigned argsmax = 200;
        char* cinput;                           //Cstring line to be inputted
	    char** args = new char*[argsmax+1];     //Allocate list of arguments
        char* mult = getInput(&cinput);         //mult is type of connector, getInput creates cinput

        //Will fork processes if necessary and create args
        split(cinput,args,mult);
        if (strcmp(args[0],"exit") == 0)
        {
            return 2;
        }



        //This group prints out args values for testing
        // printf("cinput: %s\n", cinput);
	    // for (unsigned l = 0; l < argsmax; ++l)
        // {
            // printf("l: %d\nargs[l]: %p\n", l, args[l]);
	        // cout << args[l] << endl;
	    // }

    //if (strcmp(args[0], "ls") == 0)
    //{
        //ls(args);
    //}
    //else if (strcmp(args[0], "cp") == 0)
    //{
        //cp(args);
    //}
    //else
    //{
	    int test = execvp(args[0],args);        //Run args, and print error if applicable
            if (test != 0)
	    {
	        perror("execvp");
	        exit(1);
	    }
    //}
	return 0;
}
