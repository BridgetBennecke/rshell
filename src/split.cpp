#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
using namespace std;

int inputRedirect (char* args[])
{
    cout << "inputRedirect" << endl;
    string fileStr;
    for (unsigned a = 0; args[a+1] != '\0'; a++)
    {
        printf("ponter not here args[%u] = \"%s\"\n",a,args[a]);
        if (strcmp(args[a],"<") == 0)
        {
            printf("args[%u] = \"%s\"\n",a+1,args[a+1]);
            fileStr = args[a+1];
            cout << endl;
            args[a] = 0;
            args[a+1] = 0;
            cout << "after deletion" << endl;
            break;
        }
    }
    int file = open(fileStr.c_str(), O_RDONLY);
    if (file == -1)
    {
        perror("open");
        exit(1);
    }
    int fd = dup2(file,0);
    if (fd == -1)
    {
        perror("dup2");
        exit(1);
    }
    return fd;
}

int outputRedirect (char* args[])
{

    cout << "outputRedirect" << endl;
    string fileStr;
    for (unsigned a = 0; args[a+1] != '\0'; a++)
    {
        printf("pointer %p\nargs[%u] = \"%s\"\n",args[a],a,args[a]);
        if (strcmp(args[a],">") == 0)
        {
            printf("pointer %p\nargs[%u] = \"%s\"\n",args[a+1],a+1,args[a+1]);
            fileStr = args[a+1];
            cout << endl;
            args[a] = 0;
            args[a+1] = 0;
            cout << "after deletion" << endl;
            // break;
        }
    }
    printf("File: %s\n",fileStr.c_str());
    int file = open(fileStr.c_str(), O_RDWR);
    if (file == -1)
    {
        perror("open");
        exit(1);
    }
    int fd = dup2(file,1);
    if (fd == -1)
    {
        perror("dup2");
        exit(1);
    }
    return fd;
}

int outputRedirect2 (char* args[])
{
    cout << "outputRedirect2" << endl;
    string fileStr;
    for (unsigned a = 0; args[a+1] != '\0'; a++)
    {
        printf("pointer %p\nargs[%u] = \"%s\"\n",args[a],a,args[a]);
        if (strcmp(args[a],">>") == 0)
        {
            printf("pointer %p\nargs[%u] = \"%s\"\n",args[a+1],a+1,args[a+1]);
            fileStr = args[a+1];
            cout << endl;
            args[a] = 0;
            args[a+1] = 0;
            cout << "after deletion" << endl;
            // break;
        }
    }
    printf("File: %s\n",fileStr.c_str());
    int file = open(fileStr.c_str(), O_RDWR, O_APPEND);
    if (file == -1)
    {
        cout << "p-erroring" << endl;
        perror("open");
        exit(1);
    }
    lseek(file, 0, SEEK_END);
    int fd = dup2(file,1);
    if (fd == -1)
    {
        perror("dup2");
        exit(1);
    }
    return fd;
}

bool hasRedirect(char* temp)
{
    unsigned size = strlen(temp);
    for (unsigned r = 0; r < size; r++)
    {
        // printf("temp[%u] = %c\n",r,temp[r]);
        if (temp[r] == '<'|| temp[r] == '|'|| temp[r] == '>')
        {
            return true;
        }
    }
    return false;
}

//Returns how many seperate arguments are listed
unsigned count(char* split, char* input)
{
    unsigned count = 1;
    if (strcmp(split, ">>") != 0)
    {
        for (unsigned c = 0; c < strlen(input); ++c)
        {
            if (input[c] == split[0])
            {
                count += 1;
                // if (split[1] == split[0])
                // {
                    // ++c;
                // }
            }
        }
    }
    else
    {
        for (unsigned c = 0; c < strlen(input); ++c)
        {
            if (input[c] == split[0] && input[c+1] == split[1])
            {
                count += 1;
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
        if (hasRedirect(input))
        {
            cout << "Redirect isn't empty" << endl;
            int savestdin = dup(0); // Save the stdin and stdout nums
            if (savestdin == -1)
            {
                perror("dup");
                exit(1);
            }
            int savestdout = dup(1);
            if (savestdout == -1)
            {
                perror("dup");
                exit(1);
            }
            int fd = -5;

            char delim[] = "|";
            char* temp2 = strtok(input,delim); // Split up by pipe if necessary
            unsigned sz = count(delim, input);

            for (unsigned h = 1; h < sz; h++)
            {
                cout << "Forking" << endl;
                int e = fork();
                if (e == -1)
                {
                    perror("fork");
                    exit(1);
                }
                else if (e != 0) // Parent Process
                {
                    cout << "In parent" << endl;
                    int* stat;
                    int c = wait(&stat);
                    if (c == -1)
                    {
                        perror("wait");
                        exit(1);
                    }
                    if (fd != -5)
                    {
                        if (close(fd) == -1)
                        {
                            perror("close");
                            exit(1);
                        }
                        if (dup2(savestdin,0) == -1)
                        {
                            perror("dup2");
                            exit(1);
                        }
                        if (dup2(savestdout,1) == -1)
                        {
                            perror("dup2");
                            exit(1);
                        }
                    }
                    temp2 = strtok(NULL,delim);
                }
                else if (e == 0) // Child Process
                {
                    cout << "In child" << endl;
                    break;
                }
            }
            bool in = false;
            bool out = false;
            bool out2 = false;

            char inRed[] = "<";
            char outRed[] = ">";
            char outRed2[] = ">>";

            if (count(inRed,temp2) != 1) // Determine if non-piping redirection
            {
                in = true;
            }
            else if (count(outRed2,temp2) != 1)
            {
                out2 = true;
            }
            else if (count(outRed,temp2) != 1)
            {
                out = true;
            }
            else
            {
                cout << "no count worked... fail" << endl;
            }


            char delim2[] = " ";
            unsigned siz = strlen(temp2);
            args[0] = strtok(temp2,delim2);
            cout << "Args[0] = " << args[0] << endl;
            for (unsigned i = 1; i < siz; ++i)
            {
                args[i] = strtok(NULL,delim2);
                printf("Args[%u] = %s\n",i,args[i]);
                if (args[i] == NULL)
                {
                    break;
                }
            }

            if (in)
            {
                fd = inputRedirect(args);
            }
            else if (out)
            {
                fd = outputRedirect(args);
            }
            else if (out2)
            {
                fd = outputRedirect2(args);
            }
        }
        else
        {
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
            char delim[] = " ";         //Break commands into args and run
            unsigned sz = strlen(temp);
            args[0] = strtok(temp,delim);
            for (unsigned i = 1; i < sz; ++i)
            {
                args[i] = strtok(NULL,delim);
            }
        return;
    }
}
