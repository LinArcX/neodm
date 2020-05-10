#ifndef UTIL_H
#define UTIL_H

#include <string>

class Util {

public:
    Util();
    virtual ~Util();
    void concatenate(char s1[], char s2[]);
    char* get_current_user_name();
};

void log_message(const char* message);

#endif /* UTIL_H */
