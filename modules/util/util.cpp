#include <iostream>
#include <string>

#include "util.hpp"

using namespace std;

Util::Util() {}
Util::~Util() {}

void Util::concatenate(char s1[], char s2[])
{
    int i, j;
    i = 0;
    while (s1[i] != '\0') {
        i++;
    }
    j = 0;
    while (s2[j] != '\0') {
        s1[i] = s2[j];
        i++;
        j++;
    }
    s1[i] = '\0';
}

void log_message(const char* message)
{
    FILE* logfile;
    logfile = fopen("/tmp/neodm.log", "a");
    if (!logfile)
        return;
    fprintf(logfile, "%s", message);
    fclose(logfile);
}

char* Util::get_current_user_name()
{
    char* user_name = getenv("USER");
    return user_name;
}

//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/types.h>
// #include<unistd.h>

//if (user_name == NULL) {
//return "";
//} else {
//}
//string user_name = string(getenv("USER"));
//if (user_name.empty()) {
//    user_name = " ";
//}
//return user_name;
