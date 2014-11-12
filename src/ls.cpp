#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <pwd.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <iostream>
using namespace std;

// helper 1: sets variables using arguments.
//  accepts arguments by const reference, and flag variables by reference
void flagCheck(char** argv, const int &argc, vector<char*> &args, bool &lflag, bool &aflag, bool &Rflag)
{
    for (int a = 1; a < argc; a++)
    {
        if (argv[a][0] == '-')
        {
            for (unsigned b = 1; argv[a][b] != ' ' && argv[a][b] != '\0'; b++)
            {
                if (argv[a][b] == 'l')
                {
                    lflag = true;
                }
                else if (argv[a][b] == 'a')
                {
                    aflag = true;
                }
                else if (argv[a][b] == 'R')
                {
                    Rflag = true;
                }
            }
        }
        else
        {
            args.push_back(argv[a]);
        }
    }
}

// helper 2: the lister
void printStat(dirent* direntp)
{
    struct stat stats;
    int t = stat(direntp->d_name, &stats);
    if (t != 0)
    {
        perror("stat");
        exit(1);
    }


    if (direntp->d_type == DT_DIR) //Ouput file type
    {
        cout << "d";
    }
    else if (direntp->d_type == DT_REG)
    {
        cout << "-";
    }

    //cout << stats.st_mode;   //Output permissions
    // owner
    if (stats.st_mode & S_IRUSR)
    {
        cout << "r";
    }
    else
    {
        cout << "-";
    }
    if (stats.st_mode & S_IWUSR)
    {
        cout << "w";
    }
    else
    {
        cout << "-";
    }
    if (stats.st_mode & S_IXUSR)
    {
        cout << "x";
    }
    else
    {
        cout << "-";
    }
    // group
    if (stats.st_mode & S_IRGRP)
    {
        cout << "r";
    }
    else
    {
        cout << "-";
    }
    if (stats.st_mode & S_IWGRP)
    {
        cout << "w";
    }
    else
    {
        cout << "-";
    }
    if (stats.st_mode & S_IXGRP)
    {
        cout << "x";
    }
    else
    {
        cout << "-";
    }
    // other
    if (stats.st_mode & S_IROTH)
    {
        cout << "r";
    }
    else
    {
        cout << "-";
    }
    if (stats.st_mode & S_IWOTH)
    {
        cout << "w";
    }
    else
    {
        cout << "-";
    }
    if (stats.st_mode & S_IXOTH)
    {
        cout << "x";
    }
    else
    {
        cout << "-";
    }

    cout << " " << stats.st_nlink;
    // cout << " " << stats.st_uid;
    cout << " " << getpwuid(stats.st_uid)->pw_name;
    // cout << " " << stats.st_gid;
    cout << " " << getpwuid(stats.st_gid)->pw_name;
    cout << " " << stats.st_size;
    // cout << " " << stats.st_mtime;
    tm* curtime = localtime(&stats.st_mtime);
    cout << " " << curtime->tm_mon + 1 << " " << curtime->tm_mday << " " <<
        curtime->tm_hour << ":" << curtime->tm_min;
    cout << " " << direntp->d_name << endl;
}
void ls(const bool l, const bool r, const bool a, char* path) {
    vector<char*> directories;
    // code to get ALL directories. see stack overflow page or mr izbicki's code
    DIR *cur = opendir(path);
    if (cur == NULL)
    {
        perror("opendir");
        exit(1);
    }
    dirent *direntp;
    while ((direntp = readdir(cur)))
    {
        // if it has a dot and a is true, print it
        if ((direntp->d_name[0] == '.' && a) || (direntp->d_name[0] != '.'))
        {
            // when printing:
            // if l is true, print on separate line with stats
            if (l)
            {
                printStat(direntp);
            }
            else
            {
                cout << direntp->d_name << endl;
            }
            // if r is true and the entry in question is a directory, push onto directories
            if (r && direntp->d_type == DT_DIR)
            {
               directories.push_back(direntp->d_name);
            }
        }
    // if it has a dot and a is false, don't
    }
    // if r is true and directories is not empty, recurse on all of the paths
    while (r && directories.size() != 0)
    {
        int index = directories.size() - 1;
        cout << endl << directories[index] << ":" << endl;
        ls(l,r,a,directories[index]);
        directories.pop_back();
    }
}



int main(int argc, char** argv) {
    // step 1: create variables (as you go)
    vector<char*> args;
    char* dot = new char [2];
    strcpy(dot, ".");
    bool lflag = false;
    bool aflag = false;
    bool Rflag = false;

    // step 2: check flags and set variables accordingly
    // step 3: separate paths to list from flags
    flagCheck(argv, argc, args, lflag, aflag, Rflag);
    // step 4: call the lister on all of the path arguments
    if (args.size() == 0)
    {
        args.push_back(dot);
    }
    while (args.size() != 0)
    {
        ls(lflag, Rflag, aflag, args[args.size()-1]);
        args.pop_back();
    }
    // step 5: find out what directory you're in
    // step 6: list

    delete[] dot;
    return 0;
}
