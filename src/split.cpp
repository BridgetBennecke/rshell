#include <list>
#include <string>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
using namespace std;

list<string> listRedirect(char* temp,char* directions[])
{
    list<string> redirect;
    unsigned counter = 0;
    unsigned size = strlen(temp);
    // cout << "Size = " << size << endl;
    for (unsigned r = 0; r < size; r++)
    {
        // printf("temp[%u] = %c\n",r,temp[r]);
        // cout << "args[" << r << "][" << j << "] = " << args[r][j] << endl;
        if (temp[r] == '<')
        {
            // cout << '<' << endl;
            redirect.push_back("<");
            strcpy(directions[counter],"<");
            counter++;
        }
        else if (temp[r] == '|')
        {
            // cout << '|' << endl;
            // printf("In pipe");
            strcpy(directions[counter],"|");
            redirect.push_back("|");
            counter++;
            // printf("Leaving pipe");
        }
        else if (temp[r] == '>')
        {
            // cout << '>' << endl;
            if (temp[r+1] != 0 && temp[r+1] == '>')
            {
                strcpy(directions[counter],">>");
                redirect.push_back(">>");
                counter++;
            }
            else
            {
                strcpy(directions[counter],">");
                redirect.push_back(">");
                counter++;
            }
        }
        // else
        // {
            // // printf("'%c' not handled\n", temp[r]);
        // }
    }
    return redirect;
}

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
    if (split == NULL)
    {
        // printf("In single\n");
        //then split its args and run it
        // char** directions = new char*;

        char** directions = new char*[200];
        for(unsigned i = 0; i < 200; i++)
        {
            directions[i] = new char[3];
        }

        list<string> redirect = listRedirect(input, directions);
        if (!redirect.empty())
        {
            cout << "Redirect isn't empty" << endl;
            unsigned counter = 0;
            unsigned sz = redirect.size();
            cout << "Redirect.size = " << sz << endl;
            char* temp2 = strtok(input,directions[0]);
            for (unsigned h = 1; h <= sz; h++)
            {
                cout << "Forking" << endl;
                int e = fork();
                if (e == -1)
                {
                    perror("fork");
                }
                else if (e != 0)
                {
                    cout << "In parent" << endl;
                    int* stat;
                    int c = wait(&stat);
                    if (c == -1)
                    {
                        perror("wait");
                        exit(1);
                    }
                    ++counter;
                    temp2 = strtok(NULL,directions[counter]);
                }
                else if (e == 0)
                {
                    cout << "In child" << endl;
                    break;
                }
            }
            char delim[] = " ";
            unsigned siz = strlen(temp2);
            args[0] = strtok(temp2,delim);
            cout << "Args[0] = " << args[0] << endl;
            for (unsigned i = 1; i < siz; ++i)
            {
                args[i] = strtok(NULL,delim);
                printf("Args[%u] = %s\n",i,args[i]);
            }
        }
        else
        {
            printf("Pipe doesn't exist\n");
            char delim[] = " ";
            args[0] = strtok(input,delim);
            for (unsigned h = 1; h < sz; ++h)
            {
                 args[h] = strtok(NULL,delim);
                 if (args[h] == NULL)
                 {
                      break;
                 }
            }
        }
        return;
    }
    else            //If there are multiple commands
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
        char** directions = new char*;
        list<string> redirect = listRedirect(temp, directions);
        if (!redirect.empty())
        {
            unsigned counter = 0;
            unsigned sz = redirect.size();
            char* temp2 = strtok(temp,directions[0]);
            for (unsigned h = 1; h <= sz; h++)
            {
                int e = fork();
                if (e == -1)
                {
                    perror("fork");
                }
                else if (e != 0)
                {
                    int* stat;
                    int c = wait(&stat);
                    if (c == -1)
                    {
                        perror("wait");
                        exit(1);
                    }
                    ++counter;
                    temp2 = strtok(NULL,directions[counter]);
                }
                else if (e == 0)
                {
                    break;
                }
            }
            char delim[] = " ";         //Break commands into args and run
            unsigned siz = strlen(temp2);
            args[0] = strtok(temp2,delim);
            for (unsigned i = 1; i < siz; ++i)
            {
                args[i] = strtok(NULL,delim);
            }
        }
        else
        {
            char delim[] = " ";         //Break commands into args and run
            unsigned sz = strlen(temp);
            args[0] = strtok(temp,delim);
            for (unsigned i = 1; i < sz; ++i)
            {
                args[i] = strtok(NULL,delim);
            }
        }
        return;
    }
}
