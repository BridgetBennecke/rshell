#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

char* getInput(char** cinput)
{
                                //Gets whole line of input via getline for strings
    string input;
    char hostname[200];
    if (gethostname(hostname,200) == -1)
    {
        perror("gethostname");
        exit(1);
    }
	cout << getlogin() << "@" << hostname << "$ ";
    getline(cin,input);
    cout << flush;
                                //Create pointer to cstring version of input
	*cinput = strdup(input.c_str());
    char comment[] = "#";       //Deletes commented section of input
    *cinput = strtok(*cinput, comment);

    char* temp = new char[3];   //Create variable to return
    for (unsigned j = 0; j < (input.size() - 1); ++j)   //Determines type of connector, if any
    {
        if (input[j] == ';')
        {
            strcpy(temp,";");
            return temp;
        }
        else if (input[j] == '|' && input[j+1] == '|')
        {
            strcpy(temp,"||");
            return temp;
        }
        else if (input[j] == '&' && input[j+1] == '&')
        {
            strcpy(temp,"&&");
            return temp;
        }
    }
    return NULL;
}
