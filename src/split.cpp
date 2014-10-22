#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include <cstdio>
using namespace std;

unsigned count(char split[],char* input)                //Returns how many seperate arguments are listed
{
    unsigned count = 1;
    for (unsigned c = 0; c < strlen(input); ++c)
    {
        if (input[c] == split[0])
        {
            count += 1;
            if (split[1] == '&' || split[1] == '|')
            {
                ++c;
            }
        }
    }
    return count;
}


void split(char* cinput, char* args[], char split[])
{
    char* input = strdup(cinput);                       //If there is only 1 command
    if (split == NULL)
    {                                                   //then split its args and run it
        char delim[] = " ";
        args[0] = strtok(input,delim);
        for (unsigned h = 1; h < strlen(input); ++h)
        {
            args[h] = strtok(NULL,delim);
            if (args[h] == NULL)
            {
                break;
            }
        }
        return;
    }
    else                                                //If there are multiple commands
    {
        unsigned size = count(split,input);
        cout << "size = " << size << endl;
        char* temp = strtok(input,split);
        for (unsigned k = 1; k < size; ++k)            //Fork enough processes to handle the number of commands
        {                                               //and run in order
            int t = fork();
            if (t != 0)
            {
                wait(NULL);
                temp = strtok(NULL,split);
            }
            if (t == 0)
            {
                break;
            }
        }
        char delim[] = " ";                             //Break commands into args and run
        args[0] = strtok(temp,delim);
        for (unsigned i = 1; i < strlen(temp); ++i)
        {
            args[i] = strtok(NULL,delim);
        }
        return;
    }
}
