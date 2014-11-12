#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
using namespace std;

//Returns how many seperate arguments are listed
unsigned count(char* split, char* input)
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
    //If there is only 1 command
    char* input = strdup(cinput);
    unsigned sz = strlen(input);
    //printf("input: %s\ncinput: %s\nstrlen(input): %u\nstrlen(cinput): %u\n"
            //, input, cinput, (unsigned int)strlen(input),
            //(unsigned int)strlen(cinput));
    if (split == NULL)
    {
        //then split its args and run it
        char delim[] = " ";
        args[0] = strtok(input,delim);
        //cout << "strlen(input): " << strlen(input) << endl;
        for (unsigned h = 1; h < sz; ++h)
        {
            args[h] = strtok(NULL,delim);
            //printf("pointer: %p\n", args[h]);
            if (args[h] == NULL)
            {
                break;
            }
            //cout << "args[" << h << "] = " << args[h] << endl;
            //cout << "strlen(input): " << strlen(input) << endl;
        }
        // args[strlen(input)] = NULL;
        // delete[] input;
        return;
    }
    else                                                //If there are multiple commands
    {
        unsigned size = count(split,input);
        char* temp = strtok(input,split);
        for (unsigned k = 1; k < size; ++k)            //Fork enough processes to handle the number of commands
        {                                               //and run in order
            int t = fork();
            if (t != 0)
            {
                int* stat;
                wait(&stat);
                // cout << "stat = " << stat << endl;
                // cout << "split = \"" << split << "\"" << endl;
                temp = strtok(NULL,split);
                if (strcmp(split, "||") == 0 && WEXITSTATUS(stat) != 1) //Logic for "||"
                {
                    temp = strtok(NULL,split);
                    ++k;
                }
                else if (strcmp(split, "&&") == 0 && WEXITSTATUS(stat) != 0) //Logic for "&&"
                {
                    exit(1);
                }
            }
            if (t == 0)
            {
                break;
            }
        }
        char delim[] = " ";                             //Break commands into args and run
        unsigned sz = strlen(temp);
        args[0] = strtok(temp,delim);
        for (unsigned i = 1; i < sz; ++i)
        {
            args[i] = strtok(NULL,delim);
        }
        // args[strlen(temp)] = NULL;
        return;
    }
}
